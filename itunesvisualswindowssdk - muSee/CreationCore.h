/*This is the art creation core
	It creates magical art experience through smart probabilistic model
	It collects all the objects and related parameter, then assign probability to individual object

	It also come with transistion effect core and user drawing core
*/


#pragma once
#include "CreationInterface.h"
#include "AInterface.h"



class CreationCore : CreationInterface{
private:
protected:
	
	vector<float>animationStartingProbability;
	vector<float>backgroundProbability;
	vector<float>verticalAnimationStartingProbability;

	//current state storage variables
	list<int> backgroundIDList;
	list<int> backgroundVerticalAnimationIndexList;
	vector<list<int>> MelodyAnimationIndexVecList;

public:
	CreationCore(DataReader *aReader); //legacy code
	CreationCore(CreationCoreComponent *aCreationCoreComponentStruct);
	~CreationCore(){Destroy();}

	void Destroy();
	void Init();

	//----------------------
	//Update methods
	void Update(); 


	//=================================================================================
	//Creation Part
	//=================================================================================
	void NextAnimation(int &Index , vector<float> &probabilityVect);
	void NextVerticalAnimation(float percentageIntoTheTheme, int &Index, vector<float> &probabilityVect);

	void NextBackground(int &Index);

	void AnimationProbUpdate();
	void BackgroundProbUpdate();
	//=================================================================================
	//Transistion Part
	//=================================================================================





	//=================================================================================
	//User Drawing Effect
	//=================================================================================






	//storage maintenance 
	void RemoveTooOldEntries(bool Removeall = false);
	void TractChange();
	//=================================================================================
	//Getter and Setter
	RhythmicEventCore * GetRhythmicEvenCore(){return theRhythmicEventCore;}

};