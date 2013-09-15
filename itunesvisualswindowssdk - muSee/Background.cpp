#include "Background.h"

//=========================================================================================
//Core functions
//=========================================================================================
const int thousand = 1000;

Background::Background(control * TheControl){
	alreadyInitialized = false;
	readingSuccessful = false;
	onlyUsingPrecompiledAnimationVariables = false;
	usingTexture = true;
	animationIndexAlreadyChose = false;
	parameterGenerated = false;
	previousVerticalAnimationSpawningTime = 0;
	perviousAnimationOrientation = 0;

	selfMotion = preDrawn = false; //just to assign a value

	//copy control pointers
	theControl = TheControl;
	theReader = TheControl->GetReader();
	theCreationCore = TheControl->GetCreationCore();
	theSpeedControl = TheControl->GetSpeedControl();

	theChildren = new AniDispatcher();
	theChildren->SetCleanLim(-1.0, 3, 2, -2);
	theChildren->SetActivateMode(Sequential);

	theAllActiveChildren = new AniDispatcher();
	theAllActiveChildren->SetCleanLim(-1.0, 3, 2, -2);
	theAllActiveChildren->SetActivateMode(AllActive);

	memset(&aVerticalStruct, 0, sizeof(VerticalAnimationStruct));
	colorInst = 1;
	Init();
}

void Background::Destroy(){
	//free(&aVerticalStruct); this cause error
	delete(theChildren);
	delete(theAllActiveChildren);
	
	RemoveTooOldEntries(true);
}

void Background::Init(){
	testGenerated = false;
	alreadyInitialized = true;
	
	theCreationCore->NextBackground(ThemeID);
	theReader->BackgroundInfo(ThemeID, percentageVel, XSpan, YSpan, width, height, usingTexture, textureID);
		
	//White Background
	//usingTexture = false; //set to white background

	XVel = BackgroundSpeed(1, XSpan, OpenGLXSpan); //get the speed per second
	YVel = 0;
		
	onlyUsingPrecompiledAnimationVariables = true;
	readingSuccessful = true;

	percentageInTheTheme = 0.0;
	tX_left = 0.0;
	tX_right = tX_left + XSpan;

	tY_bottom = 0.0;
	tY_top = tY_bottom + YSpan;

	timeStep = (1000.0 /((float)kPlayingPulseRateInHz));

}

void Background::Update(){
	curTimeInMS = theSpeedControl->GetTimeVisualSound() * thousand;

	UpdateThemeParameters();	//update the background
	NexAnimationObjGenerate(); //call it every time (time control is embedded)
	
	theChildren->Update();
	theAllActiveChildren->Update();

	//update all the verticalAnimation Objects
	if(AnimationObjList.size()>0){
		for(AnimationObjListIter = AnimationObjList.begin(); AnimationObjListIter != AnimationObjList.end(); ++AnimationObjListIter){
				(*AnimationObjListIter)->Update();//call to render each object
		}
	}

	//update flying animation object
	if(flyingAniObjList.size()>0){
		for(flyingAniObjIter = flyingAniObjList.begin(); flyingAniObjIter != flyingAniObjList.end(); ++flyingAniObjIter){
			if((*flyingAniObjIter)->GetAlive())
			(*flyingAniObjIter)->Update();//call to render each object
		}
	}
}

