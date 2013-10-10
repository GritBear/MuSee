#pragma once
#include"Global.h"
#include"DataReader.h"
#include"CreationCore.h"
#include"VerticalAnimationObj.h"
#include"FlyingAnimationObj.h"
#include"AFactory.h"

//#include"SpecialEffectController.h"
#include "control.h"

//testing Modules
#include "AllAObj.h"


/* Background class:
	This class renders the background and determine the current theme (directly influencing the spawning probability of next animation obj), 
	like animation obj, it also has 3 models.

	It is responsible for the drawing of vertical animation objects
*/

const float BackgrounOpenGLLeft = -0.5;
const float BackgrounOpenGLRight = 1.5;
const float OpenGLXSpan = BackgrounOpenGLRight - BackgrounOpenGLLeft;
const int maxFlyingObj = 5;

class Background{
private:
	bool readingSuccessful; //if false, use model 1, if true, use model 3S
	bool onlyUsingPrecompiledAnimationVariables; //if true, use model 2
	int numFlyingObj;

protected:
	int ThemeID;
	float percentageVel, XSpan, YSpan, XVel, YVel;
	float XVelUpdate,YVelUpdate;

	int width, height;
	bool usingTexture;
	unsigned int curTimeInMS;
	float timeStep;
	GLuint textureID; //background texture

	float tX_left, tY_bottom, tX_right, tY_top;

	//construct default gradient color background
	GLfloat red_top, green_top, blue_top, alpha_top;
	GLfloat red_bottom, green_bottom, blue_bottom, alpha_bottom;

	GLfloat maxRed, maxGreen, maxBlue, minRed, minGreen, minBlue;
	GLfloat redVel_top, greenVel_top, blueVel_top, redVel_bottom, greenVel_bottom, blueVel_bottom;
	float velRange;

	//----------------------------Vertical Animation Object Related------------------------------------
	AFactory backgroundAnimationFac;
	
	int minTime, maxTime, meanTime;
	float minHeight, maxHeight, meanHeight, nextHeight;
	float minAngle, maxAngle, meanAngle;
	float nextAngle;

	VerticalAnimationStruct aVerticalStruct;

	bool animationIndexAlreadyChose;
	vector<float>probabilityVect;

	int targetAnimationIndex;
	bool selfMotion, preDrawn;
	bool usingVerticalAnimation;
	int previousVerticalAnimationSpawningTime;

	float centerBottomX, centerBottomY, centerTopX, centerTopY;//location variable for 

	// animation spawning velocity in unit of percentage of the screen
	float xVel;
	float yVel;

	// animation bound
	float aniXBound;
	float aniYBound;
	
	// animation orientation
	int animationOrientation, perviousAnimationOrientation;
	int time_durarion_frame_delay; // pass to animation object

	//random spawning related
	int nextTime;
	bool parameterGenerated;

	//screen dimension
	int screen_w, screen_h;  //in pixels

	//animation parameter
	float percentageInTheTheme;

	//probability
	vector<float>BackgroundProbability;

	//--------------------------FlyingAnimationTexture Animation Related-----------------------------------------
	list<AnimationObj *> flyingAniObjList;
	list<AnimationObj *>::iterator flyingAniObjIter;

	list<AnimationObj *> AnimationObjList; 
	list<AnimationObj *>::iterator AnimationObjListIter;
	list<AnimationObj *>::reverse_iterator RAnimationObjListIter;
	//-------------------------------------------------------------------------------------------
	//Control components
	ControlObj * theControlObj;
	control * theControl;

	DataReader * theReader;
	CreationCore *theCreationCore;
	SpeedControl *theSpeedControl;

	//one sequential and one all active
	AniDispatcher * theChildren;
	AniDispatcher * theAllActiveChildren;
	deque<ATreeBranch*> theBranches;

public:
	//debug
	bool testGenerated;
	int colorInst;
	
	//=========================================================================================
	//Core functions
	//=========================================================================================
	bool alreadyInitialized;

	Background(control * TheControl);

	void Destroy();

	void Init();

	void Update();

	void Render(bool updating);

	void UpdateThemeParameters();
	
	void NextAnmiationInfo();
	void NextAnmiationInfo(IndexThemeControl * theIndexStruct);

	void NexAnimationObjGenerate();

	bool BestMatchParameters(int AnimationIndex);
	//=========================================================================================
	//Helper Methods
	//=========================================================================================
	float GenerateNextVelocity(float previousVel);
	void SetCurTimeInMS(int TIME){curTimeInMS = TIME;}
	//=========================================================================================
	//Getter and Setter functions
	//=========================================================================================
	int GetThemeID(){return ThemeID;}
	float GetPercentageInTheTheme(){return percentageInTheTheme;}
	bool GetAlreadyInitialized(){return alreadyInitialized;}
	
	void RemoveTooOldEntries(bool Removeall = false);

	//legacy vertical animation code
	//void legacyVertical();
	//bool legacyBestMatchParameters(int AnimationIndex); 
};

//=====================================================================================
//Values for default Background
//=====================================================================================

const float backgroundMaxRed = 200.0/255.0;
const float backgroundMaxGreen =  235.0/255.0;
const float backgroundMaxBlue = 215.0/255.0;
const float backgroundMinRed = 85.0/255.0;
const float backgroundMinGreen = 120.0/255.0;
const float backgroundMinBlue = 100.0/255.0;

const float minColorChange = 0.0005;
const float maxColorChange = 0.002;
