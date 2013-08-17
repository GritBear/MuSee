/*
This class is the moving animation objects in contrast with the static class


*/
#pragma once
#include "Global.h"
#include "DataReader.h"

#if TARGET_OS_MAC
#include<gl/GL.h>
#include<gl/GLU.h>

#else
#include<Windows.h>
#include<WinGDI.h>
#include<gl/GL.h>
#include<gl/GLU.h>

#endif

class AnimationObjMove{
private:
	int ID;
	bool alive;
	bool stationary;
	bool collidable;
	bool usingTexture;
	bool toRender;
	int Index; // this specify the category index of this animation object
	bool coordinateNotInitialized;

protected:
	//time
	int startTimeInMS; //this is the animation spawing time

	// center coordinate in unit of percentage of the screen width and height
	double x, y;
	

	// modification parameters
	double scale;
	double rotate;
	double scale_velocity;
	double rotate_velocity;

	// velocity (if moving) in unit of percentage of the screen
	double velX;
	double velY;

	//*Note: all the velocities are in unit per MS !!

	// square collision bounds (used for rough collide detection)
	int boundX;
	int boundY;

	// sprite frame parameters
	int width;
	int height;
	int screen_w, screen_h;
	
	int sprite_frame;	//the frame out of the max frame the animation is on
	int maxFrame;		//maximum different picutures in the animation
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
	unsigned int textureOffset;
	GLuint texture;

	//---------------------------------------------------------------
	//DataReader
	//---------------------------------------------------------------
	DataReader *theReader;

public:
	double x1, y1, x2,  y2; // for quad corners or connection points 
	double x12, y12, x22, y22; // this is the coordinate values at the other two vertex, x12,y12 is the lower right and x22, y22 is the upper left
	//conversion to GLdouble units
	GLint X1,Y1,X2,Y2,X12,Y12,X22,Y22;
	double x12rotate;
	double Offset;
	double Ascale;

	//animation constructor support no input default construction
	AnimationObj(int AnimationIndex, DataReader *aReader);
	void virtual Destroy();

	//center has to be assigned
	void Init(double X, double Y, int StartingTimeInMS, double VelX = 0, double VelY = 0,  
		 double X1 = 0, double Y1 = 0, double X2 = 0, double Y2 = 0, int BoundX = 0, int BoundY= 0, int AnimationOrientation = 0,int FrameDelay = 0);

	//update position,scaling,and rotations based on velocities
	void virtual UpdateMovingVariables(int ElapsedTimeInMS);

	//draw the animation
	void virtual Render(int Screen_w, int Screen_h);

	void virtual DrawSquare(double tx1, double ty1, double tx2, double ty2, double tx12, double ty12, double tx22, double ty22);

	//=================================================================
	//Helper methods
	//=================================================================
	void virtual ComputeCoordinate();
	bool virtual SquareCheckCollisions(AnimationObj *otherObject);
	void virtual Collided(int objectID);
	bool Collidable(){return collidable && (alive || stationary);}


	//========================================================================================
	//Getter and Setter
	//========================================================================================
	double GetX(){return x;}
	double GetY(){return y;}

	void SetX(double X) {AnimationObj :: x = X;}
	void SetY(double Y) {y = Y;}

	int GetBoundX(){return boundX;}
	int GetBoundY(){return boundY;}

	int GetID() {return ID;}
	void SetID(int id) { ID = id;}

	bool GetStationary() {return stationary;}
	void SetStationary(bool Stationary) {AnimationObj::stationary = Stationary;}

	bool GetAlive() {return alive;}
	void SetAlive(bool Alive) {AnimationObj::alive = Alive;}

	double GetScale() {return scale;}
	void SetScale(double Sc) { scale = Sc;}

	double GetXVel(){return velX;}
	void SetXVel(double VelX){velX = VelX;}

	double GetRotate() {return rotate;}
	void GetRotate(double Rt) { rotate = Rt;}

	double GetScaleVelocity() {return scale_velocity;}
	void SetScaleVelocity(double ScV) { scale_velocity = ScV;}

	double GetRotateVelocity() {return rotate_velocity;}
	void GetRotateVelocity(double RtV) { rotate_velocity = RtV;}

	bool GetCollidable(){return collidable;}
	void SetCollidable(bool lCollidable) {collidable = lCollidable;}

	//Getters
	int GetInd(){return Index;}
	double GetRightMostPoint(){return x2;}
	int	GetMaxAnimationOrientation(){return maxAnimationOrientation;}
	int GetCurrentAnimationOrientation(){return animationOrientation;}
	int GetStartTime(){return startTimeInMS;}
	bool GetToRender(){return toRender;}
};

