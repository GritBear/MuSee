#pragma once

struct RenderVisualData {
	unsigned __int8		numWaveformChannels;
	unsigned __int8 **	waveformData;
	
	unsigned __int8		numSpectrumChannels;
	unsigned __int8	**	spectrumData;
};

struct MelodyExtractionPram {
	int melodyExtractRate;   //extract melody N times a second
	int pulseFreq; //N update per second
	int numSpectrumEntries;
	int numWaveformEntries;
	int numMaxChannels;
};


//===============================================================================
//Define constants
//===============================================================================
const int soundEnergySampleRate = 15; //n samples per second for low passed sound
const int MaxDistributionLevel = 11; //from 0% evenly distribute levels to 100%

//Beat Detection-----------------------------------------------------------------
const float bufferNumberOfSecond[] = {0.6, 1.5, 5}; //seconds (this does not include the longest possible buffer length)
const int bufferNumberOfSecondLength = sizeof(bufferNumberOfSecond)/sizeof(float);
const float beatThresholdStandardDeviationScale = 0.6;//to determine the beat threshold
const int minBeatIter = 1; //the minimal tap a beat that must last to be a fine beat (1 means same as rough beat)


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


//-----------------------------------------------------
//Memory free function for containers with swap() method
template <typename T>
void FreeAll( T & t ) {
    T tmp;
    t.swap( tmp );
}
