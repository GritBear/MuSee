#pragma once
#include <vector>
#include <list>
#include "Parameters.h"

using namespace std;

struct RenderVisualData {
	unsigned __int8		numWaveformChannels;
	unsigned __int8 **	waveformData;
	
	unsigned __int8		numSpectrumChannels;
	unsigned __int8	**	spectrumData;
};

class MelodyExtraction {
private:
	int melodyStoragesVecSize1;
	int melodyStoragesVecSize2;
	vector<vector<float>> melodyStoragesVec;
	float dynamicThreshold;
	int num_tones;
	float FrequencyInterval;
	list<vector<int>> stepMelody; // this is used to create melody and other things
	vector<int> FrequencyBandSeparatorVec;
	int MaxTone;
	vector<int> FrequencyBandSeparatorVec;
	vector<int> ToneBandSeparatorVec;
	unsigned int MaxFrequencyIndex;
	unsigned int numMaxChannels;
	int pulseIter;
	int pulseIterUpperLimit;

	RenderVisualData	renderData;

protected:
	//------------------------------------------------
	// Storage Methods
	//------------------------------------------------
	bool MelodyStore();
	int GetLatestPulseIter();

	//------------------------------------------------
	// Extraction Methods
	//------------------------------------------------
	void  SpectrumMatrixAnalysis();
	void  BandedNormalizeMatrix(vector<int> Separator);
	void  FilterDrive();
	float FilteredEntry(unsigned int row, unsigned int col);
	void StepToneRegistrationFromFreq(vector<float> abuffer);
	void StepFreRegistration();
	void DynamicThresholdAdjustment(int num_tones);
	void MelodyLineAssignment();

	//------------------------------------------------
	// Acoustic Operations
	//------------------------------------------------
	//this method create band separator vector. frequency interval and max considered frequency
	void ConstructFrequencyBands();
	int FreqToToneInd(float frequency);
	int ToneToFrequencyInd(float tone);

	//------------------------------------------------
	// Simple Matrix Operations
	//------------------------------------------------
	void GetMatrixMaxMin(float &max, float &min);
	void PowerMatrix(int power);
	void ScalerMultiplySpectrumMatrix (float scaler);
	void Log10SpectrumMatrix();
	void ScalerAddMatrix(float value);
	// this will set all values below threshold to bottom, above or equal threshold to top
	void HardDecisionPolarizeRule(float Threshold, float bottom, float top);
	void HighFreqCompensation();

public:
	MelodyExtraction(){init();}; //
	~MelodyExtraction(){Destroy();};
	void init();
	void Destroy();
	void Update();
	void Clean(); //RemoveTooOldEntries()

};

//-----------------------------------------------------
//Memory free function for containers with swap() method
template <typename T>
void FreeAll( T & t ) {
    T tmp;
    t.swap( tmp );
}
