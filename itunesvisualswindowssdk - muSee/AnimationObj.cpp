#include "AnimationObj.h"
//=================================================================
//Core Functions of Animation Objects
//Author: Li(Lee) Xiong. All rights reserved.
//=================================================================
//Set up animation object essentials here and some default values

AnimationObj::AnimationObj(int AnimationIndex, control *TheControl, TextureType AnimationTypeID){
	//int FrameWidth = 256, int FrameHeight = 256, int MaxFrame = 1, int AnimationColumns = NULL ,int index = 0, bool Collidable = false, unsigned int TextureOffset = NULL
	InitAssign(AnimationIndex, TheControl, AnimationTypeID);
}

//a method to assign essential constructor parameters
void AnimationObj::InitAssign(int AnimationIndex, control *TheControl, TextureType AnimationTypeID){
	theControl = TheControl;
	
	theReader = TheControl->GetReader();
	theCreationCore = TheControl->GetCreationCore();
	theSpeedControl = TheControl->GetSpeedControl();

	Index = AnimationIndex;
	animationTypeID = AnimationTypeID;

	depth = -defaultAnimationObjDistance;
}


//Improved Initialization with struct implementation
void AnimationObj::Init(AnimationObjStruct *AniStruct){	
	//----------------------------------------
	theReader->AnimationObjFrameInfo(Index, width, height, maxFrame, animationColumns, 
		maxAnimationOrientation, collidable, boundX, boundY, usingTexture, textureID, entranceEffectID, stationary, animationTypeID);
	
	//initialize entrance effect parameters
	if(entranceEffectID != NoEffect)
		useEntranceEffextSprite = true;

	switch(entranceEffectID){
	case Diagonal:
			useEntranceEffextSprite = theReader->GetUseDiagonalSprite(Index, maxEntranceEffextSpriteFrame, animationTypeID);
			if(!useEntranceEffextSprite)
				entranceEffectID = NoEffect;
		break;
	default:
		break;
	}
	
	InitConst();
	//timeStep = TimeStep; //time increwment, used in update()

	usingOrientation = true;
	stationary = false;
	//----------------------------------

	//position coordinate
	x =  AniStruct->X;
	y =  AniStruct->Y;

	x1 =  AniStruct->X1;
	y1 =  AniStruct->Y1;
	x2 =  AniStruct->X2;
	y2 =  AniStruct->Y2;

	//error checking at initialization 
	if(x == 0 && y==0){
		x = (x1 + x2)/2.0;
		y = (y1 + y2)/2.0;
	}
	
	//velocity
	xVel = AniStruct->XVel;
	yVel = AniStruct->YVel;
	SetRotate(AniStruct->Rotation);
	
	boundX = AniStruct->BoundX;
	boundY =  AniStruct->BoundY;

	startTimeInMS =  AniStruct->StartingTimeInMS;
	animationOrientation =  AniStruct->AnimationOrientation;
	frameDelay = AniStruct->FrameDelay;

	////Normalize speed
	//xVel = xVel * TimeStepInSecond;
	//yVel = yVel * TimeStepInSecond;

	if(AniStruct->Radius > 0){
		SetCollisionRadius(AniStruct->Radius);
	}

	//call to set the scale and rotation
	ComputeCoordinate();
}

//called in the Init() method
void AnimationObj::InitConst(){
	//check the lower limit
	if(width == 0) width = 1;
	if(height == 0 ) height = 1;
	if(maxFrame == 0) maxFrame = 1;
	if(animationColumns == NULL) animationColumns = maxFrame;

	//set some default values
	scale = 1;
	rotate = 0;
	scale_velocity = 0;
	rotate_velocity = 0;
	collisionRadius = 0;
	//set starting frame values
	sprite_frame = 0;
	entranceEffextSpriteFrame = 0;
	curFrame = 0;
	maxEntranceEffextSpriteFrame = maxFrame; //by default
	
	//set texture variable
	alive = false;
	toRender = true;
}


// Basic Position Update
void AnimationObj::Update(){	
	//Normalize speed
	xVelPulse = theSpeedControl->SpeedNormalization(xVel);
	yVelPulse = theSpeedControl->SpeedNormalization(yVel);
	timeStep = theSpeedControl->SpeedNormalization(1000.0);

	x += xVelPulse;
	y += yVelPulse;
	x1 += xVelPulse;
	y1 += yVelPulse;
	x2 += xVelPulse;
	y2 += yVelPulse;

	x12 += xVelPulse;
	y12 += yVelPulse;
	x22 += xVelPulse;
	y22 += yVelPulse;

	scale += theSpeedControl->SpeedNormalization(scale_velocity);
	rotate += theSpeedControl->SpeedNormalization(rotate_velocity);

	if(x2 <= leftMostXValueToRenderAnimationObj)
		toRender = false;
}

