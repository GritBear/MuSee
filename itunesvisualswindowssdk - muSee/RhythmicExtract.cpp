#include "RhythmicExtract.h"

RhythmicExtract::RhythmicExtract(int NumWaveformEntries, int NumMaxChannels){
	numWaveformEntries = NumWaveformEntries;
	numMaxChannels = NumMaxChannels;
	nextValueReady = false;
	//readyForEventUpdate = false;

	//initialize size constants
	maxRawStorageEntries = kPlayingPulseRateInHz * numberOfSecondsStored;
	maxLowPassStorageSize = numberOfSecondsStored * soundEnergySampleRate;
	firstLowPassOrder = int(((float)kPlayingPulseRateInHz/(float)soundEnergySampleRate) + 0.5);
	numEntriesOfEachSoundEnergy = numWaveformEntries * numMaxChannels; // used to normalize current sound energy
	
	if(numEntriesOfEachSoundEnergy < 1 )
		numEntriesOfEachSoundEnergy = 1;

	EmptyRhythmicStates();
	Init();
}


void RhythmicExtract::Init(){
	for(int i = 0; i < bufferNumberOfSecondLength; i++){
		sizeOfLPSound[i] = bufferNumberOfSecond[i]  * soundEnergySampleRate;
	}
}

//---------------------------------------------------------------------
//Time Domain Energy Extraction
//---------------------------------------------------------------------
void RhythmicExtract::Update(RenderVisualData * RenderData){
	renderData = *RenderData;

	SoundEnergy.push_back(CurrentSoundEnergy());
	lowpassCounter ++;
	
	//one time speed control
	if(lowpassCounter == firstLowPassOrder - 1){
		nextValueReady = true;
		lowpassCounter = 0;
	}

	if(nextValueReady){
		nextValueReady = false;
		RhythmicProcessingDrive();
		readyForEventUpdate = true;
	}
};

//------------------------------------------------------
//	The processing drive
// it does not include collecting SoundEnergy, but contains all other steps to extract Rhythmic at run time
//------------------------------------------------------
void RhythmicExtract::RhythmicProcessingDrive(){
	SoundEnergyLowPassFilter(); //produce raw low passed curve
	if(lowPassedSoundEnergy.size() > 0){ //float check to make sure the size is non-zero
		SoundEnergyStatisticAnalysis(); //produce average, variance and standard deviation in 3 levels

		SoundEnergyBeatFirstExtraction();
		SoundEnergyBeatSecondExtraction();

		//SoundEnergyIterativePeriodicMatch();
		RemoveTooOldEntries();
	}
}

//------------------------------------------------------
//	Extract the Overall Energy From Time Domain (return L2 ^2 value)
//------------------------------------------------------
//the value is push backed into list<int> SoundEnergy;
float RhythmicExtract::CurrentSoundEnergy(){
	float energy = 0;
	
	for(unsigned int i = 0; i < numWaveformEntries; i++){
		for(unsigned int j = 0; j < numMaxChannels; j ++){
			energy += pow(((float)(renderData).waveformData[j][i]-128.0),2);
		}
	}
	energy = energy / numEntriesOfEachSoundEnergy;
	return energy;
}

//------------------------------------------------------
//	Low Passed Overall Energy
//this method produce lowPassedSoundEnergy from SoundEnergy
//------------------------------------------------------
void RhythmicExtract::SoundEnergyLowPassFilter(){
	//first check if the sound energy vector is large enough
	if(SoundEnergy.size() >= firstLowPassOrder){
		//then check if the sound energy vector is at max size
		float aSmoothedTap = 0;
		int seconditer = 0;

		riterSoundEnergy = SoundEnergy.rbegin();

		//sums up the last firstLowPassOrder values
		while(seconditer < firstLowPassOrder && riterSoundEnergy != SoundEnergy.rend()){
			aSmoothedTap += (*riterSoundEnergy);
			seconditer++;
			riterSoundEnergy++;
		}	

		if(seconditer > 0)
			aSmoothedTap = aSmoothedTap/(float)seconditer; //normalize the value (should never be negative)
	

		//alternative implementation
		//aSmoothedTap = lowPassedSoundEnergy.back() + (SoundEnergy.back() - SoundEnergy[SoundEnergy.size() -firstLowPassOrder])/firstLowPassOrder;
		
		lowPassedSoundEnergy.push_back(aSmoothedTap);

		//check initialization level one time usage
		if(initializedLevel < bufferNumberOfSecondLength){
			if(sizeOfLPSound[initializedLevel] <= lowPassedSoundEnergy.size())
				initializedLevel++;
		}

		//check for minimum or maximum
		if(aSmoothedTap < minLowPassedSoundEnergy && initializedLevel > 1)
			minLowPassedSoundEnergy = aSmoothedTap;

		if(aSmoothedTap > maxLowPassedSoundEnergy)
			maxLowPassedSoundEnergy = aSmoothedTap;		
	}
}

