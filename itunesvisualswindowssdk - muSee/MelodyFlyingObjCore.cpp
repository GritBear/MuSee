#include "MelodyObj.h"

void MelodyObj::SpawnMelodyFollower(int numToSpawn){
	//inScreenTraceX,inScreenTraceY are available information
	for(int i = 0; i < numToSpawn; i++){
		//Testing: FlyingAnimationTexture Object----------------------------------------------
		if(flyingAniObjList.size()<maxMelodyFlyingObj){
			FlyingAnimationObj *aFlyAnim = new FlyingAnimationObj(0, theControl);
			
			memset(&aFlyAniStruct, 0, sizeof(AnimationObjStruct));
			memset(&aFlyingStruct, 0, sizeof(FlyingObjStruct));	

			aFlyAniStruct.StartingTimeInMS = curTimeInMS;

			aFlyingStruct.ScreenXVel = xVel;// / TimeStepInSecond;
			//aFlyingStruct.ScreenYVel = yVel / TimeStepInSecond;

			aFlyingStruct.DiagonalDistance=0.02;
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
	
			//initialize trace and location
			aFlyAniStruct.X = inScreenTraceX[0];
			aFlyAniStruct.Y = inScreenTraceY[0];

			for(unsigned int i = 0; i < inScreenTraceX.size(); i++){
				aFlyAnim->PushAnchorTrace(inScreenTraceX[i],inScreenTraceY[i]);
			}

			aFlyAnim->SetAlive(true); // make sure alive
			aFlyAnim->Init(&aFlyAniStruct, &aFlyingStruct);
			aFlyAnim->SetID(flyingAniObjList.size() + 1);
			//aFlyAnim->SetDepth(-defaultVerticalAnimationDistance);
			aFlyAnim->SetDepth(0);
			
			flyingAniObjList.push_back(aFlyAnim);
		}

	}
}

void MelodyObj::SpawnAniComponent(){

	//if(aniStructSpawnCondition){
	//	
	//	AObject * aAobject = new AObject(&theAniStruct);
	//	/*
	//	aAobject->Update();
	//	*/
	//	delete(aAobject);
	//	
	//}
	//aniStructSpawnCondition = false;
}