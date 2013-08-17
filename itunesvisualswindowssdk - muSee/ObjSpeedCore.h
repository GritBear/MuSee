#pragma once
#include <math.h>
#include <time.h>
#include "VectorFunctions.h"

//This file contains speed constands for ObjSpeedCore
const int	defaultDisplayTimeInMS = 12 * 1000;		//this do not has to less than the length of stored melodies. Useful to calculate default x moving speed

//Precompiled Background info
const float defaultPercentageVel = 0.0002;
const float defaultXSpan = 0.9;
const float defaultYSpan = 1;
const float maxTimeStep = 1.0/15.0; //bigger time step will be discarded

//pulse rate
#define kPlayingPulseRateInHz		65						// when iTunes is playing, draw N times a second
#define kStoppedPulseRateInHz		2							// when iTunes is not playing, draw N times a second

//this core include X speed functions for all animationObj and Background
/*
	Necessary Functions
	1: Melody Animation Speed
	2: Background Speed
	3: Background Vertical Animation Speed
*/

float MelodyAnimationObjSpeed();
float BackgroundSpeed(int UpdatePerSecond, float BackgroundXSpan, float OpenGLXRange);
float BackGroundVerticalAnimationObjSpeed();

struct SpeedHolder {
	bool * lockptr;
	float scale;

	bool useExponential;

	float enterAcceleration;	//per second
	float exitAcceleration;		//per second

	float enterExp;				//coefficient in the exponent
	float exitExp;				//coefficient in the exponent
};

enum SpeedActiveTime {Playing, Rendering, Both};
//it normalize the render/update time intervals for all animation objects
class SpeedControl {
private: //all units are in seconds

protected:
	clock_t t;
	int fps, ftpTemp;
	float fps_t;

	float time_elapsed;	// in seconds
	float time_visualsound_elapsed;
	float time_visual_elapsed;

	//pointers to visual and sound
	bool *visualPlaying;
	bool *soundPlaying;

	float time_total;			// total time since the visualizer started (in seconds)
	float time_visualsound;		// time that the player is playing both song and visual
	float time_visual;			// time that the player is playing visual;

	float speedScaler;			//scale the speed, usually it eqauls one
	float timeScaled;					// = speedScaler * time_elapsed
	float timeVisualScaled;				// = speedScaler * time_visual_elapsed
	float timeVisualSoundScaled;		// = speedScaler * time_visualsound_elapsed

	//theme background speed
	float mainSpeed;
	float normalizedCurSpeed;

	Vector3 noramlizedScreenSpeed;
public:
	//public variables
	//screen speed
	Vector3 screenSpeed;

	SpeedControl(){Init();}
	void Init();
	void Update(); //update the times

	//float SpeedConversion(float SpeedPerSecond){return SpeedPerSecond/(*fps_ptr);}
	float SpeedNormalization(float SpeedPerSecond, SpeedActiveTime amode = Both);

	//initiate the speed scaler hold
	void SpeedScaleHold(SpeedHolder aHolder);

	void Reset();

	//Getter Setter
	void SetVisualSoundBoolPtr(bool * Visualplaying, bool * Soundplaying){visualPlaying = Visualplaying; soundPlaying = Soundplaying;}

	Vector3 * GetNormalizedScreenSpeed(){return &noramlizedScreenSpeed;}

	int GetFPS(){return fps;}
	float GetTimeElapsed(){return time_elapsed;}
	float GetTotalTime(){return time_total;}

	float GetTimeVisualSound(){return time_visualsound;}
	float GetTimeVisual(){return time_visual;}

	float GetSpeedScale(){return speedScaler;}
	void  SetSpeedScale(float scale){speedScaler = scale;}

	float GetNormalizedMainSpeed(){return normalizedCurSpeed;}
	float GetMainSpeed(){return mainSpeed;}
};