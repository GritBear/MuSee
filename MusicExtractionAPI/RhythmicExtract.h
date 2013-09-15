#pragma once
#include <deque>
#include "Parameters.h"
using namespace std;

/*
//Period Detection---------------------------------------------------------------
const int MiniInterval = 0.2 * soundEnergySampleRate; //in period checking
const int MaxInterval = 2 * soundEnergySampleRate; //in period checking
const int IntervalIncrement = 0.1*soundEnergySampleRate;
const int MaxPeriod = 2; //top N periods are stored
*/

#include "MuseeInterfaceParam.h"


//===============================================================================
//modularize the rhythmic extraction component
//===============================================================================
class RhythmicExtract {
private:
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

	void Clean();

public:
	//=================================================================
	//Rhythmic Core methods
	//=================================================================
	RhythmicExtract(MelodyExtractionPram * param);
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
};