// Draw the Object here
void AnimationObj::Render(float Screen_w, float Screen_h, bool updating){
	//update current screen dimension
	screen_w = Screen_w;
	screen_h = Screen_h;

	// Update sprite related parameters
	if(!stationary && alive){
		if(usingTexture){
			if(entranceEffextSpriteFrame == maxEntranceEffextSpriteFrame - 1)
				useEntranceEffextSprite = false;

			if(sprite_frame == maxFrame - 1){
				SetStationary(true);
			}else{
				if(curFrame<frameDelay)
					curFrame++;
				else{
					curFrame = 0;
					if(useEntranceEffextSprite)
						entranceEffextSpriteFrame = (entranceEffextSpriteFrame + 1) % maxEntranceEffextSpriteFrame;
					else
						sprite_frame = (sprite_frame + 1) % maxFrame; //make sure it is less than maxFrame
				}
			}
		}
	}

	if(GetToRender())//perform scaling,rotation and translation	
		DrawSquare();
	
}

//=================================================================
//Helper Functions
//=================================================================
//	Draw the blended Square
//	Default is a half transparent colored square
void AnimationObj::DrawSquare(){
	float tx_scale = 1.0/((float)animationColumns);
	
	X1 = x1;
	Y1 = y1;
	X2 = x2;
	Y2 = y2;

	X12 = x12;
	Y12 = y12;
	X22 = x22;
	Y22 = y22;
	
	if(useEntranceEffextSprite){
		ty1 = ty12 = 0.0;
		ty2 = ty22 = 1.0;
		tx1 = tx22 = 0.0;
		tx2 = tx12 = 1.0;
		//Recalculate coordinates and calculate sprite coordinates
		EntraceEffectPercentageCoor();
		//glEnable( GL_TEXTURE_2D ) is already initialized by default
		if(usingTexture){
			
			glEnable(GL_TEXTURE_2D);
			glBindTexture( GL_TEXTURE_2D, textureID );
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
			
			glPushMatrix();
			glBegin(GL_QUADS);
			glColor4f(1.0, 1.0, 1.0, 1.0);	//this is necessary to ensure the correct color after blending	
				//glTexCoord2f coordinate in range of 0,1s
				glTexCoord2f(tx1,ty1); glVertex3f(X1,Y1, depth);
				glTexCoord2f(tx12,ty12); glVertex3f(X12,Y12, depth);
				glTexCoord2f(tx2,ty2); glVertex3f(X2,Y2,depth);
				glTexCoord2f(tx22,ty22); glVertex3f(X22,Y22,depth);
			
			glEnd();
			glPopMatrix();
			
			glDisable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);		
			
		}else{	
			FunctionDraw();
		}

	}else{ 
		if(usingTexture){
			// if not using diagonal sprite animation but using sprite animation
			ty1 = ty12 = 0.0;
			ty2 = ty22 = 1.0;
			tx1 = tx22 = ((float)(sprite_frame % animationColumns)) * tx_scale;
			tx2 = tx12 = tx1 + tx_scale;

			// Enable 2D texturing
			glEnable(GL_TEXTURE_2D);
			glBindTexture( GL_TEXTURE_2D, textureID );
		
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

			glPushMatrix();
			glBegin(GL_QUADS);
			//glTexCoord2f coordinate in range of 0,1
			glColor4f(1.0, 1.0, 1.0, 1.0);	
			glTexCoord2f(tx1,ty1); glVertex3f(X1,Y1, depth);
			glTexCoord2f(tx12,ty12); glVertex3f(X12,Y12, depth);
			glTexCoord2f(tx2,ty2); glVertex3f(X2,Y2,depth);
			glTexCoord2f(tx22,ty22); glVertex3f(X22,Y22,depth);
			
			glEnd();
			glPopMatrix();

			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);
		}else{		
			FunctionDraw();
		}
	}
}

//default drawing
void AnimationObj::FunctionDraw(){
	// here a default OpenGL implementation is provided
	// center coordinate in unit of percentage of the screen width and height
	//float x, y;
	//conversion to GLfloat units
	//float X1,Y1,X2,Y2,X12,Y12,X22,Y22;
	//float x1, y1, x2,  y2; // for quad corners or connection points 
	//float x12, y12, x22, y22; // this is the coordinate values at the other two vertex, x12,y12 is the lower right and x22, y22 is the upper left
	glPushMatrix();
	glBegin(GL_QUADS);
		glColor4f(1.0, 1.0, 1.0, 1.0);	

		glColor4f(0.6f, 0.2f, 0.0f, 0.5f);
		glVertex3f(X1,Y1,depth);

		glColor4f(1.0f, 0.0f, 1.0f, 0.1f);
		glVertex3f(X12,Y12,depth);

		glColor4f(0.6f, 0.2f, 0.0f, 0.5f);
		glVertex3f(X2,Y2,depth);

		glColor4f(0.0f, 1.0f, 1.0f, 0.1f);
		glVertex3f(X22,Y22,depth);

	glEnd();
	glPopMatrix();

}

