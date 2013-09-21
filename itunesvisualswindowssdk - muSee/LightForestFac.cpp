#include "LightForestFac.h"

void LightForestFac::init(){

}

void  LightForestFac::destroy(){}

AInterface *  LightForestFac::create(void * param){
		AniFacParam * pramas = (AniFacParam *)param;
	
		AStruct theAniStruct, theAniStructFlower;
		//fill the test Astruct
		memset(&theAniStruct,0,sizeof(AStruct));
		memset(&theAniStructFlower,0,sizeof(AStruct));

		theAniStruct.usingTex = true;
		theAniStruct.aControl = pramas->theControl;
		theAniStruct.texInfo = pramas->theControl->GetReader()->GetTextureInfo(1, VerticalAnimationTexture); //reading the square

		theAniStructFlower.usingTex = true;
		theAniStructFlower.aControl = pramas->theControl;;
		theAniStructFlower.texInfo = pramas->theControl->GetReader()->GetTextureInfo(2, AnimationTexture); //reading the flower
		theAniStructFlower.blendColor = (blendColorMode)(int)UniformDistribution(1,11.9);;

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

	return theBranch;
}

