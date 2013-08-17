#pragma once
#include "Global.h"
#include "control.h"

//enum DrawUnitShape{rect_dia, rect_ver, triangle, circle, polygon}; this is included in Reader
const float dontCare = -9999998;

enum CollisionMode{rect_collision, round_collision, polygon_collision, pixel_collision};
enum EventType{NoEvent};
//enum AState{NoState, Enter, Exit, Dead}; defined in global
enum FlyingMode {fTrace, fRandom, fLinear};
enum EffectMode{regular,fadeIn,noEffect};

struct AStruct{
	//Motion varibles
	int vertex; //number of vertex times 3
	deque<Vector3> aPositionQ; //starting with the bottom left, goes clockwise
	deque<Vector3> aSpeedQ;

	//effect
	EffectMode effectMode;
	blendColorMode blendColor;
	float effectTime; //a seperate timer for the special effects each animation may have

	/*EffectMode effectModeOut;
	float effectOutTime;*/

	//useful for tree branch
	float length; 
	
	float angle;
	float angIndexOffset;
	float angConstant;
	int angOrder;

	float damp;
	float maxAngleSpeed;

	float width;
	float minWidth;
	int widthOrder;

	//a set of general factor and power------
	//usage:
	//	AFlyMotion component computing acceleration and speed update
	float factor;
	float power;
	float stopMaxSpeed; //in sumAbs
	float drag; // coefficient for 2 order acceleration
	//----------------------------------

	bool enableAcc;
	bool straightLine;

	//Tail parameters
	bool enableTail;
	bool timeLimit;
	bool lengthLimit;
	float timeLim;
	//length limit reuse length variable
	deque<Vector3> * tailQ;
	deque<Vector3> * trace;
	Vector3 * backgroundVel;

	//flying animation special parameters
	FlyingMode fMode;
	bool notAllowRotation;

	bool perpendicularToVelocity;
	float speed;

	Vector3* anchor;
	Vector3* anchorVel;

	//ARender varibles
	DrawUnitShape theShape;

	float totalTime;
	//float liveSpeed;
	float enterTime;
	float exitTime;

	//constraint parameters
	float maxSpeed;
	
	bool usingTex;
	bool left_right_flipped;


	TextureImage *texInfo;
	//control elements
	control * aControl;

	AStruct * subAstruct;
};