//different collision detections
bool AnimationObj::SquareCheckCollisions(AnimationObj *otherObject){
	float oX = otherObject->GetX();
	float oY = otherObject->GetY();

	int obX = otherObject->GetBoundX();
	int obY = otherObject->GetBoundY();

	if( x + boundX > oX - obX &&
		x - boundX < oX + obX &&
		y + boundY > oY - obY &&
		y - boundY < oY + obY)
		return true;
	else
		return false;
}

bool AnimationObj::RoundCheckCollishions(AnimationObj *otherObject){
	bool targetCollidable = otherObject->GetCollidable();
	if(targetCollidable && collidable){
		float xTarget = otherObject->GetX();
		float yTarget = otherObject->GetY();

		float squareRadiusTolerance = pow(otherObject->GetCollisionRadius() + collisionRadius, 2);
		float squareDistance = pow(xTarget - x,2) + pow(yTarget - y, 2);

		if(squareDistance > squareRadiusTolerance)
			return false;
		else
			return true;

	}else
		return false;
}

bool AnimationObj::CheckCollisionTopBottomScreen(float ScreenH){
	float tempScreenH = ScreenH;
	if(ScreenH == 0)
		tempScreenH = screen_h;

	bottom = 0.5 - tempScreenH/2.0;
	top = bottom + tempScreenH;

	if(y >= top - collisionRadius || y <= bottom + collisionRadius) return true;
	return false;
}

bool AnimationObj::CheckCollisionLeftRightScreen(float ScreenW){
	float tempScreenW = ScreenW;
	if(ScreenW == 0)
		tempScreenW = screen_w;

	left = 0.5 - tempScreenW/2.0;
	right = left + tempScreenW;

	if(x >= right - collisionRadius || x <= left + collisionRadius) return true;
	return false;
}

/*Compute the scale and rotation based on stored x1,x2,y1,y2 and width, height
	the coordinate of the other 2 vertex is also set here
*/
void AnimationObj::ComputeCoordinate(){
	/*	Default:	x1, y1 are bottom left position
					x2, y2 are top right position
					default: x1, y1 and x2, y2 are on the diagonal of the drawing square */
	//error checking
	if(width <= 1)
		width = 100;

	if(height <= 1)
		height = 100;
	
	//New Implementation--------------------------------------------------------
	float zeroRadian = atan(((float)height)/((float)width));
	rotate = atan(((y2-y1)*1000)/((x2-x1)*1000)) - zeroRadian;

	float X12Radian_Scale = 1;
	float	X12Radian_Offset = 0.0;

	if(usingOrientation)
		theReader->AngelScaleOffsetAtOrientation(Index, animationOrientation, X12Radian_Scale, X12Radian_Offset);
	
	x12rotate = -((rotate*X12Radian_Scale) + X12Radian_Offset); //change to clockwise

	RotationTranslationWithDiagonal(x1, y1, x2, y2, x12, y12, x22, y22,  width, height, x12rotate);
}

void AnimationObj::EntraceEffectPercentageCoor(){
	switch(entranceEffectID){
	case Diagonal:
		//default using diagonal
		if(!stationary){
			float percentage = ((float)entranceEffextSpriteFrame + 1.0)/((float)maxEntranceEffextSpriteFrame);
			Y2 = Y1 + (Y2 - Y1) * percentage;
			X2 = X1 + (X2 - X1) * percentage;
			X12 = X1 + (X12 - X1) * percentage;
			Y12 = Y1 + (Y12 - Y1) * percentage;
			X22 = X1 + (X22 - X1) * percentage;
			Y22 = Y1 + (Y22 - Y1) * percentage;
		
			ty1 = ty12 = 0.0;
			ty2 = ty22 = percentage;
			tx1 = tx22 = 0.0;
			tx2 = tx12 = percentage;

		}else{
			ty1 = ty12 = 0.0;
			ty2 = ty22 = 1.0;
			tx1 = tx22 = 0.0;
			tx2 = tx12 = 1.0;
		}
		break;

	case Vertical:
		if(!stationary){
			//(*here is different from animationOBj)
			float percentage = ((float)entranceEffextSpriteFrame + 1.0)/((float)maxEntranceEffextSpriteFrame);
			Y2 = Y12 + (Y2 - Y12) * percentage;
			X2 = X12 + (X2 - X12) * percentage;
			X22 = X1 + (X22 - X1) * percentage;
			Y22 = Y1 + (Y22 - Y1) * percentage;
		
			ty1 = ty12 = 0.0;
			ty2 = ty22 = percentage;
			tx1 = tx22 = 0.0;
			tx12 = tx2 = 1.0;

		}else{
			ty1 = ty12 = 0.0;
			ty2 = ty22 = 1.0;
			tx1 = tx22 = 0.0;
			tx2 = tx12 = 1.0;
		}
		break;
	default:
		break;
	}
	
	
}

