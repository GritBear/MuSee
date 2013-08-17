#pragma once
#include "Global.h"
#include "DataReader.h"
#include "RhythmicEventCore.h"
#include "MelodicEventCore.h"

//this struct is used to initialize the creation core
struct CreationCoreComponent{
	DataReader *aReader;
	RhythmicExtract * aRhythmicExtract;
};

class CreationInterface{
private:
protected:
	//initialize all major components
	DataReader *theReader;

	//RhythmicComponents
	RhythmicExtract *theRhythmicExtract;
	RhythmicEventCore *theRhythmicEventCore;
	RhythmicEvent *currentEvent;

public:
	virtual void Destroy(){}
	virtual void Init(CreationCoreComponent * aCreationCoreComponentStruct);

	//----------------------
	//Update methods
	virtual void Update(){}

	virtual void NextAnimation(int &Index , vector<float> &probabilityVect);
	virtual void ProbabilityUpdate(){};

	virtual void Clean(){};
};
