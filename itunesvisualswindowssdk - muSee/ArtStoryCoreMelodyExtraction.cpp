#include "ArtStoryCore.h"

//=================================================================
//Analysis methods
//Author: Li(Lee) Xiong. All rights reserved.
//=================================================================
//------------------------------------------------------
// Spectrum Matrix Analysis Drive
//------------------------------------------------------
void ArtStoryCore::SpectrumMatrixAnalysis(){
	/*	Available Infos:
		melodyStoragesVec
	*/
	float max, min;
	PowerMatrix(NormalizationPower); //power up and normalize
	GetMatrixMaxMin(max, min); //shifts to ensure all zero  or positive
	ScalerAddMatrix(-min); //integrate HighFreqCompensation inside
	//HighFreqCompensation();
	//BandedNormalizeMatrix(FrequencyBandSeparatorVec); //integrate HardDecisionPolarizeRule
	//HardDecisionPolarizeRule(dynamicThreshold,valueBottom,valueTop); // hard decision rule

	StepFreRegistration(); //Including the Tone Translation
};

//------------------------------------------------------
// Gussian Filtering Drive
//------------------------------------------------------
void ArtStoryCore::FilterDrive(){
	vector<vector<float>> atempMatrix(melodyStoragesVecSize1, vector<float>(melodyStoragesVec[0].size()));

	for(unsigned int i = 0; i < melodyStoragesVecSize1; i++){
		for(unsigned int j = 0; j < melodyStoragesVecSize2; j++){
				 atempMatrix[i][j] = FilteredEntry(i,j);
		}
	}

	FreeAll(atempMatrix);

	melodyStoragesVec = atempMatrix;
};

//------------------------------------------------------
// 3 by 3 Gussian Filtering
//------------------------------------------------------
float ArtStoryCore::FilteredEntry(unsigned int row, unsigned int col){
	float value = melodyStoragesVec[row][col];
	
	//contribution from upper left three neighbors
	if(row>=1 && col>=1){
		value += melodyStoragesVec[row-1][col] * filterMatrix[0][1];
		value += melodyStoragesVec[row - 1][col - 1] * filterMatrix[0][0];
		value += melodyStoragesVec[row][col - 1] * filterMatrix[1][0];
	}else if(row>=1){
		value += melodyStoragesVec[row-1][col] * filterMatrix[0][1];
	}else if(col>=1){
		value += melodyStoragesVec[row][col - 1] * filterMatrix[1][0];
	}

	//contribution from lower right three neighbors
	if( row < (melodyStoragesVecSize1 -1) && col< (melodyStoragesVec[row].size() -1)){
		value += melodyStoragesVec[row+1][col] * filterMatrix[2][1];
		value += melodyStoragesVec[row + 1][col + 1] * filterMatrix[2][2];
		value += melodyStoragesVec[row][col + 1] * filterMatrix[1][2];
	}else if( row < (melodyStoragesVecSize1 -1)){
		value += melodyStoragesVec[row+1][col] * filterMatrix[2][1];
	}else if(col< (melodyStoragesVec[row].size() -1)){
		value += melodyStoragesVec[row][col + 1] * filterMatrix[1][2];
	}

	//contribution from lower left and upper right corners
	if( row>=1 && col<(melodyStoragesVec[row].size() -1)){
		value += melodyStoragesVec[row-1][col+1] * filterMatrix[0][2];
	}
	if( row<(melodyStoragesVecSize1 -1) && col>=1){
		value += melodyStoragesVec[row+1][col-1] * filterMatrix[2][0];
	}

	return value;
}

