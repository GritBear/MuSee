#include "AnimationComponents.h"



//AObject Object==========================================
AObject::AObject(){
	vertex = 0;
	SetActive(false);
	id = 0;
}


AObject::AObject(AStruct * aStruct){
	Init(aStruct);
	InitSubComponent(aStruct);
}


void AObject::InitSubComponent(AStruct * aStruct){
	//initialize all components
	theMotion = new AMotion(aStruct);

	//copy variables
	position = theMotion->GetAniPostionDeque();
	speed = theMotion->GetAniSpeedDeque();
	//for debug
	theEvent = new AEvent(theMotion, aStruct);
	theRender = new ARender(theMotion,theEvent,aStruct);

}


void AObject::Init(AStruct * aStruct){
	//set initial variables
	SetActive(false);

	angle = 0;

	theControl = aStruct->aControl;
	vertex = aStruct->vertex;
	timeLimit = aStruct->timeLimit;
	timeLim = aStruct->timeLim;
	tailEnabled = aStruct->enableTail;

	theReader = theControl->GetReader();
	theCreationCore = theControl->GetCreationCore();
	theSpeedControl = theControl->GetSpeedControl();
	width = aStruct->width;
	theChildren = new AniDispatcher();

	spawningT = clock();
}

void AObject::Destroy(){
	//destroy all components and storage variables
	delete(theRender);
	delete(theEvent);
	delete(theMotion);
	delete(theChildren);
}


void AObject::Update(){
	if(toUpdate){
		theMotion->Update();
		//if(theChildren->GetActivateMode() == Sequential){
			//if(theEvent->GetState() != Enter);
			theChildren->Update();
		//}else{
			//theChildren->Update();
		//}
	}
}


void AObject::Event(EventType theType){
	//theEvent->EventUpdate(theType);
}

void AObject::Render(){
	if(toRender){
		theRender->Render();
		theChildren->Render();
	}
}

void AObject::SetAnchor(CoorPosition CenterAnchor,Vector3 * ParentAchorPosition,float * ParentAnchorAngle){
	theMotion->SetAnchor(CenterAnchor,ParentAchorPosition,ParentAnchorAngle);
}

void AObject::AddAnchoredChild(AObject * aChild, CoorPosition CenterAnchor,float deltaDepth){
	Vector3 * theAnchorptr;

	if(CenterAnchor == headCenter){
		theAnchorptr = &theMotion->headCenter;
	}else{
		theAnchorptr = &(*position)[CenterAnchor];
	}
	
	//compute the position queue (only work for rectangular textures)
	//for(int i = 0; i<aChild->position->size();i++){
	//	//assume 0,0,0 is the anchor object's original position, now need to shift 0,0,0 -> anchor
	//	VectorSum(&(*aChild->position)[i],theAnchorptr);
	//}

	//set some inheritance value for branch
	//aChild->width = width;
	aChild->SetAnchor(CenterAnchor,theAnchorptr, &angle);
	aChild->SetDepth((*position)[0].Z + deltaDepth);
	aChild->Update();//initial update
	theChildren->Insert(aChild);
}


bool AObject::checkInCleanLim(float left,float right,float top,float bottom){
	//check time limit first
	if(timeLimit){
		float Telapsed = (float(clock() - spawningT))/CLOCKS_PER_SEC;
		if( Telapsed>timeLim)
			return true;
	}

	//check all limits (consider dontCare)
	for(int i = 0; i < position->size(); i++){
		if(((*position)[i].X >= left || left==dontCare)  && ((*position)[i].X<= right || right== dontCare) &&
			((*position)[i].Y >= bottom || bottom== dontCare) && ((*position)[i].Y <= top || top==dontCare))
			return true;
	}
	return false;
}

void AObject::Clean(){
	//maintain the childrens
	theChildren->Clean();
}

float AObject::GetAvgX(){
	float xsum = 0;
	for(int i = 0; i<vertex; i++){
		xsum += (*position)[i].X;
	}
	
	return xsum/vertex;
}

float AObject::GetAvgY(){
	float ysum = 0;
	for(int i = 0; i<vertex; i++){
		ysum += (*position)[i].Y;
	}
	
	return ysum/vertex;

}

//========================================================