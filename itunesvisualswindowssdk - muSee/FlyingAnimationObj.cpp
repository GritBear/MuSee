#include "FlyingAnimationObj.h"

FlyingAnimationObj::FlyingAnimationObj(int FlyingObjIndex, control *TheControl){
	InitAssign(FlyingObjIndex, TheControl, FlyingAnimationTexture);

	//Set the Boolean Parameters
	usingTexture = false;
	usingOrientation = false;
	useFunctionDraw = false;
	useEntranceEffextSprite = false;
	anchorStopped = true;
	defineStopState = false;
	alive = true;
	displayAnchor = false;
	enableAnchor = true;

	//initialize accelerations
	Xacceleration = 
		Yacceleration = 0;

	width = 256;
	height = 256;

	//update time variables
	timeSpawnInMS = 0;
	anchorTimeToNextStop = 0;
	timeStep = TimeStep;

	xTraceVec.reserve(maxAnchorTrace);
	yTraceVec.reserve(maxAnchorTrace);
	traceCurrentInd = 0;
	totalTraceInd = 0;
	stopDrag = 0;
	stopMaxSpeed = 0;
	breakSpeedThreshold = -1;

	DebugValue2 = 0;

	queueLengthN = 0; //if using time storage
	queueCurLength = queueMaxLength = 0; //if use length storage

	//needs to resize tailXque, tailYque
	//tailXque.resize(300);
	//tailYque.resize(300);


}

void FlyingAnimationObj::Destroy(){
	//free(theFlyingStruct);
	//update the target trace
	if(xTraceVec.size()>0 && yTraceVec.size()>0){
		FreeAll(xTraceVec);
		FreeAll(yTraceVec);
	}

	tailXque.clear();
	tailYque.clear();

	AnimationObj::Destroy();
}

void FlyingAnimationObj::Init(AnimationObjStruct *AniStruct, FlyingObjStruct *flyingStruct){
	//store pointer for garbage collection
	//theFlyingStruct = flyingStruct;
	
	AnimationObj::Init(AniStruct);
	SetScreenVel((*flyingStruct).ScreenXVel, (*flyingStruct).ScreenYVel);
	
	DiagonalLength = (*flyingStruct).DiagonalDistance;
	halfDiagonal = DiagonalLength/2.0;
	useFunctionDraw = (*flyingStruct).UseFunctionDraw;
	//enableAnchor = (*flyingStruct).EnableAnchor;

	anchorSpeed = (*flyingStruct).AnchorSpeed;

	accFactor = (*flyingStruct).PullFactor;
	
	accPower = (*flyingStruct).PullPower;

	displayAnchor = (*flyingStruct).DisplayAnchor;
	defineStopState = (*flyingStruct).DefineStopState;

	stopDrag = (*flyingStruct).StopDrag;
	stopMaxSpeed = (*flyingStruct).StopMaxSpeed;

	breakSpeedThreshold = (*flyingStruct).BreakSpeedThreshold;

	//Tail parameters------------------------------------
	queueLengthN = ((*flyingStruct).TailDurationInSecond / TimeStepInSecond +0.5); //if using time storage
	queueMaxLength = (*flyingStruct).TailLength;

	//speed control
	minSpeed = flyingStruct->MinSpeed;
	maxSpeed = flyingStruct->MaxSpeed;

	//normalize time once for all------------------------
	//!! these parameters apply to speed in unit per second

	//stopDrag = stopDrag * TimeStepInSecond;
	//stopMaxSpeed = stopMaxSpeed * TimeStepInSecond;
	//accFactor = accFactor * TimeStepInSecond;
	//breakSpeedThreshold = breakSpeedThreshold * TimeStepInSecond;
	//minSpeed = minSpeed * TimeStepInSecond;
	//maxSpeed = maxSpeed * TimeStepInSecond;
	//---------------------------------------------------

	//initialize the flying object
	if(!(*flyingStruct).useInitialObjPosition){
		x = xTraceVec.front();
		y = xTraceVec.front();
	}

	//initialize anchor position and parameter
	if(xTraceVec.size()>0 && enableAnchor){
		anchorX = xTraceVec.front();
		anchorY = yTraceVec.front();
	
		xTraceIter = xTraceVec.begin();
		yTraceIter = yTraceVec.begin();

		anchorStopped = false;
		AnchorTransition();
	}
}

