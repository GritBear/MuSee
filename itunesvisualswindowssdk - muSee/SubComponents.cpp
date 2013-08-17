#include "SubComponents.h"

//AMotion Object=========================================
void AMotion::Init(AStruct *theStruct){
	perpendicularToVelocity = theStruct->perpendicularToVelocity;
	angle = theStruct->angle;
	anchorMode = noAnchor;

	notAllowRotation = theStruct->notAllowRotation;

	AMotion::vertex = theStruct->aPositionQ.size();
	inverseMass = 1; //no effect
	speedConstrainted = false;
	/*
	minSpeed.clear();
	maxSpeed.clear();
	*/
	theControl = theStruct->aControl;
	theSpeedControl = theControl->GetSpeedControl();
	position.resize(vertex);
	speed.resize(vertex);

	//active = true;
	
	//copy initial values
	for(int i = 0; i < min((theStruct->aPositionQ).size(),vertex); i++){
		position[i] = (theStruct->aPositionQ)[i];
		speed[i] = (theStruct->aSpeedQ)[i];
	}

	//initialize headCenter
	if(position.size() >= 3){
		headCenter = position[1];
		VectorSum(&headCenter, &position[2]);
		VectorMult(&headCenter, 0.5);
	}else{
		headCenter = position.back();
	}
}

void AMotion::Destroy(){
	/*
	FreeAll(minSpeed);
	FreeAll(maxSpeed);
	*/
	FreeAll(position);
	FreeAll(speed);
}

void AMotion::Update(){
	if(anchorMode == noAnchor){
		UpdateScreenVel();

		//updates all dimensions
		for(int i = 0; i < vertex; i++){
			position[i].X += theSpeedControl->SpeedNormalization(speed[i].X);
			position[i].Y += theSpeedControl->SpeedNormalization(speed[i].Y);
			position[i].Z += theSpeedControl->SpeedNormalization(speed[i].Z);
			/*position[i].X += theSpeedControl->SpeedNormalization(-0.2);
			position[i].Y += theSpeedControl->SpeedNormalization(0);
			position[i].Z += theSpeedControl->SpeedNormalization(0);*/
		}
	}else{
		if(!notAllowRotation) //wether update rotation or not
			AngleUpdate();

		for(int i = 0; i < vertex; i++){
			position[i] = (*anchorPtr); //first copy the value
			VectorSum(&position[i], &previousDiff[i]);
		}
	}

	if(vertex >=3){
		headCenter = position[1];
		VectorSum(&headCenter, &position[2]);
		VectorMult(&headCenter, 0.5);
	}else{
		headCenter = position.back();
	}
}

void AMotion::UpdateScreenVel(){
	VectorDequeSum(&position,theSpeedControl->GetNormalizedScreenSpeed());
}

void AMotion::SetAnchor(CoorPosition CenterAnchor,Vector3 * ParentAchorPosition,float * ParentAnchorAngle){
	anglePtr = ParentAnchorAngle;
	previousAngle = (*ParentAnchorAngle); //initialize previousAngle
	anchorMode = CenterAnchor;
	anchorPtr = ParentAchorPosition;
	previousDiff.resize(vertex);

	for(int i = 0; i < vertex; i++){
		previousDiff[i] = position[i];
		//VectorSum(&position[i], anchorPtr); //first update
	}
}

void AMotion::AngleUpdate(){
	angle = (*anglePtr);
	float angleDiff = previousAngle - (*anglePtr);
	if(abs(angleDiff) > minRotataionThreshold){
		for(int i = 0; i < vertex; i++){
			VectorRotate(&previousDiff[i], angleDiff);
			previousAngle = (*anglePtr);
		}
	}
}

void AMotion::SetDepth(float depth){
	for(int i = 0; i < vertex; i++){
		position[i].Z = depth;
	}
}
//========================================================


