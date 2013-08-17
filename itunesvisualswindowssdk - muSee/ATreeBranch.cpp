#include "ATreeBranch.h"

ATreeBranch::ATreeBranch(AStruct * aStruct){
	AObject::Init(aStruct);
	AObject::InitSubComponent( aStruct);
	ATreeBranch::Init(aStruct);
}

void ATreeBranch::Init(AStruct * aStruct){
	//copy effect element
	effectTime = aStruct->effectTime;
	effectState = (effectTime>0) ? Enter : NoState;
	
	//copy basic element
	lineCopy = (*aStruct);
	lineSegL.clear();
	branchChild.clear();
	deltaAngle = 0;
	angleSpeed = 0;
	angleUpdateStage = 0;

	straightline = aStruct->straightLine;
	enableAngleAcc = aStruct->enableAcc;

	previousDiff = 1000; // arbitrary large number

	indexOffset = aStruct->angIndexOffset;
	angleConst = aStruct->angConstant;
	AngOrder = aStruct->angOrder;
	dampingFactor = aStruct->damp;
	//set constraints
	if(aStruct->maxAngleSpeed > 0)
		maxAngleSpeed = aStruct->maxAngleSpeed;
	else
		maxAngleSpeed = INF;

	fluctuationCnt = 0;
	
	WOrder = aStruct->widthOrder;
	minwidth = aStruct->minWidth;

	//copy special values
	length = aStruct->length;
	vertex = (int)(length * numSegPerUnitLength + 0.5); //number of segments

	//copy out pointers
	branchRootStart = &(*position)[0];	
	treeSpeed = &(*speed)[0];

	targetP = &(*position)[1];
	targetspeed = &(*speed)[1];

	rootAngle = &(theMotion->angle);
	rootWidth = aStruct->width;

	widthDiff = rootWidth - minwidth;

	theChildren->SetCleanLim(-2.5, 4, 2.5, -2.5);
	theChildren->SetActivateMode(Sequential);

	SpawnSegments();
}

void ATreeBranch::SpawnSegments(){
	ALineSeg* previous = nullptr;
	segL = segLength; //assign constant

	//some piecewise processing
	lineCopy.vertex = 4;
	lineCopy.length = segL;
	lineCopy.theShape = rect_ver;
	lineCopy.angle = (*rootAngle);
	lineCopy.enterTime = lineCopy.enterTime/vertex;
	lineCopy.exitTime = lineCopy.exitTime/vertex;
	lineCopy.aPositionQ = deque<Vector3>(4); 
	lineCopy.aSpeedQ = deque<Vector3>(4); 

	//spawn all line segments
	for(int i = 0; i < vertex; i++){
		//compute the width
		lineCopy.width = minwidth + widthDiff * pow((float(vertex-i))/vertex,WOrder);

		//spawn line segments
		ALineSeg * thisSeg = new ALineSeg(&lineCopy, i, previous, branchRootStart);
		thisSeg->SetDepth((*position)[0].Z);
		//push into list
		lineSegL.push_back(thisSeg);
		theChildren->Insert((AInterface*)thisSeg);
		previous = thisSeg;
	}

	//get the spawned top vertex
	branchTop = lineSegL.back()->GetCurrentEnd();
	rootDiff = (*branchRootStart);
	VectorSub(&rootDiff, targetP);
	compareRootY = (rootDiff.Y <= 0)? 1:-1;
}

void ATreeBranch::Update(){
	AObject::Update();

	//update the target
	float timeElapsed = theSpeedControl->GetTimeElapsed();

	//update the angles
	if(!straightline && effectState!= NoState){
		UpdateAngleDrive();
		effectTime -= theSpeedControl->GetTimeElapsed();
		effectState = (effectTime>0) ? Enter : NoState;
	}
}

void ATreeBranch::Render(){
	if(toRender){
		//theRender->Render(); save for later implementing effects
		theChildren->Render();
	}
}

void ATreeBranch::CreateBranch(AStruct * aStruct, float PercentBranch, CoorPosition theAnchored, float depthChange){
	ATreeBranch* TheNextAni = new ATreeBranch(aStruct);
	branchChild.push_back(TheNextAni);
	InsertAnimation(TheNextAni, PercentBranch, theAnchored, depthChange);
}

void ATreeBranch::CreateEndBranch(AStruct * aStruct, float PercentBranch, CoorPosition theAnchored, float depthChange){
	ATreeBranch *thisBranch = this;
	while(thisBranch->branchChild.size()>0){
		thisBranch = branchChild.back();
	}
	thisBranch->CreateBranch(aStruct, PercentBranch, theAnchored, depthChange);
}