void FlyingAnimationObj::Update(){
	AnimationObj::Update();
	UpdatePulseSpeed();


	//update sprite, position and related info
	anchorX += anchorXVelPulse;
	anchorY += anchorYVelPulse;
	
	anchorX += screenXVelPulse;
	anchorY += screenYVelPulse;

	anchorTimeToNextStop -= timeStep;
	timeSpawnInMS += timeStep;

	DebugValue = anchorTimeToNextStop;
	if(anchorTimeToNextStop < 0 && enableAnchor){
		AnchorTransition();
	}

	ComputeAcceleration();

	//update acceleration
	xVel += Xacceleration;
	yVel += Yacceleration;

	//check speed constraint
	float curSpeed = sqrt(pow(xVel,2) + pow(yVel,2));
	float rescale = 1;
	if(curSpeed < minSpeed){
		rescale = minSpeed/curSpeed;
	}else if(maxSpeed > minSpeed && curSpeed >maxSpeed){
		rescale = maxSpeed/curSpeed;
	}

	xVel = xVel * rescale;
	yVel = yVel * rescale;

	//update the tail storage queue
	TailManagement();

	//update the target trace
	if(xTraceVec.size()>0 && yTraceVec.size()>0){
		for(xTraceIter = xTraceVec.begin(); xTraceIter!= xTraceVec.end(); xTraceIter++){
			(*xTraceIter)+=screenXVelPulse;
		}
		for(yTraceIter = yTraceVec.begin(); yTraceIter!= yTraceVec.end(); yTraceIter++){
			(*yTraceIter)+= screenYVelPulse;
		}
	}
}


//update all the pulse speed
void FlyingAnimationObj::UpdatePulseSpeed(){
	anchorXVelPulse = theSpeedControl->SpeedNormalization(anchorXVel);
	anchorYVelPulse = theSpeedControl->SpeedNormalization(anchorYVel);

	screenXVelPulse = theSpeedControl->SpeedNormalization(screenXVel);
	screenYVelPulse = theSpeedControl->SpeedNormalization(screenYVel);
}


void FlyingAnimationObj::Render(float Screen_w, float Screen_h, bool updating){
	FlyingAnimationObj::ComputeCoordinates();
	AnimationObj::Render(Screen_w, Screen_h, updating);
	if(displayAnchor)
		DrawAnchor();
}

void FlyingAnimationObj::DrawSquare(){
	AnimationObj::DrawSquare();
}

void FlyingAnimationObj::DrawAnchor(){
	float anchorWidth = 0.025;
	float aX1,aX2,aY1,aY2,aX12,aX22,aY12,aY22;

	aX1 = aX22 = anchorX - anchorWidth;
	aX2 = aX12 = anchorX + anchorWidth;
	aY1 = aY12 = anchorY - anchorWidth;
	aY22 = aY2 = anchorY + anchorWidth;
	
	glPushMatrix();
	glBegin(GL_QUADS);
		glColor4f(1.0, 1.0, 1.0, 1.0);	

		glColor4f(0.9f, 0.0f, 0.2f, 0.5f);
		glVertex3f(aX1,aY1,depth);

		glColor4f(0.9f, 0.0f, 0.2f, 0.5f);
		glVertex3f(aX12,aY12,depth);

		glColor4f(0.9f, 0.0f, 0.2f, 0.5f);
		glVertex3f(aX2,aY2,depth);

		glColor4f(0.9f, 0.0f, 0.2f, 0.5f);
		glVertex3f(aX22,aY22,depth);

	glEnd();
	glPopMatrix();
}

//------------------------------Trace Computations------------------------------------------------
//compute the trace coordinates
void FlyingAnimationObj::ComputeCoordinates(){
	//compute x1,y1,x2,y2
	if(yVel!=0 || xVel !=0){
		angle = atan(yVel/xVel);

		float xDifference = 0;
		float yDifference = 0; //the difference between x and x1, y and y1
		if(xVel>=0){
			xDifference = -halfDiagonal * cos(angle);
			yDifference = -halfDiagonal * sin(angle);
		}else{
			xDifference = halfDiagonal * cos(angle);
			yDifference = halfDiagonal * sin(angle);
		}

		//some error checking
		x1 = x + xDifference;
		y1 = y + yDifference;

		x2 = x - xDifference;
		y2 = y - yDifference;

		//error check
		if( (x1 == x2) && (y1 == y2)){
			x1 -= DiagonalLength;
		}
	
		AnimationObj::ComputeCoordinate();	
	}
}

