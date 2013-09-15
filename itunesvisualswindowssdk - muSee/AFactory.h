#pragma once
#include "AInterface.h" //animation interface
//define the abstract class for the factory

class AFactory {
private:
protected:
public:
	AFactory();
	~AFactory();
	AInterface * create(void * param); //equivalent to Object param 
};