//------------------------------------------------------
// Analysis the sound energy statistics
// critical issue: how to dynamically scale the axis (interpolation?)
//				   fade out of the previous distribution
//
// find	 lowPassedSoundEnergyAxis[MaxDistributionLevel]
//		 lowPassedSoundEnergyDistribution[MaxDistributionLevel]
//------------------------------------------------------
void RhythmicExtract::SoundEnergyStatisticAnalysis(){
	//first find the average energy
	float aBuffer = 0;
	int bufferFillingInd = 0;//fill the statistcal buffer
	
	//lowpass into three levels of average
	int iter = lowPassedSoundEnergy.size() - 1;
	while(iter>= 0 && bufferFillingInd < initializedLevel){
		aBuffer += lowPassedSoundEnergy[iter--];
		if((lowPassedSoundEnergy.size() - iter) == sizeOfLPSound[bufferFillingInd]){
			averageLowPassedSound[bufferFillingInd] = aBuffer/sizeOfLPSound[bufferFillingInd];
			bufferFillingInd++;
		}
	}
		
	//second find the variance energy (BRUTE FORCE STYLE, no chance for improvement)
	for(int j = 0; j < initializedLevel; j++){
		aBuffer = 0;

		for(int i = lowPassedSoundEnergy.size() - 1; i >= sizeOfLPSound[j]; i--){
			aBuffer += pow(lowPassedSoundEnergy[i] - averageLowPassedSound[j], 2);
		}

		varianceOfLowPassedSound[j] = aBuffer/sizeOfLPSound[j];
		standardDeviationOfLowPassedSound[j] = sqrt(varianceOfLowPassedSound[j]);
	}
}

//------------------------------------------------------
//	First Beat Detection
//  critical issue: the beat threshold level
//	use average + variance method?
//	use absolute level?
//	use percentage maxValue?
// find: currentBeatMagnitudeArray[] with statistics
//------------------------------------------------------
void RhythmicExtract::SoundEnergyBeatFirstExtraction(){
	for(int i = 0; i < initializedLevel; i++){
		float beatThreshold = BeatThreshold(i);
		if(roughBeating[i]){
			if(lowPassedSoundEnergy.back() < beatThreshold){
				//beat stop
				roughBeating[i] = false;
				fineBeating[i]  = false;
				beatLastIter[i]  = 0;
				currentBeatMagnitudeArray[i]  = 0;
			}else{
				if(++beatLastIter[i] >=minBeatIter){
					fineBeating[i]  = true;
					if(lowPassedSoundEnergy.back()>currentBeatMagnitudeArray[i] )
						currentBeatMagnitudeArray[i]  = lowPassedSoundEnergy.back();
				}
			}
		}else{
			if(lowPassedSoundEnergy.back() >= beatThreshold){
				//turn on roughBeating
				roughBeating[i]  = true;
			}
		}
	}
}

//------------------------------------------------------
//	Second Beat Detection
//	use: currentBeatMagnitudeArray[]
//	produce: beatLevelDQue, beatStorageDQue, currentBeatLevel, currentBeatEnergy
//------------------------------------------------------
void RhythmicExtract::SoundEnergyBeatSecondExtraction(){
	//first extract basic info from currentBeatMagnitudeArray
	currentBeatLevel = 0;
	currentBeatEnergy = 0;

	for(int i = 0; i < initializedLevel; i++){
		if(currentBeatMagnitudeArray[i] > 0){
			currentBeatEnergy = currentBeatMagnitudeArray[i];
			currentBeatLevel ++;
		}
	}

	//update average level and Magnitude
	averageBeatLevel = (((float)(averageBeatLevel * beatLevelDQue.size())) + currentBeatLevel)/((float)beatLevelDQue.size() + 1.0);
	averageBeatMagnitude = (((float)(averageBeatMagnitude * beatStorageDQue.size())) + currentBeatEnergy)/((float)beatStorageDQue.size() + 1.0);
	
	//collect into storage elements
	beatStorageDQue.push_back(currentBeatEnergy);
	beatLevelDQue.push_back(currentBeatLevel);
}