//compute the acceleration of the flying object
void FlyingAnimationObj::ComputeAcceleration(){
	//Output: float Xacceleration, Yacceleration; (in unit per pulse)
	//available: x,y,pullfactor, pullpower, anchorX, anchorY
	if(enableAnchor){
		float xDeviation = anchorX - x;
		float yDeviation = anchorY - y;

		int XAccelerationSign = (xDeviation <= 0) ? -1 : 1;
		int YAccelerationSign = (yDeviation <= 0) ? -1 : 1;
	
		Xacceleration = accFactor * abs(pow(xDeviation,accPower)) * XAccelerationSign;
		Yacceleration = accFactor * abs(pow(yDeviation,accPower)) * YAccelerationSign;
		float objectSpeed = sqrt(pow(xVel,2) + pow(yVel,2));

		if(!anchorStopped){
			if(breakSpeedThreshold > 0 &&  objectSpeed > breakSpeedThreshold){
				float xVelDrag = (-xVel * abs(xVel)) * stopDrag;
				float yVelDrag = (-yVel * abs(yVel)) * stopDrag;
				Xacceleration += xVelDrag;
				Yacceleration += yVelDrag;
			}
		}else if(defineStopState){
			if(objectSpeed > stopMaxSpeed){
				float xVelDrag = (-xVel * abs(xVel)) * stopDrag;
				float yVelDrag = (-yVel * abs(yVel)) * stopDrag;
				Xacceleration += xVelDrag;
				Yacceleration += yVelDrag;
			}
		}
	}
}

//compute anchor movement
void FlyingAnimationObj::AnchorTransition(){
	if(traceCurrentInd < totalTraceInd - 1){
		anchorStopped = false;
		traceCurrentInd++;

		float nextX = xTraceVec[traceCurrentInd];
		float nextY = yTraceVec[traceCurrentInd];;

		float distance = sqrt(pow(nextX - anchorX,2) + pow(nextY - anchorY,2));
		//simple error checking
		if(anchorSpeed <= 0)
			anchorSpeed = defaultAnchorSpeed;

		anchorTimeToNextStop = distance / anchorSpeed;
		anchorXVel = (nextX - anchorX)/anchorTimeToNextStop; //convert into per-pulse scale
		anchorYVel = (nextY - anchorY)/anchorTimeToNextStop;

	}else{
		anchorTimeToNextStop = 0;
		anchorXVel = 0;
		anchorYVel = 0;
		anchorStopped = true;
	}
}

void FlyingAnimationObj::EnableDisableAnchor(bool Enable){
	if(enableAnchor && !Enable){
		//empty the acceleration
		Xacceleration = 0;
		Yacceleration = 0;
	}
	enableAnchor = Enable;
}

//------------------------------Trace Computations------------------------------------------------
void FlyingAnimationObj::TailManagement(){
	if(tailXque.size() > 0){
		float TailLengthInc = sqrt(pow(tailXque.back() - x,2) + pow(tailYque.back() - y, 2));
		queueCurLength += TailLengthInc;
	}
	//first insert the 
	tailXque.push_back(x);
	tailYque.push_back(y);

	//compute length constraint
	if(queueMaxLength>0){
		while(queueCurLength > queueMaxLength){
			float removedLength = RemoveLastTail();//cur length is already updated inside
		}
	}
	//compute time constraint
	if(queueLengthN > 0){
		while(tailXque.size() > queueLengthN){
			tailXque.pop_front();
			tailYque.pop_front();
		}
	}
}

float FlyingAnimationObj::RemoveLastTail(){
	float removedX,removedY,removedLength;
	if(tailXque.size() == 0)
		return 0;

	removedX = tailXque.front();
	removedY = tailYque.front();
	removedLength = sqrt(pow(x - removedX,2) + pow(y - removedY,2));
	//remove
	tailXque.pop_front();
	tailYque.pop_front();

	queueCurLength -= removedLength;
	
	//error checking
	if(queueCurLength < 0)
		queueCurLength = 0;

	return removedLength;
}

//------------------------------OpenGL Function Drawing (simple shapes)------------------------------------------------
/*
//void FlyingAnimationObj::FunctionDrawInitialize(DrawStruct *aDrawStruct){
	
	if(aDrawStruct!=NULL){
		usingTexture = false;
		theDrawStruct = (*aDrawStruct); //copy the struct
		useFunctionDraw = true;
	}
	
}*/

