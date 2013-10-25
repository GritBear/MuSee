#include "LightStraw.h"

void LightStraw::init(){
	_totallength = 0;
}

void  LightStraw::destroy(){}

AInterface *  LightStraw::create(void * param){
		AniFacParam * pramas = (AniFacParam *)param;	//cast the object
		_theControl = pramas->theControl;	//copy the control pointer

		//compute the total length
		computeTotoalLength(&pramas->foot, &pramas->head);

		//it only create the root



		//create animation struct and add-on animation structs
		AStruct theAniStruct;

		//fill the test Astruct
		memset(&theAniStruct,0,sizeof(AStruct));

		//define the treebranch variable (later instantiate)
		ATreeBranch * theBranch;
	
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
		theAniStruct.effectTime = 6;//4 seconds from bending
		//theAniStruct.enterTime = 1;
		theAniStruct.aSpeedQ=deque<Vector3>(2);
		theAniStruct.aPositionQ=deque<Vector3>(2);

		//-----------------------------------------------------------
		//initiate non-random but scenario dependent variable
		if (pramas->head.Y < pramas->foot.Y){
			theAniStruct.angle = halfpi; //initial angle
		}
		else{
			theAniStruct.angle = -halfpi; //initial angle
		}

		theAniStruct.damp = UniformDistribution(0.7, 0.99);
		theAniStruct.angIndexOffset = 0;
		theAniStruct.angConstant = GaussianDistribution(0.25, 0.35, 0.45);
		theAniStruct.angOrder = UniformDistribution(1, 4) + 0.5;
		
		theAniStruct.width = 0.08;
		theAniStruct.minWidth = 0.01;
		theAniStruct.widthOrder = 1;
		theAniStruct.length = 0.3;

		//-----------------------------------------------------------
		//assign branch specific value
		theAniStruct.aPositionQ[0] = pramas->foot;
		theAniStruct.aPositionQ[1] = pramas->head;

		//initialize speed
		theAniStruct.aSpeedQ[0].X = 0;
		theAniStruct.aSpeedQ[0].Y = 0;
		theAniStruct.aSpeedQ[0].Z = 0;
		theAniStruct.aSpeedQ[1].X = 0;
		theAniStruct.aSpeedQ[1].Y = 0;
		theAniStruct.aSpeedQ[1].Z = 0;
		

		//generate the treebranch
		theBranch = new ATreeBranch(&theAniStruct);
	
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

	return theBranch;
}

void LightStraw::computeTotoalLength(Vector3 * root, Vector3 * head){
	_totallength = VectorDistance(root, head);
}