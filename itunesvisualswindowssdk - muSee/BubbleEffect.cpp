#include "BubbleEffect.h"

BubbleEffect::BubbleEffect(BubbleEffectStruct * aBubbleStruct){
	if(aBubbleStruct->useDefaultParameters){//all values are redefined
		//initialize all the parameters here 
		(*aBubbleStruct).aSpecialEffectStruct->maxNumObj = 6; 
		(*aBubbleStruct).aSpecialEffectStruct->minNumObj = 3;

		//Random Size Generation Parameters
		(*aBubbleStruct).aSpecialEffectStruct->minWidth = 0.05;
		(*aBubbleStruct).aSpecialEffectStruct->meanWidth = 0.1; 
		(*aBubbleStruct).aSpecialEffectStruct->maxWidth = 0.15;
		(*aBubbleStruct).aSpecialEffectStruct->widthStandardDeviation = 0.06;

		//Random Time Generation Parameters
		(*aBubbleStruct).aSpecialEffectStruct->minTime = 0.5;				//in seconds
		(*aBubbleStruct).aSpecialEffectStruct->maxTime = 10;
		(*aBubbleStruct).aSpecialEffectStruct->meanTime = 5;
		(*aBubbleStruct).aSpecialEffectStruct->timeStandardDeviation = 6;

		(*aBubbleStruct).aSpecialEffectStruct->minLifeTime = 3;
		(*aBubbleStruct).aSpecialEffectStruct->meanLifeTime = 15;
		(*aBubbleStruct).aSpecialEffectStruct->maxLifeTime = 40;
		(*aBubbleStruct).aSpecialEffectStruct->liftTimeStandardDeviation = 10;

		//speed
		(*aBubbleStruct).aSpecialEffectStruct->minSpeed = 0.04;
		(*aBubbleStruct).aSpecialEffectStruct->meanSpeed = 0.7;
		(*aBubbleStruct).aSpecialEffectStruct->maxSpeed = 0.12;
		(*aBubbleStruct).aSpecialEffectStruct->speedDeviation = 0.5;

		(*aBubbleStruct).minRotation = 0;
		(*aBubbleStruct).meanRotation = 0.1; //radian per second
		(*aBubbleStruct).maxRotation = 0.2;
		(*aBubbleStruct).RotationDeviation = 0.1;

		//bool switches
		(*aBubbleStruct).aSpecialEffectStruct->collidable = false;
		(*aBubbleStruct).aSpecialEffectStruct->bounceBack = true;
		(*aBubbleStruct).aSpecialEffectStruct->initialGeneration = true;
	
		(*aBubbleStruct).aSpecialEffectStruct->useTexture = true;
		(*aBubbleStruct).aSpecialEffectStruct->useVaryingDepth = false;
	
		(*aBubbleStruct).aSpecialEffectStruct->themeIndex = 1;
		depth = -farthestVisibleDistance * 0.7; 
	}

	minRotation = (*aBubbleStruct).minRotation;
	meanRotation = (*aBubbleStruct).meanRotation;
	maxRotation = (*aBubbleStruct).maxRotation;
	RotationDeviation = (*aBubbleStruct).RotationDeviation;

	SpecialEffectObj::Init(aBubbleStruct->aSpecialEffectStruct);
	if(collidable)
		theCollisionCore = new AnimationObjCollisionEventCore(&AnimationObjList);
	else
		theCollisionCore = new AnimationObjCollisionEventCore();
};

BubbleEffect::~BubbleEffect(){
	delete (theCollisionCore);
	SpecialEffectObj::Destroy();
}

void BubbleEffect::Update(){
	SpecialEffectObj::Update();
}

void BubbleEffect::Render(float Screen_w, float Screen_h, bool Updating){
	SpecialEffectObj::Render(Screen_w, Screen_h, Updating);
}

