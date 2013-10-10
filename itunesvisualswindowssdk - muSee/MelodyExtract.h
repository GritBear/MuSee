//#pragma once
//#include "Global.h"
//
////modularize melody extraction
//class MelodyExtract {
//private:
//protected:
//	//=================================================================
//	// Melody variables
//	//=================================================================
//	unsigned int timeInMS;
//	
//	unsigned int numSpectrumEntries;
//	unsigned int numMaxChannels;
//	unsigned int numWaveformEntries;
//	
//	int pulseIter;
//	int pulseIterUpperLimit;
//	int melodyExtractRate;   //extract melody N times a second
//	vector<vector<float>> melodyStoragesVec;
//	
//	//frequency processing
//	float FrequencyInterval;
//	unsigned int MaxFrequencyIndex;
//	float dynamicThreshold;
//
//	int num_tones;
//	int MaxTone;
//	vector<int> FrequencyBandSeparatorVec;
//	vector<int> ToneBandSeparatorVec;
//
//	list<vector<float>> stepMelodyTestOut; //stepMelodyTestOut all step extracted tones to display
//	list<vector<int>> stepMelody; // this is used to create melody and other things
//
//public:
//	//=================================================================
//	//Core methods
//	//=================================================================
//
//
//	//=================================================================
//	//Analysis methods
//	//=================================================================
//	void  SpectrumMatrixAnalysis();
//	void  BandedNormalizeMatrix(vector<int> Separator);
//	void  FilterDrive();
//	float virtual FilteredEntry(unsigned int row, unsigned int col);
//	
//	void virtual StepToneRegistrationFromFreq(vector<float> abuffer);
//	void virtual StepFreRegistration();
//
//	void DynamicThresholdAdjustment(int num_tones);
//
//	void virtual MelodyLineAssignment();
//
//}