//------------------------------------------------------
//	Perform Frequency Banded Power Normalization
//------------------------------------------------------
void ArtStoryCore::BandedNormalizeMatrix(vector<int> Separator){
	int Bands = Separator.size();
	for(int i = 0; i <= Bands; i++){
		unsigned int NextSeparatingIndex;
		unsigned int PreviousSeparatingIndex;
		float  BandMax;
		// find the starting and ending of each chunck;
		if(i == 0){
			NextSeparatingIndex = Separator[i];
			PreviousSeparatingIndex = 0;
		}else if(i == Bands){
			NextSeparatingIndex = melodyStoragesVecSize1;
			PreviousSeparatingIndex = Separator[i-1];
		}else{
			NextSeparatingIndex = Separator[i];
			PreviousSeparatingIndex = Separator[i-1];
		}
		BandMax = melodyStoragesVec[PreviousSeparatingIndex][0];
		//find the maximum
		for(unsigned int j = PreviousSeparatingIndex; j < NextSeparatingIndex; j++){
			for(unsigned int k = 0; k < melodyStoragesVecSize2; k ++){
				if(BandMax < melodyStoragesVec[j][k])
					BandMax = melodyStoragesVec[j][k];
			}
		}
		if(BandMax!=0){
			float normalfactor = MaxAfterNormalizedMatrixValue / BandMax;
			//normalize to predefined maximum
			for(unsigned int j = PreviousSeparatingIndex; j < NextSeparatingIndex; j++){
				for(unsigned int k = 0; k < melodyStoragesVecSize2; k ++){
					if( melodyStoragesVec[j][k] * normalfactor >= dynamicThreshold)
						melodyStoragesVec[j][k]  = valueTop;
					else
						melodyStoragesVec[j][k]  = valueBottom;
				}
			}
		}
	}//for number of bands
}

// Register Step Melody Notes into stepMelodyTestOut:
// Integrate the dynamic threshold adjustment here
void ArtStoryCore::StepToneRegistrationFromFreq(vector<float> abuffer){
	vector<int> astepToneMelody;
	vector<int> astepToneBass;
	vector<float>TestVisualInput(num_tones,0);
	int num_tones = 0;
	
	// the extracted tone indexes are from low to high
	for(unsigned int i = 0; i < abuffer.size(); i++){
		if(abuffer[i] == valueTop){
			int tonePoint = FreqToToneInd(i * FrequencyInterval);
			//push the tone into either bass or melody
			if(tonePoint < bassMelodyDevideTone){
				if(astepToneBass.size()<1){
					astepToneBass.push_back(tonePoint);
					num_tones++;
				}
				else if(tonePoint != astepToneBass.back()){
					astepToneBass.push_back(tonePoint);
					num_tones++;
				}
			}
			else{
				if(astepToneMelody.size()<1){
					astepToneMelody.push_back(tonePoint);
					num_tones++;
				}
				else if(tonePoint != astepToneMelody.back()){
					astepToneMelody.push_back(tonePoint);
					num_tones++;
				}
			}
		}
	}

	//perform dynamic threshold adjustment
	DynamicThresholdAdjustment(num_tones);
	
	//store the extracted tones
	stepMelody.push_back(astepToneMelody);

	// The following code is purely used to generate test
	// Ignore the lowest Base at Index 0
	//if(astepToneMelody.size()>0){
	//	for(unsigned int i = 0; i < astepToneMelody.size(); i++){
	//		int index = astepToneMelody[i];
	//		//Some Error Checking
	//		if(index >= 0){
	//			if(index >= TestVisualInput.size())
	//				index = TestVisualInput.size() - 1;

	//			TestVisualInput[index] = valueTop;	
	//		}
	//	}
	//}
	//
	//stepMelodyTestOut.push_back(TestVisualInput);

	FreeAll(TestVisualInput);
	FreeAll(astepToneMelody);
}

void ArtStoryCore::StepFreRegistration(){
	int Bands = FrequencyBandSeparatorVec.size();
	vector<float> abuffer (melodyStoragesVecSize1, 0);

	//Sum the melodyStorage into Buffer
	for(unsigned int i = 0; i < melodyStoragesVecSize1; i++){
		for(unsigned int j = 0; j < melodyStoragesVecSize2; j++){
			if(j == 0)
				abuffer[i] = melodyStoragesVec[i][j];
			else
				abuffer[i] += melodyStoragesVec[i][j];
		}
	}
	
	
	// perform Banded Normalization
	for(int i = 0; i <= Bands; i++){
		int NextSeparatingIndex;
		int PreviousSeparatingIndex;
		float  BandMax;

		// find the starting and ending of each chunck;
		if(i == 0){
			NextSeparatingIndex = FrequencyBandSeparatorVec[i];
			PreviousSeparatingIndex = 0;		
		}else if(i == Bands){
			NextSeparatingIndex = abuffer.size();
			PreviousSeparatingIndex = FrequencyBandSeparatorVec[i-1];
		}else{
			NextSeparatingIndex = FrequencyBandSeparatorVec[i];
			PreviousSeparatingIndex = FrequencyBandSeparatorVec[i-1];
		}

		BandMax = abuffer[PreviousSeparatingIndex];

		//find the maximum
		for(int j = PreviousSeparatingIndex; j < NextSeparatingIndex; j++){
				if(BandMax < abuffer[j])
					BandMax = abuffer[j];
			}
		if(BandMax !=0){
			float factor = valueTop/BandMax;
			//normalize and hard decision
			for(int j = PreviousSeparatingIndex; j < NextSeparatingIndex; j++){
				abuffer[j] = abuffer[j] * factor;
			if(abuffer[j]>=dynamicThreshold)
				abuffer[j] = valueTop;
			else
				abuffer[j] = valueBottom;
			}
		}
	}//Bands

	StepToneRegistrationFromFreq(abuffer);
	FreeAll(abuffer);
}

