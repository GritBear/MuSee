#pragma once
#include "AFactory.h"
#include "ATreeBranch.h"
#include "StatisticalFunctions.h"

//a concrete class under AFactory
//creates light emitting trees (purple light in the concept art)
class LightStraw : AFactory{
private:
	float _totallength;
	control * _theControl;

	//helper methods
	void computeTotoalLength(Vector3 * root, Vector3 * head);

public:
	LightStraw(){ init(); }
	~LightStraw(){ destroy(); }
	void init();
	void destroy();
	AInterface * create(void * param);
};