void Background::Render(bool updating){
	GLfloat top, bottom, left, right;

	screen_w = theControl->GetScreenW();
	screen_h = theControl->GetScreenH();

	if(usingTexture){
		bottom  = 1.0;
		left = BackgrounOpenGLLeft;
		top = 0.0;
		right = BackgrounOpenGLRight;


		glEnable(GL_TEXTURE_2D);
		glBindTexture( GL_TEXTURE_2D, textureID );
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
			
		glPushMatrix();
		glBegin(GL_QUADS);
		glColor4f(1.0, 1.0, 1.0, 1.0);	//this is necessary to ensure the correct color after blending	
			//glTexCoord2f coordinate in range of 0,1s
			glTexCoord2f(tX_left,tY_bottom);	glVertex3f(left,top,-farthestVisibleDistance);
			glTexCoord2f(tX_right,tY_bottom);	glVertex3f(right,top,-farthestVisibleDistance);
			glTexCoord2f(tX_right,tY_top);		glVertex3f(right,bottom,-farthestVisibleDistance);
			glTexCoord2f(tX_left,tY_top);		glVertex3f(left,bottom,-farthestVisibleDistance);
			
		glEnd();
		glPopMatrix();
			
		glDisable(GL_BLEND);

		glDisable(GL_TEXTURE_2D);	
	
	}else{
		bottom  = 1.0;
		left = BackgrounOpenGLLeft;
		top = 0.0;
		right = BackgrounOpenGLRight;


		glDisable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glPushMatrix();
		glBegin(GL_QUADS);

		//specify the bottom color
		//default white screen
		glColor4f(1.0, 1.0, 1.0, 1.0);	
		glVertex3f(left,top,-farthestVisibleDistance);
		glVertex3f(right,top,-farthestVisibleDistance);

		glVertex3f(right,bottom,-farthestVisibleDistance);
		glVertex3f(left,bottom,-farthestVisibleDistance);
		
		glEnd();
		glPopMatrix();
		glDisable(GL_BLEND);
		
	}

	
	//render the verticalAnimation Objects
	if(AnimationObjList.size()>0){
		for(AnimationObjListIter = AnimationObjList.begin(); AnimationObjListIter != AnimationObjList.end(); ++AnimationObjListIter){
			if((*AnimationObjListIter)->GetAlive() || (*AnimationObjListIter)->GetStationary())
				if((*AnimationObjListIter)->GetToRender())
					(*AnimationObjListIter)->Render(screen_w, screen_h, updating);//call to render each object
		}
	}
	
	//update flying animation object
	if(flyingAniObjList.size()>0){
		for(flyingAniObjIter = flyingAniObjList.begin(); flyingAniObjIter != flyingAniObjList.end(); ++flyingAniObjIter){
			if((*flyingAniObjIter)->GetToRender())
			(*flyingAniObjIter)->Render();//call to render each object
		}
	}	

	theChildren->Render();
	theAllActiveChildren->Render();
}

void Background::UpdateThemeParameters(){
	if(usingTexture){
		XVelUpdate = theSpeedControl->SpeedNormalization(XVel);
		YVelUpdate = theSpeedControl->SpeedNormalization(YVel);

		percentageInTheTheme += percentageVel;
		float shiftingconst = 1000.0;
		tX_left += XVelUpdate;
		tX_right += XVelUpdate;
		tY_bottom += YVelUpdate; 
		tY_top += YVelUpdate;
	
		//some overflow preventions
		if(tX_left > shiftingconst || tX_right > shiftingconst){
			tX_left -= shiftingconst;
			tX_right -= shiftingconst;
		}

		if(tY_bottom > shiftingconst || tY_top > shiftingconst){
			tY_bottom -= shiftingconst;
			tY_top -= shiftingconst;
		}

		if(percentageInTheTheme>=1) //set to zero if exceed 1
			percentageInTheTheme = 0.0;

	//if using default implementation
	}else{
		/*
		red_top += redVel_top;
		green_top += greenVel_top;
		blue_top += blueVel_top;

		red_bottom = redVel_bottom;
		green_bottom = greenVel_bottom;
		blue_bottom = blueVel_bottom;
		
		//update new random speed if the threshold is reached
		if(red_top >= maxRed || red_top<= minRed)
			redVel_top = GenerateNextVelocity(redVel_top);

		if(green_top >= maxGreen || green_top<= minGreen)
			greenVel_top = GenerateNextVelocity(greenVel_top);

		if(blue_top >= maxBlue || blue_top<= minBlue)
			blueVel_top = GenerateNextVelocity(blueVel_top);

		if(red_bottom >= maxRed || red_bottom<= minRed)
			redVel_bottom = GenerateNextVelocity(redVel_bottom);
		
		if(green_bottom >= maxGreen || green_bottom<= minGreen)
			greenVel_bottom = GenerateNextVelocity(greenVel_bottom);
		
		if(blue_bottom >= maxBlue || blue_bottom<= minBlue)
			blueVel_bottom = GenerateNextVelocity(blueVel_bottom);
		*/
	}
	RemoveTooOldEntries();
}

void Background::NextAnmiationInfo(){
	//-----------------------------------------------------------------------
	// Probablistic Next Animation Index Draw
	//-----------------------------------------------------------------------
	//random animation object index generation
	theCreationCore->NextVerticalAnimation(percentageInTheTheme, targetAnimationIndex, probabilityVect);

	animationIndexAlreadyChose = true;
}

void Background::NextAnmiationInfo(IndexThemeControl * theIndexStruct){
	targetAnimationIndex = theIndexStruct->animationIndex;
	ThemeID = theIndexStruct->themeID;
	animationIndexAlreadyChose = true;
}