//Beat Detection Helper-----------------------------------------------
//	Return the Beat threshold
//  critical issue: the beat threshold level
//	use average + variance method?
//	use absolute level?
//	use percentage maxValue?
//	THIS PROCESS SHOULD BE DYNAMICAL
//------------------------------------------------------
float RhythmicExtract::BeatThreshold(int BufferInd){
	float threshold = 0;
	//error checking
	if(BufferInd >= initializedLevel)
		BufferInd = initializedLevel;

	threshold = averageLowPassedSound[BufferInd] + beatThresholdStandardDeviationScale * standardDeviationOfLowPassedSound[BufferInd];

	return threshold;
}

/* Period Detection
//------------------------------------------------------
//	Periodic check for the Rhythmic pattern
//	use: beatStorageDQue, beatLevelDQue
//  produce: int periodInMS[MaxPeriod];
//------------------------------------------------------
void RhythmicExtract::SoundEnergyIterativePeriodicMatch(){
	//detection should start at first beat
	int startIndex = 0;
	float score = 0;
	while( (startIndex < beatLevelDQue.size()) && (beatLevelDQue[startIndex] == 0) ){
		startIndex++;
	}

	//start Iterative Testing: 
	//Strategies:
	//	1 rely on beatStorageDQue
	//	2 rely on beatLevelDQue

	//use strategy 2 here
	for(int iter = MiniInterval; iter <= MaxInterval; iter+=IntervalIncrement){
		score = IterativePeriodicMatchScoring(iter);
		InsertPeriod(iter, score);
	}
	ConvertToMSPeriod();
}

//Period Detection Helper-------------------------------
//------------------------------------------------------
//	Periodic scoring for the Rhythmic pattern
//	use: beatStorageDQue, beatLevelDQue, and PeriodFilter
//------------------------------------------------------
float RhythmicExtract::IterativePeriodicMatchScoring(int iter){
	float score = 0;


	return score;
}

//------------------------------------------------------
//	Insert an iter with its score
//	produce: int periodInIter[MaxPeriod];
//------------------------------------------------------
void RhythmicExtract::InsertPeriod(int iter, float score){

}

//------------------------------------------------------
//	Convert periodInIter to periodInMS
//------------------------------------------------------
void RhythmicExtract::ConvertToMSPeriod(){

}
//------------------------------------------------------
*/

void RhythmicExtract::RemoveTooOldEntries(){
	//Remove too old entries to keep the size of energy and time list reasonable
	while(SoundEnergy.size() > maxRawStorageEntries){
		SoundEnergy.pop_front();	
	}
	//deque<float> lowPassedSoundEnergy; maxLowPassStorageSize
	//deque<float> beatStorageDQue; maxLowPassStorageSize
	//vector<int>beatLevelDQue; maxLowPassStorageSize
	while(lowPassedSoundEnergy.size() > maxLowPassStorageSize){
		lowPassedSoundEnergy.pop_front();
	}

	while(beatStorageDQue.size() > maxLowPassStorageSize){
		beatStorageDQue.pop_front();
	}

	while(beatLevelDQue.size() > maxLowPassStorageSize){
		beatLevelDQue.pop_front();
	}
}

//------------------------------------------------------
//	Empty Rhythmic Related States
//------------------------------------------------------
void RhythmicExtract::EmptyRhythmicStates(){
	initializedLevel = 0; //this is updated in the low pass filter
	
	FreeAll(SoundEnergy);
	FreeAll(lowPassedSoundEnergy);

	FreeAll(beatLevelDQue);

	FreeAll(beatStorageDQue);

	lowpassCounter = 0;
	
	maxLowPassedSoundEnergy = -10000; //assume the energy is never negative
	minLowPassedSoundEnergy = 9999999999999999999; //assume the energy is always smaller than this

	averageBeatLevel = 0;

	for(int i = 0; i < bufferNumberOfSecondLength; i++){
		averageLowPassedSound[i] = 0;
		varianceOfLowPassedSound[i] = 0;
		standardDeviationOfLowPassedSound[i] = 0;

		roughBeating[i] = false;
		fineBeating[i] = false;
		beatLastIter[i] = 0;
		currentBeatMagnitudeArray[i] = 0.0;
		dynamicThresholdScale[i] = beatThresholdStandardDeviationScale;
	}
}