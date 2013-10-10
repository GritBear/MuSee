//// this core should be able to decorate all other objects (without internally modify those objects)
//// 
///* 	Design thoughts:
//	it should have:
//		function/object for Background
//			- Wind
//			- Fire, shining tail
//			- Cloud, Rain, Lighting (weather, maybe a subclass?)
//			- Bubble
//			- Sun
//
//		function/object for Melody
//			- Light Up
//			- Blossoming Flower
//			- Emitting Bubble
//
//		indepenednt function/object
//			- turn a page
//*/
//
//#pragma once
//#include "Global.h"
//#include "FlyingAnimationObj.h"
//#include "VerticalAnimationObj.h"
//#include "DataReader.h"
//#include "CreationCore.h"
//#include "EventDrivenCore.h"
//
//
////each special effect module has its own creation core functions (it will have an un-changed probability throughout its life time)
//
//enum SpecialEffectIndex{Bubble, Wind, MaxSpecialEffect};
//
////this struct is used to spawn special effect obj
//typedef struct{
//	bool collidable;
//	bool bounceBack;
//	bool initialGeneration;
//	
//	bool useTexture;
//	bool useVaryingDepth;
//	
//	int themeIndex;
//
//	//Core Link
//	ControlObj *theControlObj;
//
//	//Random Generation Parameters
//	float minWidth, meanWidth, maxWidth, widthStandardDeviation; //need to be specified for random generation 
//	float minNumObj, maxNumObj; //the boundary (higher priority than generation time), negative means no constraint
//	float minTime, meanTime, maxTime, timeStandardDeviation; //random generation time
//	float minLifeTime, meanLifeTime, maxLifeTime, liftTimeStandardDeviation; //life span for each objects
//	float minSpeed, meanSpeed, maxSpeed, speedDeviation;
//	
//	float depth, minDepth, meanDepth, maxDepth, depthStandardDeviation; //random animationObj depth distribution
//	//Texture Parameters
//	//vector<float> probability;
//	//vector<GLuint> textureID;
//
//} SpecialEffectStruct;
//
//struct EventMessage {
//	float currentBeat;
//
//
//
//};
//
//
////Special Effect Super Class
//class SpecialEffectObj{
//	//support collidable and uncollidabes: uncollidables can also be used for cloud
//private:
//protected:
//	//attributes
//	bool alive;
//	bool useTexture;
//	bool useVaryingDepth;
//	bool collidable;
//	bool bounceBack;
//	bool initialGeneration;
//	bool animationIndexAlreadyChose;
//
//	//time
//	unsigned int curTimeInMS;
//	float timeAxisPercentage;
//	//spawning control
//	float timeNextObj,timeSinceLast;
//	//animation spawing variable
//	AnimationObjStruct * theAniStruct;
//
//	//screen dimension
//	float screen_w, screen_h;
//	float bottom,top,left,right;
//
//	//module indexes
//	int themeIndex;
//	int flyingModuleIndex;
//	int nextAnimationIndex;
//
//	//Core Component Link
//	ControlObj *theControlObj;
//	DataReader *theReader;
//	CreationCore *theCreationCore;
//	SpeedControl *theSpeedControl;
//	//Random Generation Parameters
//	float minWidth, meanWidth, maxWidth, widthStandardDeviation; //need to be specified for random generation 
//	float minNumObj, maxNumObj; //the boundary (higher priority than generation time)
//	float minTime, meanTime, maxTime, timeStandardDeviation; //random generation time
//	float minLifeTime, meanLifeTime, maxLifeTime, liftTimeStandardDeviation; //life span for each objects
//	float depth, minDepth, meanDepth, maxDepth, depthStandardDeviation; //random animationObj depth distribution
//	float minSpeed, meanSpeed, maxSpeed, speedDeviation;//speed generation
//
//	//------------------non struct variables------------------
//	//speed offset, usually associated with screen speed. these apply to all stored objects
//	float xVelOffset, yVelOffset;
//	//acceleration offset
//	float xAccOffset, yAccOffset;
//	
//	float currentBeat;
//	float percentageOfTheme; //obtain from the background / melody class
//	//--------------------------------------------------------
//	//storage struct
//	//EventMessage theEvent; //current message
//	bool anEventUpdate;
//
//	//Texture Parameters
//	vector<float> probability;
//	vector<GLuint> textureID;
//
//	//storage elements
//	int numInScreenObj;
//	int numAliveObj;
//	/*
//	list<AnimationObj *> AnimationObjList;
//	list<AnimationObj *>::iterator AnimationObjListIter;
//	*/
//public:
//	SpecialEffectObj(){};
//	SpecialEffectObj(SpecialEffectStruct *aSpecialEffectStruct){Init(aSpecialEffectStruct);}
//	void Init(SpecialEffectStruct *aSpecialEffectStruct);
//
//	~SpecialEffectObj(){Destroy();};
//	void Destroy();
//
//	void virtual Update(EventMessage *anEvent = NULL); //per pulse
//	void virtual Render(float Screen_w = 0, float Screen_h = 0, bool Updating = true);
//	void virtual MovementCenter(){};//this should control the movement of all existing flying objects
//	
//	//--------Special Effect Generation Modules-----------------------
//	void virtual AnimationIndex() = 0;
//	void virtual BestMatchParameters() = 0;
//	void virtual EventMove() = 0;
//	void virtual NextObjGeneration() = 0;
//	bool TimeControl();
//
//	//--------------------------------------------------------------
//	void virtual RemoveTooOldEntries(bool Removeall = false);
//
//	//Getter and Setter
//	void SetVelocityOffset(float XVel, float YVel){xVelOffset = XVel; yVelOffset = YVel;}
//	void SetAccelerationOffset(float XAcc, float YAcc){xAccOffset = XAcc; yAccOffset = YAcc;}
//	void SetCurTimeInMS(unsigned int CurrentTime){curTimeInMS = CurrentTime;}
//};
//
//class Wind{
//	//this should also support the color strips
//
//
//
//};
//
//class Smoke{
//	//the foggy atmosphere render
//
//
//};
//
//class BlossomFlower{};
//
//class EmittingBubble{};
//
////Additional Independent Special Effects (extra that helps the transition effects)
//class TurnPage {
//	//good transition for fresh new start
//
//
//
//};
//
//
