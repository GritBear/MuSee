/*	This module is a layer between CreationCore and RhythmicExtract:
		1. this beat is a regular minor beat
		2. this beat is a regular major beat
		3. this beat is exceptional major beat

		4. major pattern changed (into another paragraph)
		5. speed increased/decreased

		6. it is a smooth song
		7. it is an exciting song
*/

#pragma once
#include "Global.h"
#include "RhythmicExtract.h"
// define necesssary constants and struct
// this is all that creation core needed
const float minSmoothness = 0;
const float maxSmoothness = 100;

const float minPatternChange = 0;
const float maxPatternChange = 100;

const int minBeatLevel = 1;
const int maxBeatlevel = bufferNumberOfSecondLength;

struct RhythmicEvent{
	int beatLevel; //0->no beat; 1 regular minor; 2, regular major; 3, relative major; 4, exceptional major
	
	float speedSmoothness; //minSmoothness, slow; maxSmoothness, fast; (threshold very necessary, this is used for genre classification)
	float loudnessSmoothness; //minSmoothness, quiet; maxSmoothness, loud;
	float patternSmoothness; //minSmoothness, quiet; maxSmoothness, loud;

	float patternChange; //minPatternChange -> no change, maxPatternChange -> exceptionally major change
	float speedChangeFactor; // always positive. less than 1: slow down; more than 1, speed up
	
	//this is a switch used to indicate is a new value is available
	bool updated;
};

const float InterestedTimeInterval[] = {3,15,60};//in seconds
const int NumInterstedTimeInterval = sizeof(InterestedTimeInterval)/sizeof(float);

#define LOGZERO_OFFSET 0.001
#define LOGZERO  -6.90775527898213705205397405 //log(LOGZERO_OFFSET)
const float InterestedPeriod[] = {1.0/40.0, 1.0/50.0, 1.0/ 60.0, 1.0 / 66.0, 1.0 / 74.0, 1.0/80.0, 
									1.0/90.0, 1.0/100.0, 1.0/120.0, 1.0 / 144.0, 1.0 / 160.0, 1.0/200.0};
const int numInterestedPeriod = sizeof(InterestedPeriod) / sizeof(float);

// define the rhythmic even core
static class RhythmicEventCore {
private:
protected:
	RhythmicExtract *theRhythmicExtract;
	RhythmicEvent * currentEvent;
	RhythmicEvent * bufferEvent;

	RhythmicEvent Event1, Event2;

	deque<float> *SoundEnergy;
	deque<float> *LowPassSoundEnergy;
	//the following will be zero if not currently a beat
	deque<int> * beatLevel;
	deque<float> * beatMagnitude;

	int currentLevel;
	float currentLogLoudness;
	deque<float> LogLowPassSoundEnergy;

	//analying parameters (this should be zeroed once sound track changes)
	unsigned int numOfLogLoudness;
	float averageSongLoudness;
	
	//calibration: convert into matrix
	int interestedPeriods[numInterestedPeriod];
	int interestedPeriodsTolerance[numInterestedPeriod];

	void CalibrateInterestedPeriods();
	
public:
	//core functions
	RhythmicEventCore(RhythmicExtract *TheRhythmicExtract);
	void Destroy();
	void Update();
	void SwapEventBuffer();

	//Analyzing methods
	void UpdateCurrentLevel();
	void SmoothnessDrive();
	void SpeedAnalyze();
	void LoudnessAnalyze();
	void PatternAnalyze();
	void LogSoundEnergy(deque<float> * SourceQueue, deque<float> &targetQueue); //commens in the cpp file
	
	//Maintenance methods
	void RemoveTooOldEntries();
	void EmptyStates();

	//Getter and Setter
	RhythmicEvent * GetCurrentEvent(){return bufferEvent;}
	float GetCurrentLogLoudness(){return currentLogLoudness;}
	int GetCurrentLevel(){return currentLevel;}
	float GetAverageSongLogLoudness(){return averageSongLoudness;}
};