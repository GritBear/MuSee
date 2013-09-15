#include "ArtStoryCore.h"

//=================================================================
//Core Functions
//Author: Li(Lee) Xiong. All rights reserved.
//=================================================================
ArtStoryCore::ArtStoryCore(){
	displaying = false;
	textureLoaded = false;

	theReader = new DataReader(); //DataReader has no Init()

	//debugCnt = 0;

	pulseIter = 0;
	melodyExtractRate = ExtractRate; 

	//Pulse rate parameters
	idlingPulseFreq = kStoppedPulseRateInHz;
	playingPulseFreq = kPlayingPulseRateInHz;

	// Set the parameters for spectrum analysis
	pulseIterUpperLimit = (int)playingPulseFreq/melodyExtractRate;
	if(pulseIterUpperLimit<1)
		pulseIterUpperLimit = 1;
	
	// Set channel parameters
	this->numSpectrumEntries = kVisualNumSpectrumEntries/2;
	this->numWaveformEntries = kVisualNumSpectrumEntries; //Default 2 times the number of spectrum entries
	numMaxChannels = 2; //default 2 channels

	FrequencyInterval = MP3SamplingRate/kVisualNumSpectrumEntries;

	// Calculate Frequency Bands
	ConstructFrequencyBands();

	// Initialize the spectrum storage matrix
	vector< vector<float> > storageVect(MaxFrequencyIndex, vector<float>(pulseIterUpperLimit, 0));
	melodyStoragesVec = storageVect;

	melodyStoragesVecSize1 = melodyStoragesVec.size();
	melodyStoragesVecSize2 = melodyStoragesVec[0].size();

	// Initialize Tone variable
	num_tones = MaxTone + 1; // gives some extra upper room
	dynamicThreshold = valueThreshold;

	//initialize RhythmicExtractionCore
	theRhythmicExtract = new RhythmicExtract(numWaveformEntries, numMaxChannels);

	Init();
};

void ArtStoryCore::Destroy(){
	//the log writer stream
	if(logWriteStream.is_open()){
		logWriteStream.close();
	}
	//this is useless at this point (the program is closing anyway)
	//Destroy oll melodyline objects
	if(MelodyObjList.size() > 0){
		for(melodyIter = MelodyObjList.begin(); melodyIter != MelodyObjList.end();){
			(*melodyIter)->Destroy();
			delete (*melodyIter);
			melodyIter = MelodyObjList.erase(melodyIter);
		}
	}
	
	//Destroy Background
	theBackground->Destroy();
	delete(theBackground);
	
	//Empty storages
	RemoveTooOldEntries(true);
	theReader->Destroy();
	delete(theReader);

	theCreationCore->Destroy();
	delete(theCreationCore);

	theRhythmicExtract->Destroy();
	delete(theRhythmicExtract);

	DeactiveGL();
	DeleteDC(hDC);
};

void ArtStoryCore::Init(){
	//initialize CreationCore
	CreationCoreComponent aCreationCoreComponentStruct;
	aCreationCoreComponentStruct.aReader = theReader;
	aCreationCoreComponentStruct.aRhythmicExtract = theRhythmicExtract;

	theCreationCore = new CreationCore(&aCreationCoreComponentStruct);

	theSpeedControl = new SpeedControl();
	theSpeedControl->SetVisualSoundBoolPtr(&displaying,&playing);

	//initialize control objects
	memset(&theControlObj,0,sizeof(theControlObj)); //zero the entire struct
	theControlObj.theCreationCore = theCreationCore;
	theControlObj.theReader = theReader;
	theControlObj.theSpeedControl = theSpeedControl;
	theControlObj.screenH = &screen_h;
	theControlObj.screenW = &screen_w;
	theControl = new control(&theControlObj);


	//intialize melody object
	if(MelodyObjList.size() == 0){
		MelodyObj *TopMelody = new MelodyObj(theControl);
		
		TopMelody->Init(NotATone, pulseIterUpperLimit);
		TopMelody->SetIsTheHighestLine(true);
	
		MelodyObjList.push_back(TopMelody);
	}

	//theBackground = new Background(&theControlObj);
	theBackground = new Background(theControl);


	//initialize the creative core
	Creative::SetControl(theControl);
	Creative::SetBackground(theBackground);
	Creative::SetMelodyList(&MelodyObjList);

	//theBackground->Init();

	// Signiture output
	//InfoOut = "By: Kuma.G";
	//InfoOut = "test";
	//InfoOut += "\nLoad Stage: ";
	//InfoOut += num2str(theReader->InfoOutMessage());
};

