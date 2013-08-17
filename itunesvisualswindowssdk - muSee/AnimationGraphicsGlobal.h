/*
	This header file is for information of animation spawning, velocity, location, speed, datareader and datawriter
	It also specify some boundaries, default, initial values and centers

	(this file is included in Global.h)
*/

#pragma once
#include <list>
#include <vector>
using namespace std;

#define NotAnAngle	-1000

const float defaultXatCurrentTime = 0.8;					//the left side is 0 and the right side is 1
const int	defaultBottomTone = 10;							//the default lowest tone to display for one melody line
const int	defaultTopTone = 50;							//the default highest tone to display for one melody line
const int	defaultLowerThresholdToneForStartTone = defaultBottomTone - 2;

const float	defaultMinDiagLength = 0.12;					//default minimum diagonal length in pixels if reading data unsuccessful
const float	defaultMaxDiagLength = 0.18;					//default maximum diagonal length in pixels if reading data unsuccessful
const int	defaultMaxTimeInMS = 2000;
const int	defaultMinTimeInMS = 500;

const int	defaultFrameWidth = 256;
const int	defaultFrameHeight = 256;

const int	defaultMinDelayFrame = 1;
const int	defaultMaxDelayFrame = 2;

const int	defaultAnimationColumns = 1;
const int	defaultMaxFrame = 10;

const int	defaultMaxAnimationOrientation = 2;
const float	defaultXBound = 0;
const float	defaultYBound = 0;
const bool	defaultCollidable = false;
const int	defaultFrameDelay = 1;

const int defaultSpawnAngleThreshold = 30;			//in degrees

enum CoorPosition {bottomLeft, topLeft,topRight, bottomRight, headCenter, noAnchor};
