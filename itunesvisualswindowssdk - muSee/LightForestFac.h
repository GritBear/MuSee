#pragma once
#include "AFactory.h"
#include "ATreeBranch.h"

//a concrete class under AFactory
//creates light emitting trees (purple light in the concept art)
class LightForestFac : AFactory{
private:
	//helper methods


public:
	LightForestFac(){init();}
	~LightForestFac(){destroy();}
	void init();
	void destroy();
	AInterface * create(void * param);
};