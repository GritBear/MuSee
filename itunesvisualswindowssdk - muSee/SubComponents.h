#pragma once
#include "AniStructs.h"

const float minRotataionThreshold = pi/180 * 0.5; //only update rotation if the angle change is greater than this
class AMotion {
private:
protected:
	//constraints
	bool speedConstrainted;
	bool notAllowRotation;
	float previousAngle; //the angle at previous update frame
	deque<Vector3> previousDiff;
	/*
	deque<Vector3> minSpeed;	
	deque<Vector3> maxSpeed; 
	*/
	float inverseMass; //M*force = acceleration

	control *theControl;
	SpeedControl *theSpeedControl; //a local storage to speed up computations
	//should have another centralized collision control

public:

	CoorPosition anchorMode;
	bool perpendicularToVelocity;
	float angle;
	float * anglePtr;
	//attributes
	int vertex;

	//the pointer for anchored animation
	Vector3 *anchorPtr;
	Vector3 *anchorVelPtr;

	deque<Vector3> position; //starting from vertex 1 to vertex N.. not coordinates on screen
	deque<Vector3> speed;	//starting from vertex 1 to vertex N, per second
	Vector3 headCenter;

	//Essential methods
	AMotion(){} //default

	AMotion(AStruct *theStruct){Init(theStruct);}
	~AMotion(){Destroy();}
	
	void Init(AStruct *theStruct);
	void Destroy();
	virtual void Update();
	void UpdateScreenVel();

	//if it is anchored update
	void SetAnchor(CoorPosition CenterAnchor,Vector3 * ParentAchorPosition,float * ParentAnchorAngle);
	void AngleUpdate(); //for anchored animation


	//Adjustment methods
	void AccelerationAll(deque<float>* acc);
	void Acceleration2(float dim1Acc, float dim2Acc);

	/*
	void PullForce(deque<Vector3>* force);
	void SetConstriant(deque<Vector3>* MinSpeed, deque<float>* MaxSpeed);
	void SetInverseMass(float newInverMass){inverseMass = newInverMass;}
	*/

	//powerful overwriter
	void SetPosition(deque<Vector3> * newPositionQ);
	void SetSpeed(deque<Vector3> * newSpeedQ);
	void SetDepth(float depth);

	//getters
	int Getvertex(){return vertex;}
	Vector3 GetPosition(int dim){return position[dim];}
	Vector3 GetSpeed(int dim){return speed[dim];}

	deque<Vector3>* GetAniPostionDeque(){return &position;}
	deque<Vector3>* GetAniSpeedDeque(){return &speed;}
};



class AEvent{
//it controls collision, unexpectated effects  (interactions with music and other objects)
private:
	AMotion * theMotion;
	bool noStateFinished;
protected:
	CollisionMode collisionMode;
	EventType curEvent;
	AState curState;
	DrawUnitShape theShape;
	EffectMode efftectMode;

	int finalVertex;


	float percentageOfLife; //between 0 and 100 (0 after entrance, 100 starts exiting)
	float liveSpeed;	//per-second
	float totalTime; //total life time
	float enterTime, exitTime;
	float totalEnterT, totalExitT;
	int vertex;
	deque<Vector3> * position;

	//control units
	control * theControl;
	SpeedControl *theSpeedControl;

public:
	//fade in/out
	float fadeAlpha;

	AEvent(AMotion * aMotion, AStruct *theStruct);
	~AEvent(){Destroy();}

	void Init();

	void Destroy();
	void Update();

	void EventUpdate(EventType theType);
	void EventUpdate(deque<Vector3> * drawCoor, deque<Vector2> * textCoor);

	//Getterand Setter
	void SetCollisionMode(CollisionMode aCollisionMode){collisionMode = aCollisionMode;}
	CollisionMode GetCollisionMode(){return collisionMode;}
	AState GetState(){return curState;}

	Vector3 GetFinalVertex(){return (*position)[finalVertex];}
};




//2nd letter: u:upper, l:left; 3rd letter: l:left,r:right
//enum coorIndex {xll,yll,zll, xul,yul,zul,xur,yur,zur,xlr,ylr,zlr};
class ARender{ //this is the render method
private:
	int vertex;
	bool overMotion;
	AMotion * theMotion;
	AEvent * theEvent;
	DrawUnitShape theShape;
	bool usingTex;

protected:
	//fade in/out
	float fadeAlpha;
	blendColorMode blendColorMode;
	Color3 blendColor;

	//Sprite Info
	// sprite frame parameters
	int width;
	int height;

	TextureImage * texStruct;
	TexParameter * imData;

	float curSpriteTimeLeft;
	int sprite_frame;
	int maxFrame;

	float bottom,top,left,right;
	int animationColumns; //usually useless

	//control
	control* theControl;
	DataReader * theReader;
	SpeedControl *theSpeed;
public:
	//screen info
	//reminder: screen maximum width( -0.5 : 1.5), height(0,bottom:1),
	
	//Texture Info
	GLuint textureID;

	//i*3 is x, i*3 + 1 is y, i*3 + 2 is z
	//i, and i+1 for 2D texture coordinate clockwise
	deque<Vector2> textCoor;	//coordinates on the textures 
	deque<Vector3> * position;		//coordinates on the screen
	deque<Vector3> drawCoor;		//after the enter/exit effect coordinate

	// modification parameters
	float scale;
	float rotatexy, rotatexz;
	float scale_velocity;
	float rotate_velocity;
	float xy12rotate, xz12rotate;
	bool left_right_flipped;


	ARender(AMotion * aMotion, AEvent * aEvent,  AStruct * theStruct);
	~ARender(){Destroy();}

	void Destroy();

	void Init(AStruct * theStruct);

	void Render(); 

	//DrawUnit Specification
	//rect_dia: rectangle, diagonal entrance, inverse diagonal exist (lower left to upper right)
	//rect_ver: rectangle, vertical effect, (bottom-up entrance, up-bottom exist)
	void DrawUnit(); //square, triangle, polygon, circle

	void Effect();

	//helper methods
	void DrawDefault();

	//getter & setter
	void SetDrawUnit(DrawUnitShape aShape){theShape = aShape;}
	DrawUnitShape GetDrawUnit(){return theShape;}

	bool GetOverMotion(){return overMotion;} //if true, take control of the motion

};
