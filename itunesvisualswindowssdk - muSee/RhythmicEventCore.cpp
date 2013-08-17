#include "RhythmicEventCore.h"

RhythmicEventCore::RhythmicEventCore(RhythmicExtract *TheRhythmicExtract){
	//the pointers must be initialized
	theRhythmicExtract = TheRhythmicExtract;

	currentEvent = &Event1;
	bufferEvent = &Event2;

	currentLogLoudness = 0;
	currentLevel = 0;
	numOfLogLoudness = 0;
	averageSongLoudness = 0;

	//clear the current event
	memset(currentEvent,0,sizeof(RhythmicEvent));
	memset(bufferEvent,0,sizeof(RhythmicEvent));

	SoundEnergy = TheRhythmicExtract->GetSoundEnergy();
	LowPassSoundEnergy = TheRhythmicExtract->GetLowPassSoundEnergy();
	beatLevel = TheRhythmicExtract->GetBeatLevel();
	beatMagnitude =  TheRhythmicExtract->GetBeatStorage();
}


void RhythmicEventCore::Destroy(){




}


void RhythmicEventCore::Update(){
	//information preparation
	if(theRhythmicExtract->GetNextEventUpdateReady()){
		LogSoundEnergy(LowPassSoundEnergy, LogLowPassSoundEnergy);

		//analyzing
		UpdateCurrentLevel();
		SmoothnessDrive();
		RemoveTooOldEntries();

		//set the switch boolean to true
		currentEvent->updated = true;
		SwapEventBuffer();
	}
}

//swap buffers
void RhythmicEventCore::SwapEventBuffer(){
	RhythmicEvent * temp = currentEvent;
	//clear buffer
	memset(bufferEvent, 0, sizeof(RhythmicEvent));
	currentEvent = bufferEvent;
	bufferEvent = temp;
}


//Analyzing methods
void RhythmicEventCore::UpdateCurrentLevel(){
	//fill in the currentEvent->beatLevel
	currentLevel = 0;
	for(int i = 0; i < theRhythmicExtract->GetInitializedLevel(); i++){
		if(theRhythmicExtract->currentBeatMagnitudeArray[i]!=0){
			currentLevel++;
		}
	}

	currentEvent->beatLevel = currentLevel;
}

//Produce all necessary information for genera detection
void RhythmicEventCore::SmoothnessDrive(){
	SpeedAnalyze();
	LoudnessAnalyze();
	PatternAnalyze();
}

void RhythmicEventCore::SpeedAnalyze(){
	//simple iterative checking
	//there are soundEnergySampleRate rhythmic sample per second
	//rhythmic increwment interval: (should not be a constant)
	//tolerance interval:
	//total searching interval:
	//this will be able to find T-period by maximum average hit rate






}


void RhythmicEventCore::LoudnessAnalyze(){



}


void RhythmicEventCore::PatternAnalyze(){




}

//--------------------------------------------------------------------------------------
//push the log last entry of the source queue into the target queue
//the target queue size can not exceed the source queue
//--------------------------------------------------------------------------------------
void RhythmicEventCore::LogSoundEnergy(deque<float> * SourceQueue, deque<float> &targetQueue){
	int sizeSource = SourceQueue->size();
	if(sizeSource > 0){
		float lastEntry = log(abs(SourceQueue->back()) + LOGZERO_OFFSET); //some error prevention mechanism
		targetQueue.push_back(lastEntry);

		currentLogLoudness = lastEntry;
		numOfLogLoudness++;

		if(targetQueue.size() > sizeSource)
			targetQueue.pop_front();
	}

	//compute average song energy
	if(numOfLogLoudness > 0){
		float percentageInflucent = 1.0/float(numOfLogLoudness);
		averageSongLoudness = averageSongLoudness*(1 - percentageInflucent) + percentageInflucent * currentLogLoudness;
	}else{
		numOfLogLoudness = 150;
	}
}

//--------------------------------------------------------------------------------------
//Calibration

//-------------------------------------------------------------------------------------
void RhythmicEventCore::CalibrateInterestedPeriods(){
//input:	float InterestedPeriod[], numInterestedPeriod
//			soundEnergySampleRate			
//		
//Outputs: 	int interestedPeriods[numInterestedPeriod];
//			int interestedPeriodsTolerance[numInterestedPeriod];



}


void RhythmicEventCore::RemoveTooOldEntries(){



}

void RhythmicEventCore::EmptyStates(){
	currentLogLoudness = 0;
	currentLevel = 0;
	numOfLogLoudness = 0;
	averageSongLoudness = 0;

}