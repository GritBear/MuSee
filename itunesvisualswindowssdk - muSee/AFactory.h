#pragma once
#include "AInterface.h" //animation interface
#include "AniStructs.h"
//define the base abstract class for the factory

struct AniFacParam {
	Vector3 foot;
	Vector3 head;
	Vector3 vel;
	Color3 color; //this is light color in lightforestfac
	control * theControl;
};

class AFactory {
private:
protected:
public:
	AFactory(){init();}
	~AFactory(){destroy();}
	virtual void init(){};
	virtual void destroy(){};
	virtual AInterface * create(void * param){return NULL;} //equivalent to Object param 


};