//------------------------------------------------------
//	Adjust the Hard Decision Threshold
//------------------------------------------------------
void ArtStoryCore::DynamicThresholdAdjustment(int num_tones){
	if(num_tones < dynamicPointsMinimum)
			dynamicThreshold = dynamicThreshold * dynamicDecreaseFactor;
	
	if(num_tones >dynamicPointsMaximum)
			dynamicThreshold += (valueTop - dynamicThreshold) * dynamicIncreaseFactor;
}

//------------------------------------------------------
//	Dynamic MelodyLine Processing
//------------------------------------------------------
void ArtStoryCore::MelodyLineAssignment(){
	//first iteration through the melody line objects asking if they want one or a few tones and return a score for each note
	//depending on the score, assign tones to alive melody objects
	//then spawn new melody objects if the remaining tones meet certain condition
	//another tone storage element is needed here

	/*Critical information:
		stepMelody
		MelodyObjList
	*/
	vector<int> * astepmelody = &stepMelody.back();
	int num_tone_extracted = (*astepmelody).size();
	
	// by definition MelodyObjList.begin() is always the TopMelody
	if(num_tone_extracted > 0){
		for(melodyIter = MelodyObjList.begin(); melodyIter != MelodyObjList.end(); ++melodyIter){
			if((*melodyIter)->GetIsTheHighestLine()){
				//Assign the highest melody to it
				logMelody = (*astepmelody)[num_tone_extracted-1];//for logging purpose
				(*melodyIter)->AcceptTone((*astepmelody)[num_tone_extracted-1], 0);
				
			}else if((*melodyIter)->GetAlive()||(*melodyIter)->GetRevivable()){
				// dummy codes here
				//vector<float> topscore = (*melodyIter)->AnalyseMelodyTones(astepmelody);

				//FreeAll(topscore);
			}
		}
	}
	//FreeAll(*astepmelody); should free step melody somewhere else
}

//------------------------------------------------------
// Dynamic Frequency Band Converting
//this method create band separator vector. frequency interval and max considered frequency
//------------------------------------------------------
void ArtStoryCore::ConstructFrequencyBands(){

	for(int iter = 0; iter < NumFrequencyBand - 1; iter ++){
		//Compute the Separator
		FrequencyBandSeparatorVec.push_back((int)(FrequencySeparationPoint[iter]/FrequencyInterval+0.5));
		ToneBandSeparatorVec.push_back(FreqToToneInd(FrequencySeparationPoint[iter]));
	}

	MaxFrequencyIndex = (int)(FrequencySeparationPoint[NumFrequencyBand - 1]/FrequencyInterval); // this include a little bit Extra
	//Tone Analysis
	MaxTone = FreqToToneInd(FrequencySeparationPoint[NumFrequencyBand-1]);
}

//------------------------------------------------------
// Store the Melodies
//------------------------------------------------------
bool ArtStoryCore::MelodyStore(){
	
	unsigned int	index;
	unsigned int	channel;
	
	// Sums to combine into a single channel sotrage
	for ( channel = 0;channel < numMaxChannels; channel++ ){
		if(channel == 0){
			for ( index = 0; index < melodyStoragesVecSize1; index++ ){
				melodyStoragesVec[index][pulseIter] = renderData.spectrumData[channel][index];
			}
		}
		else{
			for ( index = 0; index < melodyStoragesVecSize1; index++ ){
				melodyStoragesVec[index][pulseIter] += renderData.spectrumData[channel][index];
			}
		}
	}

	//PulseIter Management
	if(pulseIter>=pulseIterUpperLimit - 1){
		pulseIter = 0;
		return true;
	}else{
		pulseIter++;
		return false;
	}
}