void ATreeBranch::InsertAnimationInChildBranch(AObject *TheNextAni, float PercentBranch, CoorPosition theAnchored, int level, float depthChange){
	ATreeBranch *thisBranch = this;
	int cntLevel = 0;
	while(thisBranch->branchChild.size()>0 && (cntLevel++) < level){
		thisBranch = branchChild.back();
	}
	thisBranch->InsertAnimation(TheNextAni, PercentBranch, theAnchored, depthChange);
}

void ATreeBranch::InsertAnimation(AObject *TheNextAni, float PercentBranch, CoorPosition theAnchored, float depthChange){
	int theLineSegInd = ceil(vertex * PercentBranch) - 1;

	//make sure the inserted image is in the front
	//TheNextAni->SetDepth(curDepth + depthChange);
	lineSegL[theLineSegInd]->AddAnchoredChild(TheNextAni,theAnchored,depthChange);
}

void ATreeBranch::UpdateAngleDrive(){
	UpdateAngleSpeed();
	CheckAngleConstraints(); // check the anglespeed

	deltaAngle += theSpeedControl->SpeedNormalization(angleSpeed);
	
	for(unsigned int i = 0; i < lineSegL.size(); i++){
		//adding some constraint logic here
		
		angle = (*rootAngle) + deltaAngle * pow((float)(i + indexOffset)/vertex,AngOrder);
		lineSegL[i]->SetAngle(angle);
	}
}

//compute the angle speed
void ATreeBranch::UpdateAngleSpeed(){
	//check position deviation
	Vector3 topDiff = (*branchTop);
	Vector3 targetV = (*targetP);

	////angle and position correction
	VectorSub(&topDiff, branchRootStart);
	VectorSub(&targetV, branchRootStart);

	VectorRotate(&topDiff, ((*rootAngle) - halfpi));
	VectorRotate(&targetV, ((*rootAngle) - halfpi));

	VectorSub(&topDiff, &targetV);


	float divation = abs(topDiff.X) + abs(topDiff.Y) + abs(topDiff.Z);

	if(!enableAngleAcc){
		//this is a linear state machine
		switch(angleUpdateStage){
		case 0:
			//first minimize the X direction
			if(divation < previousDiff){
				int compareTopX = (topDiff.X > 0)? 1:-1;
				angleSpeed = (compareRootY * compareTopX) * divation * angleConst;
				previousDiff = divation;
			}
			else{
				//stage 1 is completed
				angleUpdateStage = 1;
				previousDiff = 1000;
			}
		break;
		case 1:
			//then minimize Y direction
			if(divation < previousDiff){
				int compareTopY = (topDiff.Y < 0)? 1:-1;
				angleSpeed = (compareRootY * compareTopY) * divation * angleConst;
				previousDiff = divation;
			}
			else{
				//stage 2 is completed
				angleUpdateStage = 2;
				previousDiff = 1000;
			}
		break;
		default:
			break;
		}
	}else{
		//this involves a small cyclic state machine
		switch(angleUpdateStage){
		case 0:
			//first minimize the X direction
			if(divation < previousDiff){
				int compareTopX = (topDiff.X > 0)? 1:-1;
				angleAcc = (compareRootY * compareTopX) * divation * angleConst;
				angleSpeed += angleAcc;
				previousDiff = divation;
			}
			else{
				//stage 1 is completed
				angleUpdateStage = 1;
				previousDiff = 1000;
			}
		break;
		case 1:
			//then minimize Y direction
			if(divation < previousDiff){
				int compareTopY = (topDiff.Y < 0)? 1:-1;
				angleAcc = (compareRootY * compareTopY) * divation * angleConst;
				angleSpeed += angleAcc;
				previousDiff = divation;
			}
			else{
				//stage 2 is completed
				previousDiff = 1000;
				angleUpdateStage = (++fluctuationCnt > maxFluctuation) ? 2:0; //break or not
			}
		break;
		case 2:
			angleSpeed = 0;
			angleUpdateStage = 3;
		default:
			break;
		}
	}

	angleSpeed = angleSpeed * dampingFactor;
}

void ATreeBranch::CheckAngleConstraints(){
	//check the angle speed
	/*if(abs(angleSpeed) > maxAngleSpeed){	
		angleSpeed = (angleSpeed > 0) ? maxAngleSpeed : -maxAngleSpeed;
	}*/


}