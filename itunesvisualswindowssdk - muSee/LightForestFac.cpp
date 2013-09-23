#include "LightForestFac.h"

void LightForestFac::init(){
	_totallength = 0;
}

void  LightForestFac::destroy(){}

AInterface *  LightForestFac::create(void * param){
		AniFacParam * pramas = (AniFacParam *)param;	//cast the object
		_theControl = pramas->theControl;	//copy the control pointer

		//compute the total length
		computeTotoalLength(&pramas->foot, &pramas->head);

		//compute the number of branch
		int numBranch = numberOfBranches();

		//create animation struct and add-on animation structs
		AStruct theAniStruct, theAniStructFlower;

		//fill the test Astruct
		memset(&theAniStruct,0,sizeof(AStruct));
		memset(&theAniStructFlower,0,sizeof(AStruct));

		//define the treebranch variable (later instantiate)
		ATreeBranch * theBranch;
		AObject *theFlower;

		//set common and constant values-----------------------------
		//for the branch
		theAniStruct.usingTex = true;
		theAniStruct.aControl = _theControl;
		theAniStruct.texInfo = _theControl->GetReader()->GetTextureInfo(1, VerticalAnimationTexture); //reading the square
		theAniStruct.effectMode = fadeIn;
		theAniStruct.vertex = 2;
		theAniStruct.theShape = rect_ver;
		theAniStruct.enableAcc = true;
		theAniStruct.straightLine = false;
		theAniStruct.maxAngleSpeed = halfpi / 4;
		theAniStruct.effectTime = 4;//4 seconds from bending
		theAniStruct.enterTime = 1;
		theAniStruct.aSpeedQ=deque<Vector3>(2);
		theAniStruct.aPositionQ=deque<Vector3>(2);

		//for the addon flower
		theAniStructFlower.usingTex = true;
		theAniStructFlower.aControl = _theControl;
		theAniStructFlower.texInfo = _theControl->GetReader()->GetTextureInfo(2, AnimationTexture); //reading the flower
		theAniStructFlower.vertex = 4;
		theAniStructFlower.theShape = rect_ver;
		theAniStructFlower.effectMode = fadeIn;
		theAniStructFlower.enterTime = 0.5;
		theAniStructFlower.aSpeedQ=deque<Vector3>(4);
		theAniStructFlower.aPositionQ=deque<Vector3>(4);
		VectorDQueZero(&theAniStructFlower.aSpeedQ);
		VectorDQueZero(&theAniStructFlower.aPositionQ);
		
		//-----------------------------------------------------------
		//initiate non-random but scenario dependent variable
		//TO DO:


		theAniStruct.angle = halfpi; //initial angle
		theAniStruct.damp = 0.95;
		theAniStruct.angIndexOffset = 0;
		theAniStruct.angConstant = 0.35;
		theAniStruct.angOrder = 3;


		//-----------------------------------------------------------
		//assign branch specific value
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
		
		theAniStructFlower.blendColor = (blendColorMode)(int)UniformDistribution(1,11.9);

		//create flower

		VectorDequeCreateSquare(&theAniStructFlower.aPositionQ, 0.1);

		theAniStruct.width = 0.08;
		theAniStruct.minWidth = 0.01;
		theAniStruct.widthOrder = 1;
		theAniStruct.length = 0.3;

		//generate the treebranch
		theBranch = new ATreeBranch(&theAniStruct);
		theFlower = new AObject(&theAniStructFlower);

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

int LightForestFac::numberOfBranches(){
	// for every 0.1 create a branch
	int meanNumber = max(_totallength * 10,1);

	// gaussian random process
	return (int)(GaussianDistribution(0.5, meanNumber, max(meanNumber *1.5,meanNumber+1), meanNumber*0.3)+0.5);
}

void LightForestFac::computeTotoalLength(Vector3 * root, Vector3 * head){
	_totallength = VectorDistance(root, head);
}