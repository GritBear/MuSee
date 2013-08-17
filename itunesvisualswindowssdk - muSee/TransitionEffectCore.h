/*
This module process the transition effects of animation objects from different class
	-. Add a layer in front of Background/VerticalAnimationTexture/Melody Animation to achieve this module
	-. it should be able to call the special effect object (take advantage of existing effects)

*/

#pragma once
#include "Global.h"
#include "SpecialEffectCore.h"

class TransitionEffectCore{
private:
	bool alive;

protected:
	int startTheme, endTheme;


public:
	TransitionEffectCore(int StartTheme, int EndTheme);
	void Destroy();

	

};