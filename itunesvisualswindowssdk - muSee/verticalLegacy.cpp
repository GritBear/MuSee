#include "Background.h"

void Background::legacyVertical(){
	//if(0){
		VerticalAnimationObj *aAnim = new VerticalAnimationObj(targetAnimationIndex, theControl, selfMotion, preDrawn);
		memset(&aVerticalStruct, 0, sizeof(VerticalAnimationStruct));
		//// Push the generated animation project in line

		aVerticalStruct.AnimationOrientation = animationOrientation;
		aVerticalStruct.CenterBottomX = centerBottomX;
		aVerticalStruct.CenterBottomY = centerBottomY;
		aVerticalStruct.CenterTopX = centerTopX;
		aVerticalStruct.CenterTopY = centerTopY;
		aVerticalStruct.Depth = -defaultVerticalAnimationDistance; //unnecessary
		aVerticalStruct.XVel = xVel;
		aVerticalStruct.YVel = 0;
		aVerticalStruct.StartingTimeInMS = curTimeInMS;
		aVerticalStruct.FrameDelay = time_durarion_frame_delay;

		aAnim->Init(&aVerticalStruct);
		aAnim->SetAlive(true); // make sure alive
		aAnim->SetDepth(-defaultVerticalAnimationDistance);

		AnimationObj * aniPtr = aAnim;
		AnimationObjList.push_back(aniPtr);
		animationIndexAlreadyChose = false;
		//delete(aAnim);

		//Testing: FlyingAnimationTexture Object----------------------------------------------
		/*
		if(flyingAniObjList.size()<maxFlyingObj){
			FlyingAnimationObj *aFlyAnim = new FlyingAnimationObj(targetAnimationIndex, theReader);
			
			AnimationObjStruct aFlyAniStruct = {};
			FlyingObjStruct aFlyingStruct = {};

			memset(&aFlyAniStruct,0,sizeof(AnimationObjStruct));
			memset(&aFlyingStruct, 0 , sizeof(FlyingObjStruct));

			aFlyAniStruct.X = 0.25;
			aFlyAniStruct.Y = 0.95;
			aFlyAniStruct.StartingTimeInMS = curTimeInMS;

			aFlyingStruct.ScreenXVel = 0;
			aFlyingStruct.DiagonalDistance=0.1;
			aFlyingStruct.UseFunctionDraw= true;
			aFlyingStruct.PullFactor = 0.1;
			aFlyingStruct.PullPower = 2;
			aFlyingStruct.useInitialObjPosition = true;
			aFlyingStruct.AnchorSpeed = 0.3;
			aFlyingStruct.DisplayAnchor = false;
			aFlyingStruct.DefineStopState = true;
			//aFlyingStruct.EnableAnchor=false;

			aFlyingStruct.StopDrag = 50;
			aFlyingStruct.StopMaxSpeed= 0.1;
			aFlyingStruct.MinSpeed = 0.001;
			aFlyingStruct.MaxSpeed=0.25;

			aFlyingStruct.BreakSpeedThreshold = -1;
			aFlyingStruct.TailDurationInSecond = 3;
			aFlyingStruct.TailLength = 0.25;

			aFlyAnim->PushAnchorTrace(0,1);
			aFlyAnim->PushAnchorTrace(0,0);
			aFlyAnim->PushAnchorTrace(1,1);
			aFlyAnim->PushAnchorTrace(1,0.5);
			aFlyAnim->PushAnchorTrace(0.5,0.5);

			aFlyAnim->SetAlive(true); // make sure alive
			aFlyAnim->Init(&aFlyAniStruct, &aFlyingStruct);
			aFlyAnim->SetID(flyingAniObjList.size() + 1);
			aFlyAnim->SetDepth(-defaultVerticalAnimationDistance);
			
			AnimationObj * aFlyAniObj = aFlyAnim;
			flyingAniObjList.push_back(aFlyAniObj);
			
			free(&aFlyAniStruct);
			free(&aFlyingStruct);
		}*/
}

bool Background::legacyBestMatchParameters(int AnimationIndex){
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