void ArtStoryCore::PulseUpdate(UInt32 TimeInMS, UInt32 timeStampID, const RenderVisualData * RenderData, UInt32 * ioPulseRate){
	
	bool FullMatrix = false;
	bool nextValueReady = false;

	//update control
	theControl->Update();

	//update 
	UpdatePulseRates(ioPulseRate);

	//Conduct Melody Analysis if playing
	if(playing && displaying){
		
		renderData = *RenderData;
		//update numberOfMaxChannels
		numMaxChannels = renderData.numSpectrumChannels;
		//---------------------------------------------------------------------
		//Time Domain Energy Extraction
		//---------------------------------------------------------------------
		theRhythmicExtract->Update(&renderData); //disable for later implementation
		

		//---------------------------------------------------------------------
		//Frequency Domain Analysis
		//---------------------------------------------------------------------
		//perform SpectrumMatrix storage
		FullMatrix = MelodyStore();
		ObjectUpdate();
		
		if(FullMatrix){
			SpectrumMatrixAnalysis();
			MelodyLineAssignment();
			RemoveTooOldEntries();

			writeLog(); //write to log
		}

		//---------------------------------------------------------------------
		//Creation Core Event Update
		//---------------------------------------------------------------------
		theCreationCore->Update();
	}
};

//update the animation objects
void ArtStoryCore:: ObjectUpdate(){
	Creative::UpdateCreative();
	
	//update background
	theBackground->Update();

	//update all the melody lines
	for(melodyIter = MelodyObjList.begin(); melodyIter != MelodyObjList.end(); ++melodyIter){
		//if((*melodyIter)->GetAlive()||(*melodyIter)->GetStationary()||(*melodyIter)->GetRevivable()){
		if((*melodyIter)->GetAlive()){
			(*melodyIter)->Update();			
		}
	}
}

void ArtStoryCore::Render(){
	DebugUpdate();

	if (destView != NULL && displaying && textureLoaded)
	{	
		ClearBackground();
		//OpenGL drawing code
		//--------------------------------------------------------------------------------------------------------
		
		//render the background
		theBackground->Render(playing);
		
		//update all the melody lines (in Reverse Order)
		
		if(MelodyObjList.size()>0){
			for(RmelodyIter = MelodyObjList.rbegin(); RmelodyIter != MelodyObjList.rend(); ++RmelodyIter){
				if((*RmelodyIter)->GetAlive()||(*RmelodyIter)->GetStationary())
				(*RmelodyIter)->Render(screen_w, screen_h, relative_w, relative_h, playing);			
			}
		}
		
		SwapBuffers(hDC);
		//---------------------------------------------------------------------------------------------------------
		// Draw an out string message
		InfoOutToScreen();
		//---------------------------------------------------------------------------------------------------------
	}
	
};


//-------------------------------------------------------------------------------------------------
//	ActivateVisual
//-------------------------------------------------------------------------------------------------
/* This is use to assign important information: view to destView, option to destOptions, 
  												get screen dimension into a RECT struct
   SetWindowLong: DWL_DLGPRO->Sets the new address of the dialog box procedure. 
*/
void ArtStoryCore::ActivateVisualizer(VISUAL_PLATFORM_VIEW destView, OptionBits options){
	GetClientRect( destView, &destRect);
	
	ArtStoryCore::destView = destView;
	ArtStoryCore::destOptions = options;

	//debugCnt++;

	displaying = true;

	CreatContext();
	InitGL();
	LoadTexture();
	GetScreenDimension();
}

//-------------------------------------------------------------------------------------------------
//	DeactivateVisual
//-------------------------------------------------------------------------------------------------
void ArtStoryCore::DeactivateVisualizer()
{
	displaying = false; 
	//debugCnt++;
	GetScreenDimension();
	DeactiveGL();
}

void ArtStoryCore::MoveVisual(OptionBits newOptions){
	destOptions = newOptions;
	GetClientRect(destView, &destRect);

	//SetDisplaying(true);
	GetScreenDimension();
	ObjectUpdate();
}

void ArtStoryCore::ResizeVisual(){
	GetClientRect(destView, &destRect);

	//SetDisplaying(true);
	GetScreenDimension();
	ObjectUpdate();
}

