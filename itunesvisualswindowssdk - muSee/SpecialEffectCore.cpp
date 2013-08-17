#include "SpecialEffectCore.h"

void SpecialEffectObj::Init(SpecialEffectStruct *aSpecialEffectStruct){
	theReader = aSpecialEffectStruct->theControlObj->theReader;
	theCreationCore = aSpecialEffectStruct->theControlObj->theCreationCore;
	theSpeedControl = aSpecialEffectStruct->theControlObj->theSpeedControl;
	theControlObj = aSpecialEffectStruct->theControlObj;


	collidable = aSpecialEffectStruct->collidable;
	bounceBack = aSpecialEffectStruct->bounceBack;
	initialGeneration = aSpecialEffectStruct->initialGeneration;
	
	useTexture = aSpecialEffectStruct->useTexture;
	useVaryingDepth = aSpecialEffectStruct->useVaryingDepth;
	
	themeIndex = aSpecialEffectStruct->themeIndex;
	minWidth = aSpecialEffectStruct->minWidth;
	meanWidth = aSpecialEffectStruct->meanWidth;
	maxWidth = aSpecialEffectStruct->maxWidth;
	widthStandardDeviation = aSpecialEffectStruct->widthStandardDeviation;

	minNumObj = aSpecialEffectStruct->minNumObj;
	maxNumObj = aSpecialEffectStruct->maxNumObj;
	minTime = aSpecialEffectStruct->minTime;
	meanTime = aSpecialEffectStruct->meanTime;
	maxTime = aSpecialEffectStruct->maxTime;
	timeStandardDeviation = aSpecialEffectStruct->timeStandardDeviation;

	minLifeTime = aSpecialEffectStruct->minLifeTime;
	meanLifeTime = aSpecialEffectStruct->meanLifeTime;
	maxLifeTime = aSpecialEffectStruct->maxLifeTime;
	liftTimeStandardDeviation = aSpecialEffectStruct->liftTimeStandardDeviation;
	
	depth = aSpecialEffectStruct->depth;
	if(useVaryingDepth){
		minDepth = aSpecialEffectStruct->minDepth;
		meanDepth = aSpecialEffectStruct->meanDepth;
		maxDepth = aSpecialEffectStruct->maxDepth;
		depthStandardDeviation = aSpecialEffectStruct->depthStandardDeviation;
		depth =  GaussianDistribution(minDepth, meanDepth, maxDepth, depthStandardDeviation);
	}

	//copy speed
	minSpeed = aSpecialEffectStruct->minSpeed;
	meanSpeed = aSpecialEffectStruct->meanSpeed;
	maxSpeed = aSpecialEffectStruct->maxSpeed;
	speedDeviation = aSpecialEffectStruct->speedDeviation;

	animationIndexAlreadyChose = false;
	nextAnimationIndex = 0;
	timeSinceLast = 0;
	timeNextObj = GaussianDistribution(minTime, meanTime, maxTime, timeStandardDeviation);

	theAniStruct = (AnimationObjStruct*)malloc(sizeof(AnimationObjStruct));
	
}

void SpecialEffectObj::Destroy(){
	free(theAniStruct);
	RemoveTooOldEntries(true);
}

void SpecialEffectObj::Update(EventMessage *anEvent){
	curTimeInMS += TimeStep;
	NextObjGeneration();
	timeSinceLast += TimeStep;

	/*
	if(anEvent != NULL){
		anEventUpdate = true;
		theEvent = (*anEvent);
	}else
		anEventUpdate = false;
	*/

	MovementCenter();
	
	//------------------------------------------------------------
	//Update the static animation object
	//update all the verticalAnimation Objects
	/*
	if(AnimationObjList.size()>0){
		for(AnimationObjListIter = AnimationObjList.begin(); AnimationObjListIter != AnimationObjList.end(); ++AnimationObjListIter){
				(*AnimationObjListIter)->Update();//call to render each object
		}
	}*/
} 

void SpecialEffectObj::Render(float Screen_w, float Screen_h, bool Updating){
	//update screen dimension if provided
	screen_w = Screen_w;
	screen_h = Screen_h;

	bottom = 0.5 - screen_h/2.0;
	top = bottom + screen_h;
	left = 0.5 - screen_w/2.0;
	right = left + screen_w;

	timeSinceLast += TimeStepInSecond;
	//render the verticalAnimation Objects
	
	/*if(AnimationObjList.size()>0){
		for(AnimationObjListIter = AnimationObjList.begin(); AnimationObjListIter != AnimationObjList.end(); ++AnimationObjListIter){
			if((*AnimationObjListIter)->GetToRender())
				(*AnimationObjListIter)->Render(Screen_w, Screen_h, Updating);//call to render each object
		}
	}*/
}

//--------Special Effect Position Modules-----------------------
bool SpecialEffectObj::TimeControl(){
	if(timeSinceLast<timeNextObj) return false;
	//else
	timeSinceLast = 0;
	timeNextObj = GaussianDistribution(minTime, meanTime, maxTime, timeStandardDeviation);
	return true;
}

//--------------------------------------------------------------
void SpecialEffectObj::RemoveTooOldEntries(bool Removeall){}