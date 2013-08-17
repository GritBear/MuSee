#pragma once
#include"AnimationObj.h"

const float defaultAnchorSpeed = 0.3333;
const unsigned int maxAnchorTrace = 10;

//FlyingObjSpecialStruct Definition
struct FlyingObjStruct {
	float ScreenXVel;
	float ScreenYVel;
	float DiagonalDistance;
	
	bool useInitialObjPosition;
	bool EnableAnchor;

	float AnchorSpeed;

	float PullFactor;
	int PullPower;
	float BreakSpeedThreshold; //start to apply stop drag if this speed is exceeded, undefined if it is negative

	bool DisplayAnchor;
	bool UseFunctionDraw;

	//stop state
	bool DefineStopState;
	float StopMaxSpeed;
	float StopDrag; // coefficient for 2 order acceleration
	float MaxSpeed, MinSpeed; //per second (keep the direction but scale the speed if it exceeds these limits)

	float TailDurationInSecond; //disable by setting to zero or negative
	float TailLength; //disable by setting to zero or negative
};

//this class will implement the flying animation object (instead of draw on the canvas, it is movie like animation object)
//the function of flyingAnimation should be generic without many movement mode
/*
struct DrawStruct {
	bool isCircle; //this determines whether polygon or line loop
	list<vector<float>> vertexCoordinate;
	list<vector<float>> vertexColor;
};*/

//it should support both:
//	Trace FlyingAnimationTexture
//	Gravitational FlyingAnimationTexture
class FlyingAnimationObj : public AnimationObj{
private:
	bool displayAnchor;
	bool enableAnchor;
	bool defineStopState;
protected:
	//Animation Object Attributes
	bool useFunctionDraw;
	float Xacceleration, Yacceleration;
	float screenXVel, screenYVel;
	float screenXVelPulse, screenYVelPulse;
	float lifeTimeSpan, timeSpawnInMS;
	//for flying obj
	float DiagonalLength; //this is a back up
	float angle;
	float halfDiagonal;

	//FlyingAnimationTexture Parameters---------------------------------------------
	float accFactor;
	float accPower;

	//Trace Anchor Parameters---------------------------------------------
	//move along the trace
	float anchorSpeed;  //unit per second
	
	float anchorX, anchorY;
	float anchorXVel, anchorYVel; //unit per pulse

	float anchorTimeToNextStop;
	bool anchorStopped;

	float anchorXVelPulse, anchorYVelPulse;

	//trace
	vector<float> xTraceVec, yTraceVec;
	int traceCurrentInd, totalTraceInd;
	vector<float>::iterator xTraceIter, yTraceIter;

	//Coordinate Updateing Method------------------------------------------
	void ComputeAcceleration();
	void AnchorTransition();

	//Tail--------------------------------------------------------------------
	deque<float> tailXque, tailYque;
	int queueLengthN; //if using time storage
	float queueCurLength, queueMaxLength; //if use length storage

	void TailManagement();
	float RemoveLastTail(); //return removed length of the tail

	//Anchor Stopping/Slow down State---------------------------------------------
	//move along the trace
	float stopMaxSpeed;
	float stopDrag; // coefficient for 2 order acceleration
	float breakSpeedThreshold;
	float minSpeed, maxSpeed;

	//Draw Struct Storage For OpenGL simple graphics---------------------------------------
public:
	//Modified Core Functions
	FlyingAnimationObj(){};
	FlyingAnimationObj(int FlyingObjIndex, control *TheControl);

	~FlyingAnimationObj(){ Destroy();}
	
	void Destroy();

	void Init(AnimationObjStruct *AniStruct, FlyingObjStruct *flyingStruct);

	void Update();

	void Render(float Screen_w = 0, float Screen_h = 0, bool updating = true);

	void ComputeCoordinates();

	void DrawSquare();

	bool GetToRender(){return alive;}
	bool GetAlive(){return AnimationObj::GetAlive();}

	void UpdatePulseSpeed();

	//OpenGL function draw---------------------------------------------------------
	//void FunctionDrawInitialize(DrawStruct *aDrawStruct);
	//For Testing
	void DrawAnchor();

	//FlyingAnimationObj Special Getter and Setter---------------------------------------------------------
	void SetScreenVel(float ScreenXVel, float ScreenYVel){screenXVel = ScreenXVel; screenYVel = ScreenYVel;}
	void SetDiagonLength(float DiagonalLength){halfDiagonal = DiagonalLength/2.0;}
	void SetAcceleration(float aX, float aY){Xacceleration = aX; Yacceleration = aY;};
	void GetAcceleration(float &Xacc, float &Yacc){Xacc = Xacceleration; Yacc = Yacceleration;}

	void PushAnchorTrace(float X, float Y){	xTraceVec.push_back(X); yTraceVec.push_back(Y);totalTraceInd++;}
	void SetPull(float PullFactor, float PullPower){accFactor = PullFactor; accPower = PullPower;}
	float GetPullPower(){return accPower;}
	float GetPullFactor(){return accFactor;}
	bool GetAnchorStatus(){return anchorStopped;};

	float GetAnchorX(){return anchorX;}
	float GetAnchorY(){return anchorY;}
	float GetAnchorXVel(){return anchorXVel;}
	float GetAnchorYVel(){return anchorYVel;}
	float GetAnchorSpeed(){return anchorSpeed;}
	void EnableDisableAnchor(bool Enable);
};