//=================================================================
//General OpenGL methods
//=================================================================
void ArtStoryCore::InitGL(){
	//intialize aspect preserved resize
	resizedBottom = resizedLeft = 0.0;
	resizedTop = resizedRight = 1.0;
	
	// initialize random seed: 
	srand((unsigned)time(NULL));
	
	glEnable(GL_TEXTURE_2D);
	// Choose a smooth shading model
	glShadeModel(GL_SMOOTH);

	//Depth test
	glClearDepth(farthestVisibleDistance);			// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);                        // Enable Depth Testing
	glDepthFunc(GL_LEQUAL);                         // Type Of Depth Testing


	// Set the clear color to white
	glClearColor(1.0, 1.0, 1.0, 1.0);

	// Enable the alpha test (Use either this or alpha blending). This is needed 
	// to be able to have images with transparent 
	// parts.
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.05f);

	//initial resize function
	ResizeGL();
};

/*Redesign Needed*/
void ArtStoryCore::ResizeGL(){
	GLsizei width = screen_w;
	GLsizei height = screen_h;
	AspectRatioPreserveResize();
	if (height == 0) height = 1;
    // Sets the size of the OpenGL viewport
    glViewport(0,0,width,height);

	 // Select the projection stack and apply
	// an orthographic projection
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	//glOrtho(0.0,width,height,0.0,-farthestFrontDistance, farthestVisibleDistance); //working implementation
	glOrtho(resizedLeft,resizedRight,resizedBottom,resizedTop,-farthestFrontDistance, farthestVisibleDistance); //working implementation

	glMatrixMode(GL_MODELVIEW);
}

//Set the background color
void ArtStoryCore::ClearBackground(){
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); //| GL_CURRENT_BIT | GL_POLYGON_BIT);	
}

//load Textures
void ArtStoryCore::LoadTexture(){
	if(!textureLoaded){
		textureLoaded = theReader->TextureLoadDrive();

		if(!theBackground->alreadyInitialized){
			theBackground->Init();
		}
	}
}

//load Textures
void ArtStoryCore::AspectRatioPreserveResize(){
	//the grand canvas is  bottom = 0, top = 1, left = 0, right = 1;
	//want to show screen as resonable as possible
	//thoughts:
	// want to preserve top and bottom as much as possible, later will 
	float H_Wratio =((float)screen_h) / ((float)screen_w);

	if(H_Wratio<= (1.0/ OpenGLXSpan)){
		resizedLeft = BackgrounOpenGLLeft;
		resizedRight = BackgrounOpenGLRight;
		
		resizedBottom = 0.5 - OpenGLXSpan*H_Wratio/2.0;
		resizedTop = 0.5 + OpenGLXSpan*H_Wratio/2.0;
	}else{
		resizedBottom  = 0.0;
		resizedTop = 1.0;
		resizedLeft = 0.5 - 1.0/H_Wratio/2.0;
		resizedRight = 0.5 + 1.0/H_Wratio/2.0;
	}
}

//=================================================================
//Helper methods
//=================================================================
//------------------------------------------------------
//	UpdatePulseRate
//------------------------------------------------------
void ArtStoryCore::TrackChanged(){
	theSpeedControl->Reset();
	RemoveTooOldEntries(true);

	if(MelodyObjList.size()>0){
		for(RmelodyIter = MelodyObjList.rbegin(); RmelodyIter != MelodyObjList.rend(); ++RmelodyIter){
			(*RmelodyIter)->RemoveTooOldEntries(true);	
		}
	}
	
	theCreationCore->TractChange();
}

//use fps to update
void ArtStoryCore::UpdatePulseRates(UInt32 * ioPulseRate){
	*ioPulseRate = (playing) ? kPlayingPulseRateInHz : kStoppedPulseRateInHz;
};

void ArtStoryCore::GetScreenDimension(){
	
	GetClientRect(destView, &clientRect );

	screen_w = clientRect.right - clientRect.left;
	screen_h = clientRect.bottom - clientRect.top;

	relative_w = resizedRight - resizedLeft;
	relative_h = resizedTop - resizedBottom;

	//also update the OpenGL screen
	ResizeGL();
}

//------------------------------------------------
//Remove Too Old Entries
//------------------------------------------------
void ArtStoryCore::RemoveTooOldEntries(bool Removeall){
	if(Removeall){
		FreeAll(stepMelodyTestOut);
		FreeAll(stepMelody);
		FreeAll(melodyStoragesVec);

		//empty states
		theRhythmicExtract->EmptyRhythmicStates();
	}else{

		//Remove too old spectrums
		while(stepMelodyTestOut.size()>maxExtractedStorageEntries){
			FreeAll(stepMelodyTestOut.front());
			stepMelodyTestOut.pop_front();	
		}

		while(stepMelody.size()>maxExtractedStorageEntries){
			FreeAll(stepMelody.front());
			stepMelody.pop_front();	
		}

	}
}
