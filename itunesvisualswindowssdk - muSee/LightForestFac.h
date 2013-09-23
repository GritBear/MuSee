#pragma once
#include "AFactory.h"
#include "ATreeBranch.h"

//a concrete class under AFactory
//creates light emitting trees (purple light in the concept art)
class LightForestFac : AFactory{
private:
	float _totallength;
	control * _theControl;

	//helper methods
	void computeTotoalLength(Vector3 * root, Vector3 * head);
	int numberOfBranches(); //depends on length


public:
	LightForestFac(){init();}
	~LightForestFac(){destroy();}
	void init();
	void destroy();
	AInterface * create(void * param);
};