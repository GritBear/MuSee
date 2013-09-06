#pragma once
//-------------------------------------------------------------------------
//Storage Variables
//-------------------------------------------------------------------------
const unsigned int numberOfSecondsStored = 12; //seconds

//-------------------------------------------------------------------------
//Band Separation
//-------------------------------------------------------------------------
//Define Central C
const float C4 = 261.625565300599;
const float toneResolution = 1;
const float minimumTone = -20;
const int MP3SamplingRate = 44100;

//a very detailed band seperation sample parameters
//const unsigned int NumFrequencyBand = 5;
//const float FrequencySeparationPoint[NumFrequencyBand] = {100, 260, 520, 900, 1200};

//current band seperation parameters
const unsigned int NumFrequencyBand = 3;
const float FrequencySeparationPoint[NumFrequencyBand] = {100, 300, 1200};

//melody band separation: above this tone is the meaningful melody
const int bassMelodyDevideTone = 0;

//-------------------------------------------------------------------------
//Melody Processing
//-------------------------------------------------------------------------
//Power Normalizing
const int NormalizationPower = 3;
const float MaxAfterNormalizedMatrixValue = 10.0;
//Gaussian Smoothing
const int sizeGaussianMatrix = 3;
// this is modified gaussian matrix
const float filterMatrix[3][3] = { {  0.1,    0.2,    0.1 }, 
									 { 0.6065,    1.0000,    0.6065 }, 
									 {  0.1,    0.2,    0.1 }};
//Hard Decision Threshold
const float valueThreshold = MaxAfterNormalizedMatrixValue * 0.8;
const float valueBottom = 0;
const float valueTop = MaxAfterNormalizedMatrixValue;
//dynamic adjustments
const int dynamicPointsMinimum = NumFrequencyBand - 1;
const int dynamicPointsMaximum = NumFrequencyBand + 1;
const float dynamicIncreaseFactor = 0.8; //both factor has to be less than 1
const float dynamicDecreaseFactor = 0.9;

//frequency scaling to compensate for high frequencies
const float endScale = 4.0;
const float startScale = 1.0;

