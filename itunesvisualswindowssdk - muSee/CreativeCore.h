#pragma once
#include "MelodyObj.h"
#include "Background.h"
#include "control.h"
//testing Modules
#include "AllAObj.h"

//this is a static class that gets called by art story driver. 
//It manages the animation, theme, speed and effects of all the melody and background obj
namespace Creative{
	//fields
	static Background * theBackground;
	static list<MelodyObj *> * allMelodyList;
	static control * theControl;
	static DataReader *theReader;
	static SpeedControl *theSpeedControl;

	//creative field
	//static float speedScaler; //defualt = 1;

	static void UpdateCreative(){
		//theSpeedControl->GetFPS();
	}

	//setters
	static void SetControl(control * theControl)
	{   
		Creative::theControl = theControl;
		theReader = theControl->GetReader();
		theSpeedControl = theControl->GetSpeedControl();
	}
	static void SetBackground(Background * theBackground){Creative::theBackground = theBackground;}
	static void SetMelodyList(list<MelodyObj *> * allMelodyList){Creative::allMelodyList = allMelodyList;} 

};

