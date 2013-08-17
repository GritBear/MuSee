#include "VerticalAnimationObj.h"

VerticalAnimationObj::VerticalAnimationObj(int VerticalAnimationIndex,  control *TheControl, bool SelfMotion, bool PreDrawn){
	InitAssign(VerticalAnimationIndex, TheControl, VerticalAnimationTexture);

	preDrawn = PreDrawn;
	selfMotion = SelfMotion;

	//initialize entrance effect parameters
	if(entranceEffectID != NoEffect)
		useEntranceEffextSprite = true;
}



void VerticalAnimationObj::Init(VerticalAnimationStruct *aVerticalAnimationStruct){
	//read basic info
	theReader->VerticalAnimationObjInfo(Index, width, height, maxFrame, animationColumns, 
		maxAnimationOrientation, collidable, boundX, boundY, usingTexture, textureID, stationary, entranceEffectID);
	
	maxFrame = 100;
	stationary = false;

	//store pointer
	theVerticalAnimationStruct = aVerticalAnimationStruct;

	//first update the center X,Y values in terms of percentage of the 
	centerBottomX = aVerticalAnimationStruct->CenterBottomX;
	centerBottomY = aVerticalAnimationStruct->CenterBottomY;
	centerTopX =  aVerticalAnimationStruct->CenterTopX;
	centerTopY = aVerticalAnimationStruct->CenterTopY;

	//forbid pre-drawing into the visible canvas
	if(centerTopX <= 1 )
		preDrawn = false;

	x = (centerTopX + centerBottomX)/2.0;
	y = (centerTopY + centerBottomY)/2.0;

	startTimeInMS = aVerticalAnimationStruct->StartingTimeInMS;

	xVel = aVerticalAnimationStruct->XVel;
	yVel = aVerticalAnimationStruct->YVel;

	animationOrientation = aVerticalAnimationStruct->AnimationOrientation;
	stationary = stationary && preDrawn;

	////Normalize speed
	//xVel = xVel * TimeStepInSecond;
	//yVel = yVel * TimeStepInSecond;



	//call to set the scale and rotation
	InitConst();

	frameDelay = aVerticalAnimationStruct->FrameDelay;
	depth = aVerticalAnimationStruct->Depth;

	VerticalAnimationObj::ComputeCoordinate();
}


void VerticalAnimationObj::Update(){
	AnimationObj::Update();

	centerBottomX += xVelPulse;
	centerBottomY += yVelPulse;
	centerTopX +=  xVelPulse;
	centerTopY +=  yVelPulse;
}

void VerticalAnimationObj::ComputeCoordinate(){
	//first to solve for normalized width
	float squareOrigionalHeight = height * height;

	float squareHeight = pow((centerTopX - centerBottomX),2) + pow((centerTopY - centerBottomY),2);

	float normalizedHalfWidth = sqrt(squareHeight / squareOrigionalHeight) * width / 2.0;

	//compute the four coordinates from the center xy
	if(animationOrientation == 0){
		x1 = centerBottomX - normalizedHalfWidth;
		x12 = centerBottomX + normalizedHalfWidth;
		x22 = centerTopX - normalizedHalfWidth;
		x2 = centerTopX + normalizedHalfWidth;
	}else{
		//flip x coordinates
		x12 = centerBottomX - normalizedHalfWidth;
		x1 = centerBottomX + normalizedHalfWidth;
		x2 = centerTopX - normalizedHalfWidth;
		x22 = centerTopX + normalizedHalfWidth;
	}

	y1 = y12 = centerBottomY;
	y22 = y2 = centerTopY;
}

void VerticalAnimationObj::DrawSquare(){
	float tx_scale = 1.0/((float)animationColumns);

	//only perform this calculation is the coordinates are not initialized
	/*
	X1 = x1 * screen_w + 0.5;
	Y1 = (1 - y1) * screen_h + 0.5;
	X2 = x2 * screen_w + 0.5;
	Y2 = (1 - y2) * screen_h + 0.5;
	X12 = x12 * screen_w + 0.5;
	Y12 = (1 - y12) * screen_h + 0.5;
	X22 = x22 * screen_w + 0.5;
	Y22 = (1 - y22) * screen_h + 0.5;
	*/

	X1 = x1;
	Y1 = y1;
	X2 = x2;
	Y2 = y2;
	X12 = x12;
	Y12 = y12;
	X22 = x22;
	Y22 = y22;

	if(useEntranceEffextSprite){
		//Recalculate coordinates and calculate sprite coordinates
		//the animation is going in the vertical direction 
		
		if(usingTexture){
			EntraceEffectPercentageCoor();

			glEnable(GL_TEXTURE_2D);
			glBindTexture( GL_TEXTURE_2D, textureID );
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
			
			glPushMatrix();
			glBegin(GL_QUADS);
			glColor4f(1.0, 1.0, 1.0, 1.0);	//this is necessary to ensure the correct color after blending	
				//glTexCoord2f coordinate in range of 0,1s
				glTexCoord2f(tx1,ty1); glVertex3f(X1,Y1, 0);
				glTexCoord2f(tx12,ty12); glVertex3f(X12,Y12, 0);
				glTexCoord2f(tx2,ty2); glVertex3f(X2,Y2,0);
				glTexCoord2f(tx22,ty22); glVertex3f(X22,Y22,0);
			
			glEnd();
			glPopMatrix();
			
			glDisable(GL_BLEND);

			glDisable(GL_TEXTURE_2D);		
		}else{		
			//some default implementation
		}
	//}else{ 
	//	//normal sprite_frame progression
	//	if(usingTexture){
	//		// if not using vertical sprite animation
	//		ty1 = ty12 = 0.0;
	//		ty2 = ty22 = 1.0;
	//		tx1 = tx22 = ((float)(sprite_frame % animationColumns)) * tx_scale;
	//		tx2 = tx12 = tx1 + tx_scale;
	//
	//		// Enable 2D texturing
	//		glEnable(GL_TEXTURE_2D);
	//		glBindTexture( GL_TEXTURE_2D, textureID );
	//	
	//		glEnable(GL_BLEND);
	//		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	

	//		glPushMatrix();
	//		glBegin(GL_QUADS);
	//		//glTexCoord2f coordinate in range of 0,1
	//		glColor4f(1.0, 1.0, 1.0, 1.0);	
	//		glTexCoord2f(tx1,ty1); glVertex3f(X1,Y1, 0);
	//		glTexCoord2f(tx12,ty12); glVertex3f(X12,Y12, 0);
	//		glTexCoord2f(tx2,ty2); glVertex3f(X2,Y2,0);
	//		glTexCoord2f(tx22,ty22); glVertex3f(X22,Y22,0);
	//		
	//		glEnd();
	//		glPopMatrix();

	//		glDisable(GL_TEXTURE_2D);
	//		glDisable(GL_BLEND);
	//	}else{		
	//		//some default implementation
	//	}
	}
}

void VerticalAnimationObj::CenterPosisitonUpdate(bool OutsideUpdate, float CenterBottomXVel,float CenterBottomYVel,float CenterTopXVel,float CenterTopYVel){
	if(OutsideUpdate){
	centerBottomX += CenterBottomXVel;
	centerBottomY += CenterBottomYVel;
	centerTopX +=  CenterTopXVel;
	centerTopY +=  CenterTopYVel;
	}

	if(selfMotion){
	//some self vabrations here
	
	
	}
}
