//Object Animation
/*This object class contains is the base class for all the animation objects except backgrounds*/


#pragma once
#include "Global.h"
#include "DataReader.h"
#include "control.h"

typedef struct                                  // Create A Structure
{
	//position coordinate
	float X;
	float Y;

	float X1;
	float Y1;
	float X2;
	float Y2;
	
	//velocity
	float XVel;
	float YVel;
	float Rotation;
	
	float BoundX;
	float BoundY;
	float Radius;

	int StartingTimeInMS;
	int AnimationOrientation;
	int FrameDelay;

} AnimationObjStruct;                                 // Structure Name

class AnimationObj{
protected:
	int ID;
	int entranceEffectID;
	int animationTypeID;

	bool alive;
	bool stationary;
	bool collidable;
	bool usingTexture;
	bool toRender;
	int Index; // this specify the category index of this animation object
	bool useEntranceEffextSprite;
	bool usingOrientation;
	GLfloat depth;

	//some multi-purpose state variable------------------------------------------------
	bool state1;
	int state2;
	float state3;
	//-------------------------------------------------------------------

	//time
	int startTimeInMS; //this is the animation spawing time
	float timeStep; //in MS
	// center coordinate in unit of percentage of the screen width and height
	float x, y;
	//conversion to GLfloat units
	float X1,Y1,X2,Y2,X12,Y12,X22,Y22;
	float x1, y1, x2, y2; // for quad corners or connection points 
	float x12, y12, x22, y22; // this is the coordinate values at the other two vertex, x12,y12 is the lower right and x22, y22 is the upper left
	float collisionRadius; //used for center distance collision check
	GLfloat tx1, ty1, tx2, ty2, tx12, ty12, tx22, ty22; //texture coordinates
	float bottom,top,left,right;

	// modification parameters
	float scale;
	float rotate;
	float scale_velocity;
	float rotate_velocity;
	float x12rotate;
	// velocity (if moving) in unit of percentage of the screen
	float xVel, yVel;
	float xVelPulse, yVelPulse;

	//*Note: all the velocities are in unit per MS !!

	// square collision bounds (used for rough collide detection)
	float boundX;
	float boundY;

	// sprite frame parameters
	int width;
	int height;
	float screen_w, screen_h;

	int sprite_frame, entranceEffextSpriteFrame;	//the frame out of the max frame the animation is on
	int maxFrame, maxEntranceEffextSpriteFrame;		//maximum different picutures in the animation
	int curFrame;		//this include the delay and is used to calculate sprite_frame
	int frameDelay;		//this will determine the time duration
	int animationColumns;  // default equal to maxFrame

	/* By Default
		animationOrientation = 0 : unfliped (default)
	    animationOrientation = 1 : fliped with respect to x axis f(y) = g(-y); y_scale = -scale;
		But this can also goes above 1 given higer maxAnimationOrientation
	*/
	int animationOrientation;
	int maxAnimationOrientation; //default is 2

	//blending texture (animation picture)
	GLuint textureID;

	//---------------------------------------------------------------
	//control
	control *theControl;

	DataReader *theReader;
	CreationCore *theCreationCore;
	SpeedControl *theSpeedControl;
	//Important parameter assignment methods
	void InitAssign(int AnimationIndex, control *TheControl, TextureType AnimationTypeID = AnimationTexture); // called in constructors
	void InitConst(); //called in Init()
public:
	float DebugValue,DebugValue2;
	//animation constructor support no input default construction
	AnimationObj(){};
	AnimationObj(int AnimationIndex, control *TheControl, TextureType AnimationTypeID = AnimationTexture);

	~AnimationObj(){Destroy();};

	//================================================================
	//Virtual methods
	virtual void Destroy(){};
	
	//initialize the animation obj parameters
	virtual void Init(AnimationObjStruct *AniStruct);

	//update position,scaling,and rotations based on velocities
	virtual void  Update();

	//draw the animation
	virtual void  Render(float Screen_w = 0, float Screen_h = 0, bool updating = true);

	//compute the 4 corner coordinate of a square
	void virtual ComputeCoordinate();

