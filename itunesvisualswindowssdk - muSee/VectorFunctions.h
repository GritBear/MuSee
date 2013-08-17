#pragma once
#include <list>
#include <vector>
#include <math.h>
#include <queue>

using namespace std;

//coordinate of one vertex
struct Vector2{
	float X;
	float Y;
};


struct Vector3{
	float X;
	float Y;
	float Z;
};


Vector3 PercentageLengthV(Vector3 origin, Vector3 target, float percentage);
void VectorSum(Vector3 * base, Vector3 * value);
void VectorSub(Vector3 * base, Vector3 * value);
float VectorDistance(Vector3 * A, Vector3 * B); // output the euclidean distance
float VectorDistance(Vector3 * A); //distance from origin
void VectorMult(Vector3 * base, float value);
void VectorDiv(Vector3 * base, float value);
void VectorZero(Vector3 * A);
void VectorDQueZero(deque<Vector3> * A);

void VectorNormalizedSum(Vector3 * P, Vector3 * Speed, float timeElapsed);
void VectorDequeUpdate(deque<Vector3> * vDeque, Vector3 * Vel, float timeElapsed);
void VectorDequeSum(deque<Vector3> * ADque, Vector3 * value);
void VectorDequeCreateSquare(deque<Vector3> * ADque, float width, Vector3 * origin = nullptr);


void VectorRotate(Vector3 * A, float counterClockAngle); //rotate counterclockwise, onlyXY plane
void VectorRotate(deque<Vector3> * ADque, float counterClockAngle);

float VectorAbsSum(Vector3 * A); //output the sum of vector absolute value
float VectorNorm(Vector3 * A); //output the norm of vector
float VectorXYAngle(Vector3 * A);