void BubbleEffect::NextObjGeneration(){
	//only execute when the number of objects are smaller than maxNumObj
	//Prepare to migrate to superclass
	//need to prepare nextAnimationIndex and animationIndexAlreadyChose
	if(!animationIndexAlreadyChose){
		//-----------------------------------------------------------------------
		// Probablistic Next Animation Index Draw
		//-----------------------------------------------------------------------
		AnimationIndex();
	}

	if(AnimationObjList.size() <= maxNumObj){
		if(AnimationObjList.size() <= minNumObj){
			if(TimeControl()){
				BestMatchParameters();
				CollidableBubble *aAnim = new CollidableBubble(nextAnimationIndex, theControlObj);
				aAnim->Init(theAniStruct);
				aAnim->SetAlive(false);
				AnimationObjList.push_back(aAnim);

				free(theAniStruct);
				animationIndexAlreadyChose = false;
			}
		}
	}
}

void BubbleEffect::BestMatchParameters(){
	/* generation logics
		1. bubble should be generated outside of the viewport
		2. bubble should die outside of the window	
	*/
	AnimationObjStruct aAniStruct = {};
	memset(&aAniStruct,0,sizeof(aAniStruct));
	//generate the bubble
	/*roles: 
		1. spawning outside of the view port
		2. flying towards an area around center
		3. once entered the boundary, it will remain inside and until kill command is issued
	*/

	//Generate the radius of the obj
	aAniStruct.Radius = GaussianDistribution(minWidth, meanWidth, maxWidth, widthStandardDeviation);

	//Generate spawing X,Y location
	//first select out of which bound do we spawn the obj
	switch(rand() % 4){
	case 0: //bottom
		aAniStruct.Y = bottom - aAniStruct.Radius - 0.05;
		aAniStruct.X = UniformDistribution(left, right);
		break;
	case 1:
		aAniStruct.Y = top + aAniStruct.Radius + 0.05;
		aAniStruct.X = UniformDistribution(left, right);
		break;
	case 2:
		aAniStruct.X = left - aAniStruct.Radius - 0.05;
		aAniStruct.Y = UniformDistribution(bottom, top);
		break;
	case 3:
	default:
		aAniStruct.X = right + aAniStruct.Radius + 0.05;
		aAniStruct.Y = UniformDistribution(bottom, top);
	}

	//Generate first targeting X,Y location
	float centerDeviation = 0.12;
	float targetX = UniformDistribution(0.5  - centerDeviation, 0.5  + centerDeviation);
	float targetY = UniformDistribution(0.5  - centerDeviation, 0.5  + centerDeviation);

	//Generate Speed
	float speed = GaussianDistribution(minSpeed, meanSpeed, maxSpeed, speedDeviation); //per second

	//compute velocity
	float timeToTarget = sqrt(pow(targetX - aAniStruct.X,2) + pow(targetY - aAniStruct.Y,2))/speed;

	aAniStruct.XVel = (targetX - aAniStruct.X) / timeToTarget;
	aAniStruct.YVel = (targetY - aAniStruct.Y) / timeToTarget;

	//compute rotation
	aAniStruct.Rotation = GaussianDistribution(minRotation, meanRotation, maxRotation, RotationDeviation);
	
	//compute square size (maybe not important)
	float initialRotation = UniformDistribution(0, 2*pi);
	aAniStruct.X1 = aAniStruct.X - aAniStruct.Radius;
	aAniStruct.Y1 = aAniStruct.Y - aAniStruct.Radius;
	aAniStruct.X2 = aAniStruct.X + aAniStruct.Radius;
	aAniStruct.Y2 = aAniStruct.Y + aAniStruct.Radius;

	PointRotation(aAniStruct.X1, aAniStruct.Y1, aAniStruct.X,aAniStruct.Y, initialRotation);
	PointRotation(aAniStruct.X2, aAniStruct.Y2, aAniStruct.X,aAniStruct.Y, initialRotation);
}

void BubbleEffect::AnimationIndex(){
	//produce nextAnimationIndex
	nextAnimationIndex = 0;
	animationIndexAlreadyChose = true;
}

void BubbleEffect::MovementCenter(){}
void BubbleEffect::EventMove(){}


//Garbage Collection
void BubbleEffect::RemoveTooOldEntries(bool Removeall){
	if(Removeall){
	
	
	}else{
	
	
	}
}

