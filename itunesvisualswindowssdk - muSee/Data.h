//=====================================================================================
//Values to be read by DataReader
//provide build-in values not needed from reading encrypted txt
//=====================================================================================
#pragma once
enum TextureTypes{leaf, MaxPreCompiledAniType};

const float	abstractLeefMinDiagLength = 0.6;					//abstractLeef minimum diagonal length in pixels if reading data unsuccessful
const float	abstractLeefMaxDiagLength = 0.18;					//abstractLeef maximum diagonal length in pixels if reading data unsuccessful
const int	abstractLeefMaxTimeInMS = 1500;
const int	abstractLeefMinTimeInMS = 300;

const int	abstractLeefMinDelayFrame = 0;
const int	abstractLeefMaxDelayFrame = 1;

const int	abstractLeefAnimationColumns = 1;

const int	abstractLeefMaxFrame = 20;
const int	abstractLeefMaxAnimationOrientation = 2;
const float	abstractLeefXBound = 0;
const float	abstractLeefYBound = 0;
const bool	abstractLeefCollidable = false;

const int abstractLeefUpSpawnAngleThreshold = 80;				//in degrees
const int abstractLeefDownSpawnAngleThreshold = -80;			//in degrees




