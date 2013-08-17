#include "CreationInterface.h"

void CreationInterface::Init(CreationCoreComponent * aCreationCoreComponentStruct){
	theReader = aCreationCoreComponentStruct->aReader;
	theRhythmicExtract = aCreationCoreComponentStruct->aRhythmicExtract;

	//memset(&currentEvent, 0, sizeof(currentEvent)); redundant
	theRhythmicEventCore = new RhythmicEventCore(theRhythmicExtract);
}


void CreationInterface::NextAnimation(int &Index, vector<float> &probabilityVect){
	//initialize probabilityVect at first use
	if(probabilityVect.size()<1)
		Index = 0;
	else{
		//-----------------------------------------------------------------------
		// Probablistic Next Animation Index Draw
		//-----------------------------------------------------------------------
		//random animation object index generation
		const int currentMaxType = probabilityVect.size();

		vector<float> cumulativeProbability(currentMaxType + 1, 0);
		int rand_precision_int = 10000;
		float totalWeights = 0; //calculate again in case the sum do not equal 1
	
		cumulativeProbability[0] = 0;
		for(int i = 0; i<currentMaxType; i++){
			totalWeights += probabilityVect[i];
			cumulativeProbability[i + 1] = totalWeights;
		}

		float rand_select = rand() % rand_precision_int;
		float target_rand = rand_select*totalWeights/(float)rand_precision_int;

		for(int i = currentMaxType - 1; i >= 0 ; i--)
		{
			if(target_rand >= cumulativeProbability[i]){
				Index = i;
				break;
			}
		}
	}
}