int ArtStoryCore::GetLatestPulseIter(){
	if(pulseIter > 0)
		return pulseIter - 1;
	else
		return pulseIterUpperLimit - 1;
}

//------------------------------------------------------
// Matrix Algorithmics
//------------------------------------------------------
void ArtStoryCore::GetMatrixMaxMin(float &max, float &min){
	max = melodyStoragesVec[0][0];
	min = melodyStoragesVec[0][0];

	for(unsigned int i = 0; i < melodyStoragesVecSize1; i++){
		for(unsigned int j = 0; j < melodyStoragesVecSize2; j++){
			float cur = melodyStoragesVec[i][j];
			//if(max<cur){
				//max = cur;
			//}else if(min>cur){
				if(min>cur){
					min = cur;
				}
		}
	}
}

void ArtStoryCore::ScalerAddMatrix(float value){ //with high freq compensation included
	//use linear compensation for frequency
	float slope = (endScale - startScale) / ((float)melodyStoragesVecSize1);

	for(unsigned int i = 0; i < melodyStoragesVecSize1; i++){
		float scaleFreq = ((float)i + 1.0) * slope + startScale;
		for(unsigned int j = 0; j < melodyStoragesVecSize2; j++){
				 melodyStoragesVec[i][j] = (melodyStoragesVec[i][j] +  value)* scaleFreq;
		}
	}
}

void ArtStoryCore::PowerMatrix(int power){
	for(unsigned int i = 0; i < melodyStoragesVecSize1; i++){
		for(unsigned int j = 0; j < melodyStoragesVecSize2; j++){
				 melodyStoragesVec[i][j] = pow(melodyStoragesVec[i][j],power);
		}
	}
}

void ArtStoryCore::ScalerMultiplySpectrumMatrix (float scaler){
	for(unsigned int i = 0; i < melodyStoragesVecSize1; i++){
		for(unsigned int j = 0; j < melodyStoragesVecSize2; j++){
				 melodyStoragesVec[i][j] = (scaler * melodyStoragesVec[i][j]);
		}
	}
}

void ArtStoryCore::Log10SpectrumMatrix(){
	for(unsigned int i = 0; i < melodyStoragesVecSize1; i++){
		for(unsigned int j = 0; j < melodyStoragesVecSize2; j++){
				 melodyStoragesVec[i][j] = log10(melodyStoragesVec[i][j]);
		}
	}
}

// this will set all values below threshold to bottom, above or equal threshold to top
void ArtStoryCore::HardDecisionPolarizeRule(float Threshold, float bottom, float top){
	for(unsigned int i = 0; i < melodyStoragesVecSize1; i++){
		for(unsigned int j = 0; j < melodyStoragesVecSize2; j++){
			if( melodyStoragesVec[i][j] >= Threshold)
				melodyStoragesVec[i][j] = top;
			else
				melodyStoragesVec[i][j] = bottom;
		}
	}
}

//logrithmic conversion (verified by matlab)
int ArtStoryCore::FreqToToneInd(float frequency){
	float tone = 12 * (log(frequency/C4)/log(2.0));
	if(tone<minimumTone)
		tone = minimumTone;
	int toneInd = (int)((tone - minimumTone)/ toneResolution +0.5);     // Incoperate Resolution into the computation
	return toneInd;
};

int ArtStoryCore::ToneToFrequencyInd(float tone){
	float freq = C4 * pow(2, (float)tone/12.0);
	float IndexPrecise =  freq / FrequencyInterval;
	
	return (int)(IndexPrecise + 0.5); //this is equivalent to rounding
};

//give high frequency energy compensation
void ArtStoryCore::HighFreqCompensation(){
	//use linear compensation
	float slope = (endScale - startScale) / ((float)melodyStoragesVecSize1);
	for(unsigned int i = 0; i < melodyStoragesVecSize1; i++){
		float scaleFreq = ((float)i + 1.0) * slope + startScale;
		for(unsigned int j = 0; j < melodyStoragesVecSize2; j++){
				 melodyStoragesVec[i][j] = melodyStoragesVec[i][j] * scaleFreq;
		}
	}
}
