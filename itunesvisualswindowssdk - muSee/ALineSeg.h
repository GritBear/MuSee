#pragma once
#include "AnimationComponents.h"

//create Motion child class for line segment
class ALineSegMotion : AMotion{
private:
	bool isRoot;
	bool isTail;
protected:
	Vector3 * startP;
	deque<Vector3> *previousPositionQ;
	
	float endingWidth;
	float halfEndingWidth;
public:
	//line dimensions
	float *angleptr; //link to the line animation object
	float rightAngle;
	float segL;

	ALineSegMotion(AStruct *theStruct,float *Angle);
	void Init(AStruct *theStruct,float *Angle);

	void Update();
	void Rotation();
	void LinePosition();

	void SetAngle(float * Ang){angleptr = Ang;rightAngle = (*angleptr) + halfpi;}
	void SetPreviousPositionA(deque<Vector3> * previousPosition){previousPositionQ = previousPosition;}
	void SetLineLengthWidth(float SegLength, float width){segL = SegLength; endingWidth = width;halfEndingWidth = width*0.5;}
	void SetRoot(bool IsRoot);
	
	void ComputeStart();
	void ComputeEnd();

	void SetStartP(Vector3 * StartP){startP = StartP;}
	Vector3 * GetEndP(){return &headCenter;}
};


//it is a square
class ALineSeg : public AObject{
private:
protected:
	int segNum; //0 is root
	ALineSeg *previousALineSeg;

	Vector3 * currentStart;
	Vector3 * currentEnd;

	float segL;
public:
	ALineSeg(){};//default constructor

	ALineSeg(AStruct * aStruct, int SegNum, ALineSeg *Previous = nullptr, Vector3 * startP = nullptr);
	~ALineSeg(){AObject::Destroy();}

	//void Destroy(){AObject::Destroy();}

	void InitSubComponent(AStruct * aStruct);
	void InitLine(float Width);

	//getter and setter
	//Vector3 GetFinalVertex(){return headCenter;}
	
	void SetLineSeg(ALineSeg * previous){previousALineSeg = previous;}
	
	void SetAngle(float Angle){angle = Angle;}
	float GetAngle(){return angle;}

	void SetWidth(float Width){width = width;}

	Vector3 * GetCurrentEnd(){return currentEnd;}
	void SetCurrentStart(Vector3 * curStart){currentStart = curStart;}
};