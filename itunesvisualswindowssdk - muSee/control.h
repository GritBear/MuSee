#pragma once
#include "DataReader.h"
#include "CreationCore.h"


struct ControlObj{
	//-----------------------------------------------------------------
	// DataReader Object
	DataReader *theReader;

	//-----------------------------------------------------------------
	// Probabilistic Core Object	
	CreationCore *theCreationCore;

	//-----------------------------------------------------------------
	SpeedControl *theSpeedControl;

	//-----------------------------
	//current screen dimensions
	float * screenW;
	float * screenH;
};

struct IndexThemeControl {
	int animationIndex;
	int themeID;
};

//using an object to solve security problem
class control {
private:
	//-----------------------------------------------------------------
	// DataReader Object
	DataReader *theReader;

	//-----------------------------------------------------------------
	// Probabilistic Core Object	
	CreationCore *theCreationCore;

	//-----------------------------------------------------------------
	SpeedControl *theSpeedControl;

	//-------------screen parameters
	float * screenW;
	float * screenH;
public:
	control(DataReader *TheReader, CreationCore *TheCCore, SpeedControl *TheSpeedControl){
		theReader = TheReader;
		theCreationCore = TheCCore;
		theSpeedControl = TheSpeedControl;
	}

	control(ControlObj *controlStruc){
		theReader = controlStruc->theReader;
		theCreationCore = controlStruc->theCreationCore;
		theSpeedControl = controlStruc->theSpeedControl;
		screenW = controlStruc->screenW;
		screenH = controlStruc->screenH;
	}

	void Update(); //internal parameter updates

	//Getters
	DataReader * GetReader(){return theReader;}
	CreationCore * GetCreationCore(){return theCreationCore;}
	SpeedControl * GetSpeedControl(){return theSpeedControl;}

	float GetScreenW(){return (*screenW);}
	float GetScreenH(){return (*screenH);}
};