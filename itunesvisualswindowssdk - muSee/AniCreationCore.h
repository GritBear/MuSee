#pragma once
#include "CreationInterface.h"

//this class is included in the Melody class for animation spawning
class AniCreationCore : CreationInterface{
private:
protected:

public:
	AniCreationCore(CreationCoreComponent *aCreationCoreComponentStruct);
	~AniCreationCore(){Destroy();}

	void Destroy();







};