//AEvent Object==========================================
AEvent::AEvent(AMotion * aMotion, AStruct *theStruct){
	theMotion = aMotion;
	position = aMotion->GetAniPostionDeque();
	vertex = theStruct->vertex;
	efftectMode = theStruct->effectMode;

	theShape = theStruct->theShape;
	totalTime = theStruct->totalTime;

	liveSpeed = (totalTime == 0)?  0:(1.0/totalTime); //0 means stationary

	totalEnterT = enterTime = theStruct->enterTime;
	totalExitT = exitTime = theStruct->exitTime;
	percentageOfLife = 0;

	theControl = theStruct->aControl;
	theSpeedControl = theControl->GetSpeedControl();
	Init();
}

void AEvent::Init(){
	//by default
	finalVertex = min(vertex,3);
	fadeAlpha = 1;
	curState = Enter;
	Update(); //update the initial states
}

void AEvent::Update(){
	//update time status
	if(enterTime>0){
		enterTime -= theSpeedControl->GetTimeElapsed();
		curState = Enter;
	}else if(percentageOfLife<100){
		percentageOfLife += theSpeedControl->SpeedNormalization(liveSpeed, Rendering);
		curState = NoState;
	}else if(exitTime>0){
		exitTime -= theSpeedControl->GetTimeElapsed();
		curState = Exit;
	}else{
		curState = Dead;
	}

}

//sychronize the event variable with the system
//this will be called in the update 
void AEvent::EventUpdate(EventType theType){
	curEvent = theType;
}

void AEvent::EventUpdate(deque<Vector3> * drawCoor, deque<Vector2> * textCoor){
	//check if it is during the entering
	//copy all values
	for(int i = 0; i < vertex; i++){
			//position = theMotion->GetAniPostionDeque();
			(*drawCoor)[i] = (*position)[i];
	}

	switch(theShape){
		case rect_dia:
		case rect_ver:
		default:
			finalVertex = 3;
			(*textCoor)[0].X = 0;
			(*textCoor)[0].Y = 0;

			(*textCoor)[1].X = 0;
			(*textCoor)[1].Y = 1;

			(*textCoor)[2].X = 1;
			(*textCoor)[2].Y = 1;

			(*textCoor)[3].X = 1;
			(*textCoor)[3].Y = 0;
	}

	switch(curState){
	case Enter:
		float percentage = (1 - enterTime / totalEnterT);
		
		switch(efftectMode){
		case regular: //including fade in
			switch(theShape){
			case rect_dia:
				//diagonal entering
				(*textCoor)[1].Y = percentage;
				(*textCoor)[2].X = percentage;
				(*textCoor)[2].Y = percentage;
				(*textCoor)[3].X = percentage;

				(*drawCoor)[1] = PercentageLengthV((*drawCoor)[0],  (*position)[1], percentage);
				(*drawCoor)[2] = PercentageLengthV((*drawCoor)[0],  (*position)[2], percentage);
				(*drawCoor)[3] = PercentageLengthV((*drawCoor)[0],  (*position)[3], percentage);
				break;
			case rect_ver:
				//vertical entering
				(*textCoor)[1].Y = percentage;
				(*textCoor)[2].Y = percentage;

				(*drawCoor)[1] = PercentageLengthV((*drawCoor)[0],  (*position)[1], percentage);
				(*drawCoor)[2] = PercentageLengthV((*drawCoor)[0],  (*position)[2], percentage);
				break;
			}

		case fadeIn:
			fadeAlpha = percentage;
			break;

		case noEffect:
		default:
			//nothing happened here
			break;
		}
	break;
	}
}

void AEvent::Destroy(){ }
//========================================================


//ARender Object============================================
ARender::ARender(AMotion * aMotion, AEvent * aEvent, AStruct * theStruct){
	
	theMotion =  aMotion;
	theEvent = aEvent;

	//Initiating
	Init(theStruct);
}

void ARender::Destroy(){}

