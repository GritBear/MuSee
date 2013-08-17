//#pragma once
//#include "MelodyObj.h"
//
////=================================================================
////Core Functions
////=================================================================
//MelodyObj::MelodyObj(control *theControlObj){
//	alive = true;
//	revivable = true;
//	stationary = false;
//	isTheHighestLine = false;
//	animationIndexAlreadyChose = false;
//	aniStructSpawnCondition = false;
//
//	//default
//	usingRegularAniObj = true;
//	usingFlyingAniObj = true;
//
//	//copy control pointer
//	theControl = theControlObj;
//	theReader = theControl->GetReader();
//	theCreationCore = theControl->GetCreationCore();
//	theSpeedControl = theControl->GetSpeedControl();
//
//	previousAngleInRad = NotAnAngle;
//	targetAnimationIndex = 0;
//	specialStatus = 0; //no special status
//	boundY = toneBound;		//distance within 1 tones
//	boundX = timeBound;		//distance within 5 MS
//
//	curToneTimeInMS = NULL;
//
//	memset(&aAniStruct,0,sizeof(AnimationObjStruct));
//	memset(&aFlyAniStruct,0,sizeof(AnimationObjStruct));
//	memset(&aFlyingStruct, 0 , sizeof(FlyingObjStruct));
//
//	//initialize animation default parameters
//	curTimeXPosition = defaultXatCurrentTime;
//	bottomTone = defaultBottomTone;
//	topTone = defaultTopTone;
//	lowerThresholdTone = defaultLowerThresholdToneForStartTone;
//	SpawningThresholdRadian = (float)defaultSpawnAngleThreshold / 180.0 * pi; 
//
//	//reserve vectors
//	probabilityVect.reserve(50);
//	inScreenTraceX.clear();
//	inScreenTraceY.clear();
//
//	x = 0;
//	y = 0;
//	x1 = 0;
//	y1 = 0;
//	x2 = 0;
//	y2 = 0;
//}
//
//void MelodyObj::Destroy(){
//	/*
//	This will cause error
//	free(&aFlyAniStruct);
//	free(&aFlyingStruct);
//	free(&aAniStruct);
//	*/
//
//	delete(theChildren);
//
//	RemoveTooOldEntries(true, true);
//}
//
//void MelodyObj::Init(int startingTone, int actualNumberOfExtractionPulse, unsigned int timeInMS)
//{
//	//not using the pointer to the vector because the probability will change after initial assignment
//	//Set time variables and rate variables
//	pulseRate = kPlayingPulseRateInHz;
//
//	curTimeInMS =
//	previousTimeInMS =
//	startTimeInMS =
//	previousAnimationEndingTimeInMS =
//	endTimeInMS = timeInMS;
//
//
//	previousToneTimeInMS = startTimeInMS;
//	numPulsePerFrame = actualNumberOfExtractionPulse;
//	
//	if(startingTone != NotATone ){
//		melodyLineToneList.push_back(startingTone);
//
//		//start animation generation if a tone is assigned
//		NexAnimationObjGenerate();
//	}
//	//reserve an arbitrary large number
//	curScore.reserve(maxCurTone);
//	time_elapsed = 1000.0/pulseRate + 0.5; //in mili-second
//
//	theChildren = new AniDispatcher();
//	theChildren->SetCleanLim(-2.5, 4, 2.5, -2.5);
//	theChildren->SetActivateMode(Sequential);
//	aniFlipped = false;
//}
//
//void MelodyObj:: Update(unsigned int TimeInMS){
//	//support no time input and update by default pulse rate
//	if(TimeInMS != NULL){
//		curTimeInMS = endTimeInMS = TimeInMS;
//	}else{
//		curTimeInMS += time_elapsed;
//		endTimeInMS += time_elapsed; 
//	}
//	previousTimeInMS = curTimeInMS;
//
//	theChildren->Update();
//	theChildren->Clean();
//
//	//------------------------------------------------------------
//	//Update the static animation object
//	if(aniObjList.size() > 0){
//		for(aniObjListIter = aniObjList.begin(); aniObjListIter != aniObjList.end(); ++aniObjListIter){
//			(*aniObjListIter)->Update();
//		}
//	}
//
//	//------------------------------------------------------------
//	//Update the flying animation object
//	if(flyingAniObjList.size() > 0){
//		for(flyingAniObjIter = flyingAniObjList.begin(); flyingAniObjIter != flyingAniObjList.end(); ++flyingAniObjIter){
//			if((*flyingAniObjIter)->GetAlive() || (*flyingAniObjIter)->GetStationary())
//			(*flyingAniObjIter)->Update();
//		}
//	}
//
//	//------------------------------------------------------------
//	//check if this is a new song
//	if(curTimeInMS < previousAnimationEndingTimeInMS){
//		FreeAll(melodyLineToneList);
//		FreeAll(effectiveMelodyLineToneList);
//		previousAnimationEndingTimeInMS = curTimeInMS;
//	}
//
//	//------------------------------------------------------------
//	//set alive to the next unstationary not alive object
//	if(aniObjList.size() > 0)
//		aniObjList.front()->SetAlive(true); //make sure the first animation is always alive
//
//	//set alive to the next unstationary not alive object
//	if(aniObjList.size() >= 2){
//		RaniObjListIter = aniObjList.rbegin();
//		bool Notfinished = true;
//		while(RaniObjListIter != aniObjList.rend() && Notfinished){
//			if(!(*RaniObjListIter)->GetAlive()){
//				RaniObjListIter++;
//				if((*RaniObjListIter)->GetStationary() || (*RaniObjListIter)->GetRightMostPoint()<0.45){
//					//this is to avoid the non-render caused leaf out of window problem
//					(*RaniObjListIter)->SetStationary(true);
//					RaniObjListIter--;
//					(*RaniObjListIter)->SetAlive(true);
//					Notfinished = false;
//				}
//			}else
//				Notfinished = false;
//		}
//	}
//
//	//Update InScreen Trace Vect---------------------------------------------------------------------
//	for(unsigned int i = 0; i <inScreenTraceX.size();i++){
//		inScreenTraceX[i] += xVel * TimeStepInSecond;
//		inScreenTraceY[i] += yVel * TimeStepInSecond;
//	}
//}
//
//void MelodyObj:: Render(int Screen_w, int Screen_h, float RelativeW, float RelativeY, bool updating){
//	screen_w = Screen_w;
//	screen_h = Screen_h;
//
//	relative_w = RelativeW;
//	relative_h = RelativeY;
//
//	theChildren->Render();
//
//	//render all the alive or stationary animation object that is less than 300 pixel out of the screen
//	if(usingRegularAniObj){
//		if(aniObjList.size() > 0){
//			for(aniObjListIter = aniObjList.begin(); aniObjListIter != aniObjList.end(); ++aniObjListIter){
//				if((*aniObjListIter)->GetAlive() || (*aniObjListIter)->GetStationary())
//					if((*aniObjListIter)->GetToRender())
//						(*aniObjListIter)->Render(Screen_w, Screen_h, updating);//call to render each object
//			}
//		}
//	}
//
//	//render flying obj
//	if(usingFlyingAniObj){
//		if(flyingAniObjList.size() > 0){
//			for(flyingAniObjIter = flyingAniObjList.begin(); flyingAniObjIter != flyingAniObjList.end(); ++flyingAniObjIter){
//				if((*flyingAniObjIter)->GetAlive() || (*flyingAniObjIter)->GetStationary())
//					if((*flyingAniObjIter)->GetToRender())
//						(*flyingAniObjIter)->Render(Screen_w, Screen_h, updating);//call to render each object
//			}
//		}
//	}
//
//}
//
////aToneBufferVect is from the lowest to the highest
//vector<float> MelodyObj::AnalyseMelodyTones(vector<int> *aToneBufferVect){
//	int bufferOrder = (*aToneBufferVect).size();
//	vector<float> score((*aToneBufferVect).size());
//	
//	/*push backs here to return score
//	 finding best trellis finding given the buffer
//	 Analyze several frames but only return scores of one frame
//	*/
//	//assuming vectors are passed in from low to high
//	for(int j = 0; j < score.size(); j++){
//		score[j] += EvaluateToneAcceptanceScore((*aToneBufferVect)[j], score.size() - j);
//	}
//	//make a copy to the storage element (may not be the best implementation)
//	curScore = score;
//	return score;
//}
//
//void MelodyObj::EffectiveToneRegistration(int tone, int rank){
//	/*Available info: 	list<int> melodyLineToneList;
//						list<float> effectiveMelodyLineToneList; 
//					1. Melody Slope Implementation works poorly (very unstable)
//					2. Adaptive Smoothing Filtering (works well)
//	*/
//
//	int size_effectiveMelody = effectiveMelodyLineToneList.size();
//	if(size_effectiveMelody <= minimumBuffer){
//		//store the initial tones directly
//		effectiveMelodyLineToneList.push_back((float)tone);
//
//	}else{
//		/*The Effective Melody Rule:
//			Using adaptive order smoothing: find the order of smoothing that best matches the tone, at least 2 tone smoothing
//		*/
//		
//		// the smoothing filter best matching
//		float bestDeviation = 99999999; // an arbitary big initial value
//		float bestSmoothedValue = tone;
//
//		//sums up values to be smoothed
//		list<float>::iterator anInterator = effectiveMelodyLineToneList.end();
//		float sumValue = 0;
//		//adaptive smoothing loop
//		for(unsigned int i = 0; i < maxSmoothingOrder; i++){
//			--anInterator;
//			sumValue +=	(*anInterator);
//			
//			if(i >= minSmoothingOrder - 1){
//				float sumValueTemp = sumValue + tone;
//				float smoothedValue = sumValueTemp/(i+2);
//				float deviation = abs(tone - smoothedValue);
//			
//				// compare and find the best so far
//				if(deviation<bestDeviation){
//					bestDeviation = deviation;
//					bestSmoothedValue = smoothedValue;
//				}
//			}
//		}
//
//		//store the smoothed value
//		effectiveMelodyLineToneList.push_back(bestSmoothedValue);	
//	}
//}
//
//void MelodyObj::NexAnimationObjGenerate(){
//	if(!animationIndexAlreadyChose){
//		//-----------------------------------------------------------------------
//		// Probablistic Next Animation Index Draw
//		//-----------------------------------------------------------------------
//		//random animation object index generation
//		theCreationCore->NextAnimation(targetAnimationIndex, probabilityVect);
//		animationIndexAlreadyChose = true;
//	}
//
//	//-----------------------------------------------------------------------
//	//Calculate necessary condition and spawn the animation object
//	//-----------------------------------------------------------------------
//	bool spawndingConditionGood = false;
//	if(effectiveMelodyLineToneList.size() > 1){
//		spawndingConditionGood = BestMatchParameters(targetAnimationIndex);
//	}
//
//	if(spawndingConditionGood){
//	//if(0){
//		//AnimationObj *aAnim = new AnimationObj(targetAnimationIndex, theControlObj);
//		AnimationObj *aAnim = new AnimationObj(targetAnimationIndex, theControl);
//		memset(&aAniStruct,0,sizeof(AnimationObjStruct));
//
//		aAniStruct.AnimationOrientation = animationOrientation;
//		aAniStruct.X = x;
//		aAniStruct.Y = y;
//
//		aAniStruct.BoundX = aniXBound;
//		aAniStruct.BoundY = aniYBound;
//		aAniStruct.X1 = x1;
//		aAniStruct.X2 = x2;
//		aAniStruct.Y1 = y1;
//		aAniStruct.Y2 = y2;
//		aAniStruct.XVel = xVel;
//		aAniStruct.YVel = yVel;
//		aAniStruct.StartingTimeInMS = curTimeInMS;
//		aAniStruct.FrameDelay = time_durarion_frame_delay;
//
//		// Push the generated animation project in line
//		aAnim->Init(&aAniStruct);
//		aAnim->SetAlive(false);
//		
//		aniObjList.push_back(aAnim);
//		animationIndexAlreadyChose = false;
//		
//		inScreenTraceX.push_back(aAnim->GetX());
//		inScreenTraceY.push_back(aAnim->GetY());
//
//		//SpawnMelodyFollower(1);
//		//SpawnAniComponent();
//
//		//--------------------------------------------------------------------------------------
//		//--------------------------------------------------------------------------------------
//
//		memset(&theAniStruct, 0, sizeof(AStruct));
//
//		theAniStruct.usingTex = true;
//
//		theAniStruct.aControl = theControl;
//		theAniStruct.vertex = 4;
//
//		theAniStruct.theShape = rect_dia;
//
//		theAniStruct.texInfo = theReader->GetTextureInfo(0, AnimationTexture);
//		aniStructSpawnCondition =  RectDiaRadConstraintSpawning(x1, y1, x2, y2, MelodyAnimationObjSpeed(), 0, previousAngleInRad, aniFlipped, theAniStruct);
//		
//		if(aniStructSpawnCondition){
//			AObject * aAobject = new AObject(&theAniStruct);
//			theChildren->Insert((AInterface*) aAobject);
//
//			FreeAll(theAniStruct.aPositionQ);
//			FreeAll(theAniStruct.aSpeedQ);
//
//			aniFlipped = !aniFlipped;
//		}
//	}
//}
//
////=================================================================
////Analysis methods
////=================================================================
//bool MelodyObj::BestMatchParameters(int AnimationIndex){
//	//---------------------------------------------------------------------------------------
//	//preliminary test
//	//check if the start point is too low
//	if(PreviousAniEndTone <= lowerThresholdTone){
//		PreviousAniEndTone = effectiveMelodyLineToneList.back();
//		return false;
//	}
//
//	float minDiaLength; 
//	float maxDiaLength;
//	float maxAngleChangeInRad;
//
//	int maxTime;
//	int minTime;
//	float diagonalLength;
//	int TimeElapsed = curTimeInMS - previousAnimationEndingTimeInMS;
//
//	theReader->GetDiagonalLength(AnimationIndex, minDiaLength, maxDiaLength, minTime, maxTime, maxAngleChangeInRad);
//	
//	if(TimeElapsed < 0){
//		previousAnimationEndingTimeInMS = curTimeInMS;
//		return false;
//	}
//
//	//check if the time interval is tooo huge
//	if( abs((int)(TimeElapsed)) > 2*maxTime){
//		previousAnimationEndingTimeInMS = curTimeInMS;
//		return false;
//	}
//
//	//or time is too short
//	if(TimeElapsed < minTime){
//		return false;
//	}
//
//	if(effectiveMelodyLineToneList.size() < minimumBuffer){
//		return false;
//	}
//
//	//---------------------------------------------------------------------------------------
//	//calculate diagonal length and x,y coordinates
//	x1 = curTimeXPosition - (float)(TimeElapsed)/ (float)defaultDisplayTimeInMS;
//	float toneSpan = (topTone - bottomTone);
//	if(aniObjList.size() == 0){
//		y1 = (effectiveMelodyLineToneList.front() - bottomTone)/toneSpan;
//		PreviousAniEndTone = effectiveMelodyLineToneList.front();
//	}else{
//		//x1 = aniObjList.back()->GetRightMostPoint();
//		y1 = previousY2;
//	}
//
//	x2 = curTimeXPosition;
//	y2 = (effectiveMelodyLineToneList.back() - bottomTone)/toneSpan;
//
//	//---------------------------------------------------------------------------------------
//	//Test Angle Here
//	
//	float angle = atan((y2 - y1)/(x2 - x1)); //useful later in upward downward detection
//	
//	if(!(previousAngleInRad == NotAnAngle || maxAngleChangeInRad == NotAnAngle)){
//		if(abs(angle - previousAngleInRad) > maxAngleChangeInRad){
//			float angleDifference = (angle - previousAngleInRad < 0) ? -maxAngleChangeInRad : maxAngleChangeInRad;
//			float correctAngle = previousAngleInRad + angleDifference;
//
//			float yAdjust = tan(correctAngle) * (x2 - x1);
//			y2 =  y1 + yAdjust;
//		}
//	}
//	//---------------------------------------------------------------------------------------
//
//	//---------------------------------------------------------------------------------------
//	//test diagonal length
//	diagonalLength = sqrt(pow((x2-x1), 2) + pow((y2-y1), 2));
//	
//	//check if the minimum length constrain is met
//	if(diagonalLength < minDiaLength){
//		return false;
//	}
//	
//	if((diagonalLength >= minDiaLength) && (diagonalLength <= maxDiaLength)){
//		//Set Velocity
//		xVel = MelodyAnimationObjSpeed();
//		yVel = 0;
//
//		//set the bound (now using default 0, leave for later implementation)
//		aniXBound = 0;
//		aniYBound = 0;
//
//		//set the animation orientation
//		animationOrientation = (previous_animation_orientation + 1) % theReader->GetMaxAniOrientation(AnimationIndex);
//		previous_animation_orientation = animationOrientation;
//		
//		previousAnimationEndingTimeInMS = curTimeInMS;
//		previousY2 = y2;
//		previousAngleInRad =  atan((y2 - y1)/(x2 - x1));
//		SetFrameDelay(AnimationIndex,TimeElapsed);
//
//		return true;
//	}else if(diagonalLength > maxDiaLength){
//
//		float resizeScale = (float)diagonalLength/(float)maxDiaLength;
//		float x_Offset = x2 - x1;
//		float y_Offset = y2 - y1;
//
//		x_Offset = x_Offset / resizeScale;
//		y_Offset = y_Offset / resizeScale;
//
//		x2 = x1 + x_Offset;
//		y2 = y1 + y_Offset;
//
//		float timeOffset = curTimeInMS - previousAnimationEndingTimeInMS;
//
//		timeOffset = timeOffset/resizeScale;
//
//		previousAnimationEndingTimeInMS += timeOffset;
//		//--------------------------------------------------------------------
//		//Set other varaibles
//		//Set Velocity
//		xVel = MelodyAnimationObjSpeed();
//		yVel = 0;
//
//		//set the bound (now using default 0, leave for later implementation)
//		aniXBound = 0;
//		aniYBound = 0;
//
//		//set the animation orientation
//		animationOrientation = (previous_animation_orientation + 1) % theReader->GetMaxAniOrientation(AnimationIndex);
//		previous_animation_orientation = animationOrientation;
//		previousY2 = y2;
//		previousAngleInRad =  atan((y2 - y1)/(x2 - x1));
//		SetFrameDelay(AnimationIndex, TimeElapsed);
//		
//		return true;
//	}else
//		return false;
//}
//
////Melodies can colide with each other: Revise Needed
//bool MelodyObj:: CheckMelodyTouching(MelodyObj *otherMelody){
//	float oX = otherMelody->GetAnimationSpawningX();
//	float oY = otherMelody->GetAnimationSpawningY();
//
//	int obX = otherMelody->GetMelodyBoundX();
//	int obY = otherMelody->GetMelodyBoundY();
//
//	if( x + boundX > oX - obX &&
//		x - boundX < oX + obX &&
//		y + boundY > oY - obY &&
//		y - boundY < oY + obY)
//		return true;
//	else
//		return false;
//}
//
////set frame Delay according to constraints and time duration
//void MelodyObj::SetFrameDelay(int AnimationIndex, int TimeDurationInMS){
//	//set frame Delay
//	int minDelay, maxDelay, maxFrame, delay;
//	theReader->GetAniMinMaxFrameDelay(AnimationIndex, minDelay, maxDelay);
//	maxFrame = theReader->GetMaxFrame(AnimationIndex);
//	
//	delay =(int)(((float)TimeDurationInMS) / ((float)maxFrame) + 0.5);
//
//	if(TimeDurationInMS == 0 || delay <= 0){
//		time_durarion_frame_delay = minDelay;
//	}else if(delay > 0 && delay <= maxDelay){
//		time_durarion_frame_delay = delay;
//	}else{
//		time_durarion_frame_delay = maxDelay;
//	}
//}
//
////------------------------------------------------
////Evaluate the likelyhood score
////tone_rank = 0 means the tone is the highest, 1 means second highest and so on
////should implementing the adaptive smoothing filter algorithm
////------------------------------------------------
//float MelodyObj::EvaluateToneAcceptanceScore(int tone, int tone_rank){
//	float score = startingOffset;
//	UpdateMelodyVector();
//	
//	/* Evaluation based on:
//		1: if it the The Highest Tone
//		2: how close the tone is to its previous one
//		3: how good it fits with the trend of past N tone
//	*/
//	// 1st step
//	if(isTheHighestLine && tone_rank == 0)
//		score += isHighestOffset;
//	else if(isTheHighestLine && tone_rank!=0)
//		score += negativeHighestNotPickedScale * tone_rank;
//
//	// 2ed step
//	score += ToneClosenessEvaluation(tone);
//
//	// 3rd step
//	score += TrendFittingEvaluation(tone, regressionOrder);
//
//	return score;
//}
//
///*return a score change value by how far the new tone is to the previous one*/
//float MelodyObj::ToneClosenessEvaluation(int tone){
//	float scorechange = 0;
//	int tone_diff = abs(tone - melodyLineToneList.back());
//	//using a linear negative scale
//	scorechange = tone_diff * negativeClosenessScale;
//	return scorechange;
//}
//
///*using 2 points regression
//	1st point is calculated by the average of all the points at the middle on x-axis
//	2nd point is calculated by the average of the last and second to last points and put at correponding x location 
//	A line was draw and the deviation from this trend is recorded
//*/
//float MelodyObj::TrendFittingEvaluation(int tone, int order){
//	float scorechange = 0;
//	int size_melodyVect = melodyLineToneVect.size();
//	//the order has to be greater or equal to two
//	if(order >= 2 && size_melodyVect>=2){
//		//first find 2 points
//		float point1 = 0;
//		for(int i = 1; i <= order; i++)
//			point1 += melodyLineToneVect[size_melodyVect - i];
//		point1 = point1/order;
//		float x1 = order/2;
//
//		float point2 = (melodyLineToneVect[size_melodyVect - 1] +  melodyLineToneVect[size_melodyVect - 2]) /2;
//		float x2 = order - 0.5;
//
//		//find the target and calculate score change
//		float target = point2 + 1.5*((point2 - point1)/(x2 - x1));
//		scorechange = (tone - target) * negativeTrendScale;
//
//	}
//	return scorechange;
//}
//
////=================================================================
////Helper methods
////=================================================================
//void MelodyObj::UpdateMelodyVector(){
//	if(melodyLineToneList.size()>0){
//		vector<int> amelodyLineToneVect(melodyLineToneList.size(), 0);
//		list<int>::iterator toneListIterator;
//		int vectorIter = 0;
//		for(toneListIterator = melodyLineToneList.begin(); toneListIterator != melodyLineToneList.end(); ++toneListIterator){
//			amelodyLineToneVect[vectorIter] = (*toneListIterator);
//			vectorIter++;
//		}		
//		melodyLineToneVect = amelodyLineToneVect;
//	}
//}
//
////------------------------------------------------
////Assigning Next Tone to this melody line
//// tone_rank = 0 means the tone is the highest, 1 means second highest and so on
////------------------------------------------------
//void MelodyObj:: AcceptTone(int tone, int rank, unsigned time){
//	// first update the times
//	if(curToneTimeInMS != NULL){
//		previousToneTimeInMS = curToneTimeInMS;
//		curToneTimeInMS = time;
//	}else{
//		curToneTimeInMS = time;
//	}
//	
//	melodyRank = rank;
//	melodyLineToneList.push_back(tone);
//	EffectiveToneRegistration(tone, rank);
//
//	NexAnimationObjGenerate();
//
//	RemoveTooOldEntries();
//}
//
////------------------------------------------------
////Remove Too Old Entries
////------------------------------------------------
//void MelodyObj::RemoveTooOldEntries(bool ChangeTrack, bool Removeall){
//	if(ChangeTrack){
//		FreeAll(melodyLineToneList);
//		FreeAll(effectiveMelodyLineToneList);
//	}
//	else if(Removeall){//remove everything
//		FreeAll(melodyLineToneList);
//		FreeAll(effectiveMelodyLineToneList);
//
//		if(aniObjList.size() > 0){
//			for(aniObjListIter = aniObjList.begin(); aniObjListIter != aniObjList.end();){
//				(*aniObjListIter)->Destroy();//call to destroy each object
//				delete (*aniObjListIter);
//				aniObjListIter = aniObjList.erase(aniObjListIter);
//			}
//		}
//		FreeAll(aniObjList);
//
//		if(flyingAniObjList.size() > 0){
//			for(flyingAniObjIter = flyingAniObjList.begin(); flyingAniObjIter != flyingAniObjList.end();){
//				delete (*flyingAniObjIter);
//				flyingAniObjIter = flyingAniObjList.erase(flyingAniObjIter);
//			}
//		}
//		FreeAll(flyingAniObjList);
//		FreeAll(inScreenTraceX);
//		FreeAll(inScreenTraceY);
//	}else{
//		while(melodyLineToneList.size()>maxExtractedStorageEntries){
//			melodyLineToneList.pop_front();
//		}
//
//		while(effectiveMelodyLineToneList.size()>maxExtractedStorageEntries){
//			effectiveMelodyLineToneList.pop_front();
//		}
//		
//		while(aniObjList.size()>0 && aniObjList.front()->GetRightMostPoint()<=leftMostXValueToStoreAnimationObj){
//			delete (aniObjList.front());
//			aniObjList.pop_front();
//		}
//
//		//use if instead of while
//		if(flyingAniObjList.size() > maxMelodyFlyingObj){
//			for(flyingAniObjIter = flyingAniObjList.begin(); flyingAniObjIter != flyingAniObjList.end();){
//				if((*flyingAniObjIter)->GetAlive() || (*flyingAniObjIter)->GetX() <= leftMostXValueToStoreAnimationObj ||(*flyingAniObjIter)->GetY() > 3 || (*flyingAniObjIter)->GetY() < -1 ){
//					delete (*flyingAniObjIter);
//					flyingAniObjIter = flyingAniObjList.erase(flyingAniObjIter);
//				}
//			}
//		}
//
//		while(inScreenTraceX.size()>flyingAniObjList.size()){
//			inScreenTraceX.pop_front();
//			inScreenTraceY.pop_front();
//		}
//	}
//}