	virtual void  Collided(AnimationObj *otherObject){}
	virtual void  EntraceEffectPercentageCoor();
	virtual bool GetAlive() {return alive;}
	virtual float  GetRightMostPoint(){return x2;}
	virtual bool GetToRender(){return toRender && alive;}
	//-----------------------------------------------------------------

	//=================================================================
	//Helper methods
	//=================================================================
	void  DrawSquare();
	void FunctionDraw();
	
	bool SquareCheckCollisions(AnimationObj *otherObject);
	bool RoundCheckCollishions(AnimationObj *otherObject);

	bool CheckCollisionTopBottomScreen(float ScreenH = 0);
	bool CheckCollisionLeftRightScreen(float ScreenW = 0);
	
	//========================================================================================
	//Getter and Setter
	//========================================================================================
	float GetX(){return x;}
	float GetY(){return y;}

	void SetX(float X) {AnimationObj :: x = X;}
	void SetY(float Y) {y = Y;}

	int GetBoundX(){return boundX;}
	int GetBoundY(){return boundY;}

	int GetID() {return ID;}
	void SetID(int id) { ID = id;}

	bool GetStationary() {return stationary;}
	void SetStationary(bool Stationary) {AnimationObj::stationary = Stationary;}

	void SetAlive(bool Alive) {AnimationObj::alive = Alive;}

	float GetScale() {return scale;}
	void SetScale(float Sc) { scale = Sc;}

	float GetXVel(){return xVel;}
	void SetXVel(float XVel){xVel = XVel;}

	float GetYVel(){return yVel;}
	void SetYVel(float YVel){yVel = YVel;}

	float GetRotate() {return rotate/TimeStepInSecond;}
	void SetRotate(float Rt) { rotate = Rt * TimeStepInSecond;}

	float GetScaleVelocity() {return scale_velocity;}
	void SetScaleVelocity(float ScV) { scale_velocity = ScV;}

	float GetRotateVelocity() {return rotate_velocity;}
	void GetRotateVelocity(float RtV) { rotate_velocity = RtV;}

	bool GetCollidable(){return collidable && (alive || stationary);}
	void SetCollidable(bool Collidable) {collidable = Collidable;}

	void SetDepth(GLfloat DEPTH){depth = DEPTH;}

	void SetMaxDiagonalFrame(int maxDFrame) {maxEntranceEffextSpriteFrame = maxDFrame;}
	
	void SetCollisionRadius(float CollisionRadius){collisionRadius = CollisionRadius;}
	float GetCollisionRadius(){return collisionRadius;}
	
	//Getters
	int GetInd(){return Index;}
	int	GetMaxAnimationOrientation(){return maxAnimationOrientation;}
	int GetCurrentAnimationOrientation(){return animationOrientation;}
	int GetStartTime(){return startTimeInMS;}
	int GetTextureID(){return (int)textureID;}
	void GetFourCorners(float &X1, float &Y1, float &X12, float &Y12, float &X2, float &Y2,float &X22, float &Y22){
		X1 = x1 ; X2 = x2 ;Y1 = y1 ;Y2 = y2; X12 = x12; Y12 = y12 ; X22 = x22 ; Y22 = y22 ;
	}

	void GetStateVariable(bool &State1, int &State2, float &State3){ State1 = state1; State2 = state2; State3 = state3;};
	void SetStateVariable(bool State1, int State2, float State3){state1 = State1; state2 = State2; state3 = State3;}



	//====================================================================================================
	//Depreciated methods
	void Init(float X, float Y, int StartingTimeInMS, float XVel = 0, float YVel = 0,  
		float X1 = 0, float Y1 = 0, float X2 = 0, float Y2 = 0, float BoundX = 0, float BoundY= 0, int AnimationOrientation = 0,int FrameDelay = 0);

	//center has to be assigned
	void simplified_Init(float X1 = 0, float Y1 = 0, float X2 = 0, float Y2 = 0, float XVel = 0, float YVel = 0, float BoundX = 0, float BoundY= 0,int FrameDelay = 0, int AnimationOrientation = 0);
	//-----------------------------------------------------------------------------------------------------
};