void Background::NexAnimationObjGenerate(){
	if(!animationIndexAlreadyChose){
		NextAnmiationInfo();
	}

	//-----------------------------------------------------------------------
	//Calculate necessary condition and spawn the animation object
	//-----------------------------------------------------------------------
	bool spawndingConditionGood = false;
	spawndingConditionGood = BestMatchParameters(targetAnimationIndex);

	
	//testing here
	if(spawndingConditionGood){
		//for test demo
		//legacyVertical(); create legacy grass

		AStruct theAniStruct, theAniStructFlower;
		//fill the test Astruct
		memset(&theAniStruct,0,sizeof(AStruct));
		memset(&theAniStructFlower,0,sizeof(AStruct));

		theAniStruct.usingTex = true;
		theAniStruct.aControl = theControl;
		theAniStruct.texInfo = theReader->GetTextureInfo(1, VerticalAnimationTexture); //reading the square

		theAniStructFlower.usingTex = true;
		theAniStructFlower.aControl = theControl;
		theAniStructFlower.texInfo = theReader->GetTextureInfo(2, AnimationTexture); //reading the flower
		theAniStructFlower.blendColor = (blendColorMode)(int)UniformDistribution(1,11.9);;
		colorInst++;
		theAniStructFlower.effectMode = fadeIn;
		theAniStructFlower.enterTime = 0.5;

		theAniStruct.effectMode = fadeIn;
		theAniStruct.vertex = 2;
		theAniStruct.theShape = rect_ver;
		theAniStruct.angle = halfpi;
		theAniStruct.damp = 0.95;
		theAniStruct.angIndexOffset = 0;
		theAniStruct.angConstant = 0.35;
		theAniStruct.angOrder = 3;
		theAniStruct.enableAcc = true;
		theAniStruct.straightLine = false;
		theAniStruct.enterTime = 1;
		theAniStruct.effectTime = 5;//5 seconds from bending
		theAniStruct.maxAngleSpeed = halfpi / 4;

		theAniStruct.aSpeedQ=deque<Vector3>(2);
		theAniStruct.aPositionQ=deque<Vector3>(2);

		theAniStructFlower.vertex = 4;
		theAniStructFlower.theShape = rect_ver;
		theAniStructFlower.aSpeedQ=deque<Vector3>(4);
		theAniStructFlower.aPositionQ=deque<Vector3>(4);

		theAniStruct.aPositionQ[0].X = 1.0;
		theAniStruct.aPositionQ[0].Y = 0;
		theAniStruct.aPositionQ[0].Z = -0.1;

		theAniStruct.aPositionQ[1].X = 1.1;
		theAniStruct.aPositionQ[1].Y = 0.1;
		theAniStruct.aPositionQ[1].Z = -0.1;

		theAniStruct.aSpeedQ[0].X = 0;
		theAniStruct.aSpeedQ[0].Y = 0;
		theAniStruct.aSpeedQ[0].Z = 0;
		theAniStruct.aSpeedQ[1].X = 0;
		theAniStruct.aSpeedQ[1].Y = 0;
		theAniStruct.aSpeedQ[1].Z = 0;

		//create flower
		VectorDQueZero(&theAniStructFlower.aSpeedQ);
		VectorDQueZero(&theAniStructFlower.aPositionQ);
		VectorDequeCreateSquare(&theAniStructFlower.aPositionQ, 0.1);

		theAniStruct.width = 0.08;
		theAniStruct.minWidth = 0.01;
		theAniStruct.widthOrder = 1;
		theAniStruct.length = 0.3;

		//generate the treebranch

		ATreeBranch * theBranch = new ATreeBranch(&theAniStruct);
		AObject *theFlower = new AObject(&theAniStructFlower);

		theAniStruct.length = 0.25;
		theAniStruct.width = 0.05;
		theAniStruct.minWidth = 0.008;
		//theAniStruct.angle = halfpi;
		theAniStruct.angConstant = 0.3;
		theAniStruct.angOrder = 2;

		theAniStruct.aPositionQ[0].X = 0;
		theAniStruct.aPositionQ[0].Y = 0;

		theAniStruct.aPositionQ[1].X = UniformDistribution(-0.35, 0.35);
		theAniStruct.aPositionQ[1].Y = 0.3;

		theBranch->CreateEndBranch(&theAniStruct,0.6,headCenter);

		theAniStruct.length = 0.20;
		theAniStruct.angConstant = 0.4;
		theAniStruct.angOrder = 2;
		theAniStruct.width = 0.04;
		theAniStruct.minWidth = 0.005;
		
		theAniStruct.aPositionQ[0].X = 0.0;
		theAniStruct.aPositionQ[0].Y = 0;
	
		theAniStruct.aPositionQ[1].X = UniformDistribution(-0.35, 0.35);
		theAniStruct.aPositionQ[1].Y = 0.3;
	
		theBranch->CreateEndBranch(&theAniStruct,0.6,headCenter);
		theBranch->InsertAnimationInChildBranch(theFlower,1,headCenter,1);

		//update into storages
		theAllActiveChildren->Append(theBranch);
		theBranches.push_back(theBranch);
	}
}

