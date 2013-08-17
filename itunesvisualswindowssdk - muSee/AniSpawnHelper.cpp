#include "AniSpawningHelper.h"

//rec_dia spawning
void RectDia2Point2DSpawn(float x1, float y1,float x2,float y2, float xVel, float yVel,  bool flip, deque<Vector3> &position, deque<Vector3> &speed, float width, float height){
	//call the convert method
	float x12 = 0;
	float y12 = 0;
	float x22 = 0;
	float y22 = 0;
	
	//error checking. Make sure dimension is correct
	if((position).size()!=12)
		(position).resize(12);

	if((speed).size()!=12)
		(speed).resize(12);
	
	if(width <= 1)
		width = 100;

	if(height <= 1)
		height = 100;

	//copy speed
	for(int i = 0; i<speed.size(); i++){
		speed[i].X = xVel;			//x-coordinates
		speed[i].Y = yVel;		//y-coordinates
		speed[i].Z = 0;			//z-coordinates
	}

	//compute the angle
	float zeroRadian = atan(((float)height)/((float)width));
	float rotate = -(atan((y2-y1)/(x2-x1)) - zeroRadian);

	RotationTranslationWithDiagonal(x1, y1, x2, y2, x12, y12, x22, y22,  width, height, rotate); //called from "independentfunction.h"

	if(flip){
		float temp;
		temp = x12;
		x12 = x22;
		x22 = temp;

		temp = y12;
		y12 = y22;
		y22 = temp;
	}

	//copy values
	position[0].X = x1;
	position[0].Y = y1;
	position[0].Z = 0;

	position[1].X = x22;
	position[1].Y = y22;
	position[1].Z = 0;

	position[2].X = x2;
	position[2].Y = y2;
	position[2].Z = 0;

	position[3].X = x12;
	position[3].Y = y12;
	position[3].Z = 0;
}

bool RectDiaRadConstraintSpawning(float x1, float y1,float x2,float y2, float xVel, float yVel, float previousAngleInRad, bool flip, AStruct &theStruct){
	float angle = atan((y2 - y1)/(x2 - x1)); //useful later in upward downward detection
	float maxAngleChangeInRad = theStruct.texInfo->texParameter.max_AngleChangeInRad;
	float minDiaLength = theStruct.texInfo->texParameter.minLength;
	float maxDiaLength = theStruct.texInfo->texParameter.maxLength;

	theStruct.aPositionQ = deque<Vector3>(4);
	theStruct.aSpeedQ = deque<Vector3>(4); 


	if(!(previousAngleInRad == NotAnAngle ||  maxAngleChangeInRad == NotAnAngle)){
		if(abs(angle - previousAngleInRad) > maxAngleChangeInRad){
			float angleDifference = (angle - previousAngleInRad < 0) ? -maxAngleChangeInRad : maxAngleChangeInRad;
			float correctAngle = previousAngleInRad + angleDifference;

			float yAdjust = tan(correctAngle) * (x2 - x1);
			y2 =  y1 + yAdjust;
		}
	}

	float diagonalLength = sqrt(pow((x2-x1), 2) + pow((y2-y1), 2));
	if((diagonalLength >= minDiaLength) && (diagonalLength <= maxDiaLength)){
	
		theStruct.enterTime = UniformDistribution(theStruct.texInfo->texParameter.minEntranceTime, theStruct.texInfo->texParameter.maxEntranceTime);
		theStruct.exitTime = UniformDistribution(theStruct.texInfo->texParameter.minExitTime, theStruct.texInfo->texParameter.maxExitTime);

		RectDia2Point2DSpawn(x1, y1, x2, y2, xVel, yVel, flip, theStruct.aPositionQ, theStruct.aSpeedQ, theStruct.texInfo->width, theStruct.texInfo->height);
		return true; //spawn successed
	}else if(diagonalLength > maxDiaLength){
		float resizeScale = (float)diagonalLength/(float)maxDiaLength;
		float x_Offset = x2 - x1;
		float y_Offset = y2 - y1;

		x_Offset = x_Offset / resizeScale;
		y_Offset = y_Offset / resizeScale;

		x2 = x1 + x_Offset;
		y2 = y1 + y_Offset;

		theStruct.enterTime = UniformDistribution(theStruct.texInfo->texParameter.minEntranceTime, theStruct.texInfo->texParameter.maxEntranceTime);
		theStruct.exitTime = UniformDistribution(theStruct.texInfo->texParameter.minExitTime, theStruct.texInfo->texParameter.maxExitTime);

		RectDia2Point2DSpawn(x1, y1, x2, y2, xVel, yVel, flip, theStruct.aPositionQ, theStruct.aSpeedQ, theStruct.texInfo->width, theStruct.texInfo->height);

		return true; //spawn forced successed
	}else{
		return false; //spawn unsuccessed
	}
}	


//Tree Branch Spawn
void TreeBranchSpawn(Vector3 root, AStruct &theStruct){

}