#pragma once
#include "Global.h"

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

/*
//Period Detection---------------------------------------------------------------
const int MiniInterval = 0.2 * soundEnergySampleRate; //in period checking
const int MaxInterval = 2 * soundEnergySampleRate; //in period checking
const int IntervalIncrement = 0.1*soundEnergySampleRate;
const int MaxPeriod = 2; //top N periods are stored
*/

//===============================================================================
//modularize the rhythmic extraction component
//===============================================================================
class RhythmicExtract {
private:
protected:
	//=================================================================
	// Rhythmic variables
	//=================================================================
	int numWaveformEntries;
	int numMaxChannels;
	int maxLowPassStorageSize;

	RenderVisualData renderData;
	bool nextValueReady;
	bool readyForEventUpdate;
	
	//Necessary! not yet implemented
	int pulseRate;
	float pulseDuration; //in seconds

	//=================================================================
	// Rhythmic variables
	//=================================================================
	unsigned int maxRawStorageEntries;
	deque<float> SoundEnergy; // Energy for then entire time storage interval
	deque<float>::reverse_iterator riterSoundEnergy;

	float numEntriesOfEachSoundEnergy; 
	deque<float> lowPassedSoundEnergy; 
	
	int lowpassCounter;
	int firstLowPassOrder;
	
	//min and max will be reset when change track
	float maxLowPassedSoundEnergy;
	float minLowPassedSoundEnergy;

	//sound energy distribution 
	//(this is useful to recognize genre when matched with a precompiled ruler together with Rhythmic perturbation)
	//float lowPassedSoundEnergyAxis[MaxDistributionLevel]; //from 0%(MinLowPassedSoundEnergy) evenly distribute levels to 100%(MaxLowPassedSoundEnergy)
	//float lowPassedSoundEnergyDistribution[MaxDistributionLevel]; //corresponding probability

	//Rhythmic preturbation (measure how much the energy fluctuate)
	float sizeOfLPSound[bufferNumberOfSecondLength];
	int initializedLevel; //this indicates which time duration interval has been filled (starting from 0, goes to bufferNumberOfSecondLength
	
	bool roughBeating[bufferNumberOfSecondLength], fineBeating[bufferNumberOfSecondLength];	//measure if currently on a beat
	int beatLastIter[bufferNumberOfSecondLength];
	
	//=================================================================
	//Rhythmic Analysis methods
	//=================================================================
	void RhythmicProcessingDrive();
	
	float CurrentSoundEnergy();
	void SoundEnergyLowPassFilter();
	void SoundEnergyStatisticAnalysis();
	void SoundEnergyBeatFirstExtraction();
	void SoundEnergyBeatSecondExtraction();
	float BeatThreshold(int BufferInd);

	/*
	//Rhythmic period (time between beats): achieved through dynamic pattern matching
	//can be achieved by finding the distance between max value iteratively (with consistancy constraint)
	int periodInMS[MaxPeriod];
	int periodInIter[MaxPeriod];
	float periodScore[MaxPeriod]; //corresponding scores of periodInMS[MaxPeriod];

	void SoundEnergyIterativePeriodicMatch();
	//helper method for period detection---------------------
	float IterativePeriodicMatchScoring(int iter);
	void InsertPeriod(int iter, float score);
	void ConvertToMSPeriod();
	//-------------------------------------------------------
	*/
	void RemoveTooOldEntries();

public:
	//=================================================================
	//Publicly available output variables
	//=================================================================
	//time domain statistics
	float averageLowPassedSound[bufferNumberOfSecondLength];
	float varianceOfLowPassedSound[bufferNumberOfSecondLength], standardDeviationOfLowPassedSound[bufferNumberOfSecondLength];

	float dynamicThresholdScale[bufferNumberOfSecondLength];

	float currentBeatMagnitudeArray[bufferNumberOfSecondLength];
	int currentBeatLevel; //measure how many entries in currentBeatMagnitudeArray is non-zero
	float currentBeatEnergy; //find the energy out of currentBeatMagnitudeArray
	
	deque<float> beatStorageDQue;
	deque<int>beatLevelDQue;

	float averageBeatLevel, averageBeatMagnitude;

	//=================================================================
	//Rhythmic Core methods
	//=================================================================
	RhythmicExtract(int NumWaveformEntries,	int NumMaxChannels);
	void Init();
	void Destroy(){EmptyRhythmicStates();}
	void Update(RenderVisualData * RenderData);
	
	void EmptyRhythmicStates();

	//Getter and Setter
		//return all raw information
	deque<float> * GetSoundEnergy(){return &SoundEnergy;}
	deque<float> * GetLowPassSoundEnergy(){return &lowPassedSoundEnergy;}

		//return the beat related
	deque<int> * GetBeatLevel(){return &beatLevelDQue;}
	deque<float> * GetBeatStorage(){return &beatStorageDQue;}
	
	int GetInitializedLevel(){return initializedLevel;}

		//return status
	bool GetNextEventUpdateReady(){ if(readyForEventUpdate){readyForEventUpdate = false; return true;} return false;}
};