bool Background::BestMatchParameters(int AnimationIndex){
	/*	Parameters Need to be Generated:

		centerBottomX, centerBottomY, centerTopX, centerTopY, xVel, yVel
		curTimeInMS, animationOrientation, time_durarion_frame_delay
		selfMotion, preDrawn
	*/
	if(!parameterGenerated){
		theReader->GetBackgroundVerticalAnimationSpawningParameters(AnimationIndex, maxTime, minTime, meanTime,
			maxHeight, minHeight, meanHeight, maxAngle, minAngle, meanAngle);

		nextTime = GaussianDistribution(minTime, meanTime, maxTime);

		nextHeight = GaussianDistribution(minHeight, meanHeight, maxHeight);
		nextAngle = GaussianDistribution(minAngle, meanAngle, maxAngle);
		
		centerBottomX = GaussianDistribution(defaultBackgroundMinBottomX, defaultBackgroundMeanBottomX, defaultBackgroundMaxBottomX);
		centerBottomY = 0.0;

		//calculate all parameters here 
		//first find the centerBottomX
		centerTopX = centerBottomX + nextHeight * sin((float)nextAngle/180.0 * pi);
		centerTopY = centerBottomY + nextHeight * cos((float)nextAngle/180.0 * pi);

		yVel = 0;
		xVel = BackGroundVerticalAnimationObjSpeed(); //same speed as the background

		animationOrientation = rand()%2; //either 0 or 1

		time_durarion_frame_delay = (int)GaussianDistribution(0, 1, 2);

		parameterGenerated = true;
	}

	//check time constraints here-------------------------------------
	if(curTimeInMS < previousVerticalAnimationSpawningTime){
		previousVerticalAnimationSpawningTime = curTimeInMS;
		return false;
	}
	//--------------------------------------------------------------------

	if(curTimeInMS - previousVerticalAnimationSpawningTime >= nextTime ){
		previousVerticalAnimationSpawningTime = curTimeInMS;
		parameterGenerated = false;
		return true;
	}else{
		return false;
	}
}

//=========================================================================================
//Helper Methods: for default color change (relic code)
//=========================================================================================
float Background::GenerateNextVelocity(float previousVel){
	int precision = 1000;
	//srand((unsigned)time(NULL));

	if(previousVel >= 0)  //if value uninitialized
		return -1.0*(minColorChange + velRange * ((rand() % precision) / (float)precision));
	else
		return (minColorChange + velRange * ((rand() % precision) / (float)precision));
}

void Background::RemoveTooOldEntries(bool Removeall){
	theChildren->Clean();
	theAllActiveChildren->Clean();
	
	//clear the vertical animation list 
	if(Removeall){
		if(AnimationObjList.size()>0){
			for(AnimationObjListIter = AnimationObjList.begin(); AnimationObjListIter != AnimationObjList.end();){
					delete (*AnimationObjListIter);
					AnimationObjListIter = AnimationObjList.erase(AnimationObjListIter);
			}
		}
	FreeAll(AnimationObjList);

	if(flyingAniObjList.size()>0){
			for(flyingAniObjIter = flyingAniObjList.begin(); flyingAniObjIter != flyingAniObjList.end();){
					delete (*flyingAniObjIter);
					flyingAniObjIter = flyingAniObjList.erase(flyingAniObjIter);
			}
		}
	FreeAll(flyingAniObjList);

	}else{
		while(AnimationObjList.size()>0 && AnimationObjList.front()->GetRightMostPoint()<=leftMostXValueToStoreAnimationObj){
			AnimationObjList.front()->Destroy();
			free (AnimationObjList.front());
			AnimationObjList.pop_front();
		}

		while(flyingAniObjList.size()>maxFlyingObj){
			flyingAniObjList.front()->Destroy();
			free (flyingAniObjList.front());
			flyingAniObjList.pop_front();
		}
	}
}