//Author: Li(Lee) Xiong. All rights reserved.
#pragma once
#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <math.h>
#include <sstream>
#include <queue>
//#include <hash_map>
#include <map>

//Including all the subGlobal parameter files
#include "iTunesVisualAPI.h"
#include "AnimationGraphicsGlobal.h"
#include "GLtexture.h"
#include "Data.h"
#include "MelodyExtractionParameters.h"
#include "MelodyObjParameters.h"
#include "iTunesSystemDefinition.h"
#include "ObjSpeedCore.h"
#include "BackgroundData.h"
#include "Colors.h"
#include "VectorFunctions.h"

using namespace std;
enum AState{NoState, Enter, Exit, Dead};

//----------------------------------------------------------------------------
// Visualizer Essential Parameter
//----------------------------------------------------------------------------
const int UpdateMessagePerSecond = 60;
const int RenderMessagePerSecond = 20;

// Constant Values
#define pi 3.14159265358979323846
#define halfpi 1.57079632679489661923
const float INF = 999999999999999e10;


//----------------------------------------------------------------------------
// Rate Parameters
//----------------------------------------------------------------------------
const int MP3SamplingRate = 44100;
const int ExtractRate = 6;
const float TimeStep = 1000.0 /((float)kPlayingPulseRateInHz); //In MS

const float TimeStepInSecond = 1.0/((float)kPlayingPulseRateInHz);

//----------------------------------------------------------------------------
// Storage Constant
//----------------------------------------------------------------------------
const unsigned int numberOfSecondsStored = 12; //seconds
const unsigned int maxExtractedStorageEntries = numberOfSecondsStored * ExtractRate;
const unsigned int maxCurTone = 100; // arbitrarily large to avoid further complication
const float	leftMostXValueToRenderAnimationObj = -2.6;
const float	leftMostXValueToStoreAnimationObj = -3.5; //N screens away

const int maxDirectorySize = MAX_PATH;


//----------------------------------------------------------------------------
// OpenGL
//----------------------------------------------------------------------------
const GLfloat	farthestVisibleDistance = 10.0;
const GLfloat	defaultVerticalAnimationDistance = farthestVisibleDistance * 0.9;
const GLfloat	defaultAnimationObjDistance = 0.0;
const GLfloat	farthestFrontDistance = 10.0;
const GLfloat	defaultSpecialEffectDepth = 2.0;


//----------------------------------------------------------------------------
// Independent Functions
//----------------------------------------------------------------------------
string ExePath();
double str2double(string valuestr);
string num2str(float number);

//rotation translation functions
void PointRotation(float &x1, float &y1, float centerX,float centerY, float thetaClockwise);
void PointRotation(float &x1, float &y1, float centerX,float centerY, float sinThetaClockwise, float cosThetaClockwise);
void RotationTranslation(float &x1, float &y1, float &x2, float &y2, float &x12, float &y12, float &x22, float &y22, float thetaClockwise);
void RotationTranslationWithDiagonal(float &x1, float &y1, float &x2, float &y2, float &x12, float &y12, float &x22, float &y22, float Length, float Width, float thetaClockwise);
float PercentageLength(float origin, float target, float percentage);


//Memory free function for containers with swap() method
template <typename T>
void FreeAll( T & t ) {
    T tmp;
    t.swap( tmp );
}

