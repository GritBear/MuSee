#include "VectorFunctions.h"

//rotate a vector from center to x1,y1 
void PointRotation(float &x1, float &y1, float centerX,float centerY, float thetaClockwise){
	float Ks = sin(thetaClockwise);
	float Kc = cos(thetaClockwise);

	x1 = centerX + (x1 - centerX) * Kc + (y1 - centerY) * Ks;
	y1 = centerY + (y1 - centerY) * Kc - (x1 - centerX) * Ks;
}

//slightly faster for large number of points
void PointRotation(float &x1, float &y1, float centerX,float centerY, float sinThetaClockwise, float cosThetaClockwise){
	x1 = centerX + (x1 - centerX) * cosThetaClockwise + (y1 - centerY) * sinThetaClockwise;
	y1 = centerY + (y1 - centerY) * cosThetaClockwise - (x1 - centerX) * sinThetaClockwise;
}

//return the coordinate of 4 points after translation, using the original value of the four point as the zero condition
void RotationTranslation(float &x1, float &y1, float &x2, float &y2, float &x12, float &y12, float &x22, float &y22, float thetaClockwise){
	//precompute tri-identity
	float Ks = sin(thetaClockwise);
	float Kc = cos(thetaClockwise);
	
	//compute center
	float x = (x1 + x2)/2.0;
	float y = (y1 + y2)/2.0;
	
	//call point rotate 4 times
	PointRotation(x1, y1, x, y, Ks, Kc);
	PointRotation(x2, y2, x, y, Ks, Kc);
	PointRotation(x12, y12, x, y, Ks, Kc);
	PointRotation(x22, y22, x, y, Ks, Kc);
}

//return the four points after rotation, using only the diagonal value and lengthToWidthRatio
void RotationTranslationWithDiagonal(float &x1, float &y1, float &x2, float &y2, float &x12, float &y12, float &x22, float &y22, float Height, float Width, float thetaClockwise){
	//Default:	x1, y1 are bottom left position
	//				x2, y2 are top right position
	//				default: x1, y1 and x2, y2 are on the diagonal of the drawing square
	// width and length are just for ratio purpose
	
	float Ks = sin(-thetaClockwise);
	float Kc = cos(-thetaClockwise);

	//error checking
	if(Height == 0)
		Height = 100;

	if(Width == 0)
		Width = 100;

	//first to solve for normalized Length and Width
	float squareOrigionalDiagonal = Height * Height + Width * Width;

	float squareDiagonal = pow((x1 - x2),2) + pow((y1 - y2),2);

	float normalizedWidth = sqrt(squareDiagonal / squareOrigionalDiagonal) * Height;

	// Compute x12, y12, x22, y22 here
	float xOffset = normalizedWidth * Kc;
	float yOffset = normalizedWidth * Ks;

	if(x2 >= x1){
		x12 = x1 + xOffset;
		y12 = y1 + yOffset;
	
		x22 = x2 - xOffset;
		y22 = y2 - yOffset;
	}else{
		x12 = x1 - xOffset;
		y12 = y1 - yOffset;
	
		x22 = x2 + xOffset;
		y22 = y2 + yOffset;
	}
}

float PercentageLength(float origin, float target, float percentage){
	return origin + (target - origin) * percentage;
}

Vector3 PercentageLengthV(Vector3 origin, Vector3 target, float percentage){
	Vector3 ReturnV;
	ReturnV.X = PercentageLength(origin.X, target.X, percentage);
	ReturnV.Y = PercentageLength(origin.Y, target.Y, percentage);
	ReturnV.Z = PercentageLength(origin.Z, target.Z, percentage);
	return ReturnV;
}

void VectorSum(Vector3 * base, Vector3 * value){
	base->X += value->X;
	base->Y += value->Y;
	base->Z += value->Z;
}

void VectorSub(Vector3 * base, Vector3 * value){
	base->X -= value->X;
	base->Y -= value->Y;
	base->Z -= value->Z;
}

void VectorMult(Vector3 * base, float value){
	base->X = base->X * value;
	base->Y = base->Y * value;
	base->Z = base->Z * value;
}

void VectorDiv(Vector3 * base, float value){
	base->X = base->X / value;
	base->Y = base->Y / value;
	base->Z = base->Z / value;
}

void VectorNormalizedSum(Vector3 * P, Vector3 * Speed, float timeElapsed){
	P->X += Speed->X * timeElapsed;
	P->Y += Speed->Y * timeElapsed;
	P->Z += Speed->Z * timeElapsed;
}

float VectorDistance(Vector3 * A, Vector3 * B){
	return sqrt(pow( A->X - B->X, 2) + pow( A->Y - B->Y, 2)  + pow( A->Z - B->Z, 2));
}

float VectorDistance(Vector3 * A){
	return sqrt(pow(A->X, 2) + pow( A->Y, 2)  + pow(A->Z, 2));
}

void VectorZero(Vector3 * A){
	memset(A,0,sizeof(Vector3));
}

void VectorDQueZero(deque<Vector3> * A){
	for(int i = 0; i < A->size(); i++){
		VectorZero(&(*A)[i]);
	}
}

void VectorDequeUpdate(deque<Vector3> * vDeque, Vector3 * Vel, float timeElapsed){
	int queSize = vDeque->size();
	for(int i = 0; i < queSize; i++){
		VectorNormalizedSum(&(*vDeque)[i],Vel,timeElapsed);
	}
}

void VectorRotate(Vector3 * A, float counterClockAngle){
	Vector3 temp = (*A);
	float kc = cos(counterClockAngle);
	float ks = sin(counterClockAngle);

	temp.X = (*A).X * kc + (*A).Y * ks;
	temp.Y = (*A).Y * kc - (*A).X * ks;

	(*A).X  = temp.X;
	(*A).Y  = temp.Y;
}

void VectorRotate(deque<Vector3> * ADque, float counterClockAngle){
	float kc = cos(counterClockAngle);
	float ks = sin(counterClockAngle);

	for(int i = 0; i < ADque->size(); i++){
		Vector3 temp = (*ADque)[i];
		temp.X = (*ADque)[i].X * kc + (*ADque)[i].Y * ks;
		temp.Y = (*ADque)[i].Y * kc - (*ADque)[i].X * ks;

		(*ADque)[i].X  = temp.X;
		(*ADque)[i].Y  = temp.Y;
	}
}

void VectorDequeSum(deque<Vector3> * ADque, Vector3 * value){
	for(int i = 0; i < ADque->size(); i++){
		VectorSum(&(*ADque)[i], value);
	}
}

float VectorAbsSum(Vector3 * A){
	return abs(A->X) + abs(A->Y) + abs(A->Z);
}

float VectorNorm(Vector3 * A){ //output the norm of vector
	return VectorDistance(A);
}

float VectorXYAngle(Vector3 * A){
	return atan2(A->Y, A->X);
}

void VectorDequeCreateSquare(deque<Vector3> * ADque, float fullwidth, Vector3 * origin){
	float halfWidth = fullwidth/2.0;
	
	(*ADque)[0].X = -halfWidth;
	(*ADque)[0].Y = -halfWidth;
		
	(*ADque)[1].X = -halfWidth;
	(*ADque)[1].Y = halfWidth;

	(*ADque)[2].X = halfWidth;
	(*ADque)[2].Y = halfWidth;

	(*ADque)[3].X = halfWidth;
	(*ADque)[3].Y = -halfWidth;
	
	if(origin != nullptr){
		for(int i = 0; i < 4; i++){
			VectorSum(&(*ADque)[i], origin);
		}
	}
}