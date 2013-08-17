/*VerticalAnimationObj
	these vertical animation objects are spawned by background
	they are not necessary vertical, but most of the time they will be vertical
*/

#pragma once
#include"AnimationObj.h"

//Define the struct to store openGL texture
typedef struct                                  // Create A Structure
{	//position parameters
	float CenterBottomX;
	float CenterBottomY; 
	float CenterTopX;
	float CenterTopY;

	//velocities
	float XVel;
	float YVel;

	//other parameters
	int StartingTimeInMS;
	int AnimationOrientation;
	int FrameDelay;
	
	float	Depth;

} VerticalAnimationStruct;                                 // Structure Name


class VerticalAnimationObj : public AnimationObj{
private:
	bool preDrawn;
	bool usingVerticalAnimation;
	bool selfMotion;
	VerticalAnimationStruct *theVerticalAnimationStruct;
protected:
	float centerBottomX, centerBottomY, centerTopX, centerTopY;
	float centerBottomXVel, centerBottomYVel, centerTopXVel, centerTopYVel;

	//this cumulation is only effective for self motion
	float cumulativeBottomXMotion, cumulativeBottomYMotion, cumulativeTopXMotion, cumulativeTopYMotion;
	
public:
	//Modified Core Functions
	VerticalAnimationObj(int VerticalAnimationIndex, control *TheControl,  bool SelfMotion = false, bool PreDrawn = false);
	~VerticalAnimationObj(){Destroy();}

	//void Destroy(){free(theVerticalAnimationStruct); AnimationObj::Destroy();}

	void Destroy(){AnimationObj::Destroy();}

	void Init(VerticalAnimationStruct *aVerticalAnimationStruct);

	void Update();

	//draw the animation
	void Render(float Screen_w, float Screen_h, bool updating = true){
	//draw the animation
		AnimationObj::Render(Screen_w, Screen_h, updating);
	}

	void ComputeCoordinate();

	void DrawSquare();

	bool GetToRender(){return AnimationObj::GetAlive();}
	bool GetAlive(){return AnimationObj::GetAlive();}

	//Modified Helpers
	float GetRightMostPoint(){return max(x2, x12);}

	//Vertical Animation Special Functions
	void CenterPosisitonUpdate(bool OutsideUpdate = true, float CenterBottomXVel = 0,float CenterBottomYVel = 0,float CenterTopXVel = 0,float CenterTopYVel = 0);


	
	//====================================================================================================================
	//Depreciated methods
	void Init(float CenterBottomX, float CenterBottomY, float CenterTopX, float CenterTopY, float XVel, float YVel, int StartingTimeInMS, int AnimationOrientation, int FrameDelay);
	//-------------------------------------------------------------------------------------------------------------------
};