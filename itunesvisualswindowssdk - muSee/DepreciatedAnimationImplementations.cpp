#include "FlyingAnimationObj.h"
#include "VerticalAnimationObj.h"

//========================================================================================================
//Animation Obj
//this implementation is DEPRECIATED
void AnimationObj::simplified_Init(float X1, float Y1, float X2, float Y2, float XVel, float YVel,
	float BoundX, float BoundY,int FrameDelay, int AnimationOrientation){
	
	x1 = X1;
	x2 = X2;
	y1 = Y1;
	y2 = Y2;

	x = (x1 + x2)/2.0;
	y = (y1 + y2)/2.0;

	xVel = XVel;
	yVel = YVel;

	//update square box boundary
	if( BoundX != 0 || BoundY != 0){
		boundX = BoundX;
		boundY = BoundY;

	}else{
		//default boundary
		boundX = boundX * scale;
		boundY = boundY * scale;
	}


	frameDelay = FrameDelay;
	animationOrientation = AnimationOrientation;

	//Normalize speed
	xVel = xVel * TimeStepInSecond;
	yVel = yVel * TimeStepInSecond;

	//call to set the scale and rotation
	ComputeCoordinate();
}

//this implementation is DEPRECIATED
void AnimationObj::Init(float X, float Y, int StartingTimeInMS, float XVel, float YVel,
	float X1, float Y1, float X2, float Y2, float BoundX, float BoundY ,int AnimationOrientation,int FrameDelay){

	x = X;
	y = Y;

	startTimeInMS = StartingTimeInMS;

	x1 = X1;
	x2 = X2;
	y1 = Y1;
	y2 = Y2;

	xVel = XVel;
	yVel = YVel;

	//update square box boundary
	if( BoundX != 0 || BoundY != 0){
		boundX = BoundX;
		boundY = BoundY;

	}else{
		//default boundary
		boundX = boundX * scale;
		boundY = boundY * scale;
	}


	frameDelay = FrameDelay;
	animationOrientation = AnimationOrientation;

	//Normalize speed
	xVel = xVel * TimeStepInSecond;
	yVel = yVel * TimeStepInSecond;

	//call to set the scale and rotation
	ComputeCoordinate();
}
//--------------------------------------------------------------------------------------------------------

//========================================================================================================
//Vertical Animtaion
//this implementation is DEPRECIATED
void VerticalAnimationObj::Init(float CenterBottomX, float CenterBottomY, float CenterTopX, float CenterTopY, float XVel, float YVel, int StartingTimeInMS, int AnimationOrientation, int FrameDelay){
	//first update the center X,Y values in terms of percentage of the 
	centerBottomX = CenterBottomX;
	centerBottomY = CenterBottomY;
	centerTopX =  CenterTopX;
	centerTopY = CenterTopY;

	//forbid pre-drawing into the visible canvas
	preDrawn = false;

	x = (centerTopX + centerBottomX)/2.0;
	y = (centerTopY + centerBottomY)/2.0;

	startTimeInMS = StartingTimeInMS;

	xVel = XVel;
	yVel = YVel;

	animationOrientation = AnimationOrientation;
	stationary = stationary && preDrawn;

	//Normalize speed
	xVel = xVel * TimeStepInSecond;
	yVel = yVel * TimeStepInSecond;

	//call to set the scale and rotation
	VerticalAnimationObj::ComputeCoordinate();
	frameDelay = FrameDelay;
}
//--------------------------------------------------------------------------------------------------------

//========================================================================================================