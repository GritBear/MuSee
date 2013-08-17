#include "FlyingAniComp.h"
//The motion component
AFlyMotion::AFlyMotion(AStruct * aStruct, AMotion *FlyingMotion){
	AMotion::Init(aStruct);
	AFlyMotion::Init(aStruct, FlyingMotion);
}

void AFlyMotion::Init(AStruct * aStruct, AMotion *FlyingMotion){
	flyingMotion = FlyingMotion;

	trace = (*aStruct->trace); //copy the entire Dqueue
	backgroundVel = (*aStruct->backgroundVel);

	//copy anchor
	anchor = (*aStruct->anchor);
	anchorVel = (*aStruct->anchorVel);
	InitCoorFromCenter();

	fMode = aStruct->fMode;
	speed = VectorNorm(&anchorVel);
	stopped =(trace.size() >= 2) ? false:true;

	distanceTillNext = 0;
	angleAbsSumFactor = 1;
	isAnchor = false;

	//copy functional parameters
	accFactor = aStruct->factor;
	accPower = aStruct->power;
	stopMaxSpeed = aStruct->maxSpeed; //in sumAbs
	drag = aStruct->drag; // coefficient for 2 order acceleration
}

void AFlyMotion::InitCoorFromCenter(){
	int pSize = position.size();
	coorFromCenter.resize(pSize);
	VectorDQueZero(&coorFromCenter);

	for(int i = 0; i < pSize; i++){
		VectorSub(&coorFromCenter[i], &anchor);
	}
	previousAngle = 0;
}


void AFlyMotion::Update(){
	Vector3 temp = backgroundVel;
	float timeElapsed = theSpeedControl->GetTimeElapsed();
	
	if(!stopped){
		VectorSum(&temp, anchorVelPtr);
	}

	VectorNormalizedSum(anchorPtr, &temp, timeElapsed);
	
	if(isAnchor){
		//update the distance
		VectorDequeUpdate(&trace, &backgroundVel, timeElapsed);
		distanceTillNext -= speed;

		if(distanceTillNext<=0){
			InitAnchor(); //initialize for the next journey
		}
	}else{
		AnchorPullDrive();
	}

	angle = VectorXYAngle(&anchorVel); //update angle
	AnchorBasedPositionUpdate();//this produces the position deque
	//AMotion::Update(); //updates the position deque
}

void AFlyMotion::AnchorBasedPositionUpdate(){
	//update the coorFromCenter dQue
	float angDiff = angle - previousAngle;

	if(abs(angDiff) > minRotataionThreshold){
		VectorRotate(&coorFromCenter, angDiff);
		previousAngle = angle;
		//sum that with the anchor
		FreeAll(position);
		position = coorFromCenter;//first make a copy
		VectorDequeSum(&position,  &anchor);
	}
}

void AFlyMotion::SetIsAnchor(bool IsAnchor, AFlyMotion * AnchorMotion){
	isAnchor = IsAnchor;
	anchorMotion = AnchorMotion;
	if(!IsAnchor){
		//copy some pointers
		anchorPtr = &AnchorMotion->anchor;
		anchorVelPtr = &AnchorMotion->anchorVel;
	}else{
		InitAnchor();
		//copy the ptr to itself
		anchorPtr = &anchor;
		anchorVelPtr = &anchorVel;
	}
	InitCoorFromCenter();
}

void AFlyMotion::InitAnchor(){
	if(trace.size() > 0){
		anchor = trace.front();
		trace.pop_front();
		ComputeAnchorVel();
	}else{
		stopped = true;
	}
}

void AFlyMotion::ComputeAnchorVel(){
	if(trace.size() > 0 || speed > 0.005){
		//this part is HIGHTLY Optimized!!
		anchorVel = trace.front(); //coordinates
		VectorSub(&anchorVel,  &anchor); //coordinates difference
		distanceTillNext = VectorDistance(&anchorVel); //distance
		//float timeTillNext = distanceTillNext / speed;  //time till next anchor
		VectorMult(&anchorVel, speed/distanceTillNext);//get the velocity
	}else{
		stopped = true;
	}
}

void AFlyMotion::AnchorPullDrive(){
	//initialization phase
	Vector3 coordDiveate = anchor;
	VectorSub(&coordDiveate,  anchorPtr); //get the coordinate difference
	Vector3 acc  = coordDiveate; //acceleration

	//use the deviation sum(instead of norm, as it tolerates more error)
	// in fact absSum/norm = 1 to 1.414
	angleAbsSumFactor = sin(angle) + cos(angle);
	float deviationNorm = VectorAbsSum(&coordDiveate) / angleAbsSumFactor;

	//calculate the accerleration based on the equation
	float accMag = accFactor * pow(deviationNorm, accPower);
	VectorMult(&acc, accMag/deviationNorm);

	//update velocity
	VectorSum(&anchorVel, &acc);

	//call subsequent helper function
	Slowing();
	CheckLim();
}

void AFlyMotion::CheckLim(){
	//check the limit of anchorVel and other variables
	float speed = VectorAbsSum(&anchorVel) / angleAbsSumFactor;
	if(stopMaxSpeed > 0 && speed > stopMaxSpeed){
		VectorMult(&anchorVel, stopMaxSpeed/speed);
		speed = stopMaxSpeed;
	}
}

void AFlyMotion::Slowing(){
	VectorMult(&anchorVel, drag);
}

//===========================================================================



//===========================================================================
//AFly Obj
AFly::AFly(AStruct * aStruct, AObject * AnimationToFly, AFly * AnchorObj){
	AObject::Init(aStruct);
	AFly::Init(aStruct,AnimationToFly, AnchorObj);
	AFly::InitSubComponent( aStruct);
}

void AFly::Init(AStruct * aStruct, AObject * AnimationToFly, AFly * AnchorObj){
	//copy basic element
	tailCopy = (*aStruct);
	animationToFly = AnimationToFly;
	anchorObj = AnchorObj;

	if(AnchorObj == nullptr){
		enableAnchor = false;
		SetIsAnchor(true);
	}

	theChildren->SetCleanLim(-2.5, 4, 2.5, -2.5);
	theChildren->SetActivateMode(AllActive);
	theChildren->Append(animationToFly);
}

void AFly::InitSubComponent(AStruct * aStruct){
	//spawn default anchor if it is a nullptr
	if(anchorObj == nullptr){
		AStruct tempAnchor = (*aStruct);
		//modifications
		tempAnchor.usingTex = false;

		//spawn anchorObj
		anchorObj = new AFly(&tempAnchor);
	}

	//put anchorObj into childrens for render and update
	theChildren->Append((AInterface*)anchorObj);
	anchorObj->SetToRender(displayAnchor);

	//hijacks all the components from the passed in animationObj
	aFlyMotion = new AFlyMotion(aStruct, animationToFly->GetMotion());
	aFlyMotion->SetIsAnchor(false,anchorObj->GetMotion());

	theMotion = (AMotion*)aFlyMotion;
	theEvent = animationToFly->GetEvent();
	theRender = animationToFly->GetRender();

	//copy variables
	position = theMotion->GetAniPostionDeque();
	vertex = theMotion->Getvertex();
}

void AFly::SetIsAnchor(bool anchor){
	aFlyMotion ->SetIsAnchor(anchor);
	if(anchor){
		anchorObj = nullptr;
		if(animationToFly == nullptr){
			//spawn a default animationObj

		}

	}
}