void ARender::Init(AStruct * theStruct){
	blendColorMode = theStruct->blendColor;
	blendColor = selectColor(blendColorMode);
	fadeAlpha = 1;

	theControl = theStruct->aControl;;
	theReader = theControl->GetReader();
	theSpeed = theControl->GetSpeedControl();

	theShape = theStruct->theShape;
	
	vertex = theStruct->vertex;

	position = theMotion->GetAniPostionDeque();
	textCoor.resize(vertex);
	drawCoor.resize(vertex);

	texStruct = theStruct->texInfo;
	width = texStruct->width;
	height = texStruct->height;
	textureID = texStruct->texID;

	imData = &(theStruct->texInfo->texParameter);
	maxFrame = imData->max_frame;
	animationColumns = imData->animationColumns;

	usingTex = theStruct->usingTex;
	left_right_flipped = theStruct->left_right_flipped;
}

void ARender::Render(){
	theEvent->Update();
	Effect();
	DrawUnit();
}

//call the GL methods
void ARender::DrawUnit(){
	
	//useless anti-aliasing
	//glShadeModel(GL_SMOOTH);
	//glEnable( GL_LINE_SMOOTH );
	//glEnable( GL_POLYGON_SMOOTH );
	//glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
	//glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );

	if(usingTex){
		glEnable(GL_TEXTURE_2D);
		glShadeModel(GL_SMOOTH);
		glEnable( GL_LINE_SMOOTH );
		glEnable( GL_POLYGON_SMOOTH );
		glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );
		glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );

		glBindTexture( GL_TEXTURE_2D, textureID );
		glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	
		glPushMatrix();
		//select the shape
		switch(theShape){
		case rect_dia:
		case rect_ver:
			glBegin(GL_QUADS);
			break;
		case triangle:
			glBegin(GL_TRIANGLES);
			break;
		case circle:
		case polygon:
		default:
			glBegin(GL_POLYGON);
		}

		//incorporate blending color
		glColor4f(blendColor.r * fadeAlpha, blendColor.g * fadeAlpha, blendColor.b * fadeAlpha, fadeAlpha);	//this is necessary to ensure the correct color after blending
		
		//draw the unit
		for(int i = 0;i <vertex; i++){
			glTexCoord2f(textCoor[i].X, textCoor[i].Y);
			glVertex3f(drawCoor[i].X,drawCoor[i].Y,drawCoor[i].Z);
		}

		//end and clean up
		glEnd();
		glPopMatrix();
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
	}else{
		DrawDefault();
	}
} 

//render effects
void ARender::Effect(){
	if(theEvent->GetState() == NoState){
		fadeAlpha = 1;
		drawCoor = (theMotion->position);
	}else{
		theEvent->EventUpdate(&drawCoor, &textCoor);
		fadeAlpha = theEvent->fadeAlpha; //updated fading variable
	}
}

void ARender::DrawDefault(){
	float anchorWidth = 0.025;
	float aX1,aX2,aY1,aY2,aX12,aX22,aY12,aY22;
	if(vertex>0){
		aX1 = aX22 = drawCoor[0].X - anchorWidth;
		aX2 = aX12 = drawCoor[0].X + anchorWidth;
		aY1 = aY12 = drawCoor[0].Y - anchorWidth;
		aY22 = aY2 = drawCoor[0].Y + anchorWidth;
	
		glPushMatrix();

		glBegin(GL_QUADS);
			glColor4f(1.0, 1.0, 1.0, 1.0);	

			/*glColor4f(0.9f, 0.0f, 0.2f, 0.5f);
			glVertex3f(aX1,aY1,drawCoor[0].Z);

			glColor4f(0.9f, 0.0f, 0.2f, 0.5f);
			glVertex3f(aX12,aY12,drawCoor[0].Z);

			glColor4f(0.9f, 0.0f, 0.2f, 0.5f);
			glVertex3f(aX2,aY2,drawCoor[0].Z);

			glColor4f(0.9f, 0.0f, 0.2f, 0.5f);
			glVertex3f(aX22,aY22,drawCoor[0].Z);*/
	
			for(int i = 0;i <vertex; i++){
				if(i < vertex - 2)
					glColor4f(0.9f, 0.0f, 0.2f, 0.5f);
				else
					glColor4f(0.1f, 0.9f, 0.2f, 0.5f);

				glVertex3f(drawCoor[i].X,drawCoor[i].Y,drawCoor[i].Z);
			}


		glEnd();
		glPopMatrix();
	}
}
//========================================================

