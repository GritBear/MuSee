#include "CreationCore.h"

CreationCore::CreationCore(DataReader *aReader){
	theReader = aReader;
}

CreationCore::CreationCore(CreationCoreComponent *aCreationCoreComponentStruct){
	CreationInterface::Init(aCreationCoreComponentStruct);
	CreationCore::Init();
}

void CreationCore::Destroy(){
	theRhythmicEventCore->Destroy();
	RemoveTooOldEntries(true);
}

void CreationCore::Init(){
	/*this is now a skeleton implementation*/
	theReader->GetStartingProbability(animationStartingProbability);
	verticalAnimationStartingProbability.push_back(0.8);
	verticalAnimationStartingProbability.push_back(0.2);
}

//All update
void CreationCore::Update(){
	//rhythmic update
	theRhythmicEventCore->Update(); //event driven lock is implemented
	currentEvent = theRhythmicEventCore->GetCurrentEvent();

	RemoveTooOldEntries();
}

void CreationCore::NextAnimation(int &Index, vector<float> &probabilityVect){
	//initialize probabilityVect at first use
	if(probabilityVect.size()<animationStartingProbability.size())
		probabilityVect = animationStartingProbability;
	
	CreationInterface::NextAnimation(Index, probabilityVect);
}

void CreationCore::NextVerticalAnimation(float percentageIntoTheTheme, int &Index, vector<float> &probabilityVect){
	
	//initialize probabilityVect at first use
	if(probabilityVect.size()<verticalAnimationStartingProbability.size())
		probabilityVect = verticalAnimationStartingProbability;
	
	if(probabilityVect.size()<=1)
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
	//for testing purpose
	Index = 0;
}

void CreationCore::NextBackground(int &Index){
	Index = 0;
	if(backgroundIDList.size() == 0){
		backgroundIDList.push_back(Index);
	}else{
		if(Index != backgroundIDList.back()){
			backgroundIDList.push_back(Index);
			RemoveTooOldEntries(); //some book keeping
		}
	}
}

//-----------------------------------------------------------------
//storage maintenance
//-----------------------------------------------------------------
void CreationCore::RemoveTooOldEntries(bool Removeall){
	//define the maximum storage limits here;
	int backgroundIndStorageLimit = 10;

	if(Removeall){
		FreeAll(verticalAnimationStartingProbability);
		FreeAll(animationStartingProbability);
		FreeAll(backgroundProbability);

		FreeAll(backgroundVerticalAnimationIndexList);
		FreeAll(MelodyAnimationIndexVecList);

		FreeAll(backgroundIDList);
	}else{
		while(backgroundIDList.size()>backgroundIndStorageLimit){
			backgroundIDList.pop_front();
		}
	}
}


void CreationCore::TractChange(){
	theRhythmicEventCore->EmptyStates();

}