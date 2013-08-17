#include "ALineSeg.h"

//========================================================================
//ALineSegMotion sub-component
ALineSegMotion::ALineSegMotion(AStruct *theStruct, float *Angle){
	Init(theStruct,Angle);
	AMotion::Init(theStruct);
}

void ALineSegMotion::Init(AStruct *theStruct,float *Angle){
	SetAngle(Angle);
	endingWidth = theStruct->width;
	halfEndingWidth = endingWidth/2;
	isRoot = false; //default
	isTail = theStruct->enableTail;
	segL = theStruct->length;
}


void ALineSegMotion::Update(){
	rightAngle = (*angleptr) + halfpi;

	if(!isTail){
		Rotation();
		LinePosition();
	}

	//this is not necessary
	//AMotion::Update();
}

void ALineSegMotion::Rotation(){
	float xDeviation = segL * cos((*angleptr));
	float yDeviation = segL * sin((*angleptr));

	headCenter.X = (*startP).X + xDeviation;
	headCenter.Y = (*startP).Y + yDeviation;
	//headCenter.Z = (*startP).Z;
}

void ALineSegMotion::LinePosition(){
	//copy first two vertex from previous position Q
	if(!isRoot){
		position[0] = (*previousPositionQ)[1];
		position[3] = (*previousPositionQ)[2];
	}else{
		ComputeStart();
	}
	
	ComputeEnd();
}

void ALineSegMotion::SetRoot(bool IsRoot){
	isRoot = IsRoot;
	if(IsRoot){
		ComputeStart();
	}
}

void ALineSegMotion::ComputeStart(){
	float xdiv = halfEndingWidth * cos(rightAngle);
	float ydiv = halfEndingWidth * sin(rightAngle);

	position[0].X = (*startP).X + xdiv;
	position[0].Y = (*startP).Y + ydiv;
	//position[0].Z = (*startP).Z;

	position[3].X = (*startP).X - xdiv;
	position[3].Y = (*startP).Y - ydiv;
	//position[3].Z = (*startP).Z;
}

void ALineSegMotion::ComputeEnd(){
	//compute the new end
	float xdiv = halfEndingWidth * cos(rightAngle);
	float ydiv = halfEndingWidth * sin(rightAngle);

	position[1].X = headCenter.X + xdiv;
	position[1].Y = headCenter.Y + ydiv;
	//position[1].Z = headCenter.Z;

	position[2].X = headCenter.X - xdiv;
	position[2].Y = headCenter.Y - ydiv;
	//position[2].Z = headCenter.Z;
}


//========================================================================
ALineSeg::ALineSeg(AStruct * aStruct, int SegNum, ALineSeg *Previous, Vector3 * startP){
	//initialize line segment variables
	SetLineSeg(Previous);
	segNum = SegNum;
	segL = aStruct->length;
	angle = aStruct->angle;
	width = aStruct->width;

	if(segNum!= 0)
		currentStart = previousALineSeg->GetCurrentEnd();
	else
		currentStart = startP;

	//call AObject Init()
	AObject::Init(aStruct);
	ALineSeg::InitSubComponent(aStruct);
};

void ALineSeg::InitSubComponent(AStruct * aStruct){
	//initialize all components
	ALineSegMotion *theLineMotion = new ALineSegMotion(aStruct, &angle);

	theLineMotion->SetStartP(currentStart);
	if(segNum != 0){
		theLineMotion->SetPreviousPositionA(previousALineSeg->GetPosition());
		theLineMotion->SetRoot(false);
	}
	else
		theLineMotion->SetRoot(true);
	
	currentEnd = theLineMotion->GetEndP();
	theLineMotion->SetLineLengthWidth(segL,width);

	theMotion = (AMotion*)theLineMotion;
	theEvent = new AEvent(theMotion, aStruct);
	theRender = new ARender(theMotion,theEvent,aStruct);

	//copy variables
	position = theMotion->GetAniPostionDeque();
	vertex = theMotion->Getvertex();
}
