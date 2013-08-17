#pragma once
#include "Global.h"
#include "AnimationObj.h"
#include "FlyingAnimationObj.h"
#include "DataReader.h"
#include "CreationCore.h"
//#include "SpecialEffectController.h"
#include "control.h"
#include "MelodyToneAccept.h"

#include "AllAObj.h"

using namespace std;

const int maxMelodyFlyingObj = 10;
const int thousand = 1000;

class MelodyObj{
private:
	//currently empty
	bool alive;
	bool revivable;
	bool stationary;
	bool isTheHighestLine;
	bool flyingAnimationObjAlive;

	int melodyID; //in case it is useful

protected:
	//=================================================================
	// child class accessible status variables
	//=================================================================
	int currentThemeID;
	int specialStatus;
	int melodyRank; // specify the rank of this melody at its last tone aquisition
	
	//=================================================================
	// melody variables
	//=================================================================
	//time variables
	unsigned int curTimeInMS, previousTimeInMS;
	unsigned int startTimeInMS,  endTimeInMS;
	int time_elapsed;
	unsigned int previousToneTimeInMS, curToneTimeInMS;
	float pulseRate; // reader melody sampling rate
	int numPulsePerFrame; // number of frame per second

	//storage elements
	list<int> melodyLineToneList;
	list<float> effectiveMelodyLineToneList; //this is the processed line to generate animation objects
	vector<int> melodyLineToneVect;
	vector<float> curScore;
		
	// melody collision bounds
	int boundY;
	int boundX;

	//==================================================================
	// animation variables
	//==================================================================
	int screen_w, screen_h;
	float relative_w, relative_h;

	unsigned int previousAnimationEndingTimeInMS;
	float PreviousAniEndTone;

	float curTimeXPosition;
	int bottomTone;
	int topTone;
	int lowerThresholdTone;

	float x1, y1, x2,  y2; // for corners or connection points in unit of percentage of the screen
	// animation spawning location 
	float x;
	float y;

	// animation spawning velocity in unit of percentage of the screen
	float xVel;
	float yVel;

	// animation bound
	float aniXBound;
	float aniYBound;
	
	// animation orientation
	int animationOrientation;
	bool aniFlipped;


	int time_durarion_frame_delay; // pass to animation object
	
	int previous_animation_orientation;
	float previousY2;
	float previousAngleInRad;

	float SpawningThresholdRadian;

	//==================================================================
	// Animation Object Lists
	//==================================================================
	bool animationIndexAlreadyChose;
	int targetAnimationIndex;

	vector<float>probabilityVect;
	bool usingRegularAniObj;
	
	//-------------------------------------------------------------------
	// The FlyingAnimationTexture Animation Object
	
	AnimationObjStruct aFlyAniStruct;
	FlyingObjStruct aFlyingStruct;


	bool usingFlyingAniObj;
	list<FlyingAnimationObj *>::iterator flyingAniObjIter;
	//-------------------------------------------------------------------
	control * theControl; //for higher security

	// The Control Pointers
	DataReader * theReader;

	CreationCore * theCreationCore;

	SpeedControl * theSpeedControl;

	//==================================================================
	// FlyingAnimationObj Spawning Mechanism
	//==================================================================
	void SpawnMelodyFollower(int numToSpawn);
	void SpawnAniComponent(); //spawn one animation by component with arbitrary size and location

	//testing component animation
	AStruct theAniStruct;
	bool aniStructSpawnCondition;
	

public:
	//Debug Variables
	AniDispatcher * theChildren;

	deque<float> inScreenTraceX,inScreenTraceY;
	list<FlyingAnimationObj *> flyingAniObjList;
	//==================================================================
	// Melody Core Methods
	//==================================================================
	MelodyObj(control *theControlObj);

	void virtual Destroy();
	void Init(int startingTone, int actualNumberOfExtractionPulse);
	void virtual Update();
	void virtual Render(int Screen_w, int Screen_h, float RelativeW, float RelativeH, bool updating = true);
	
	//this method returns the scores for an input buffer
	vector<float> virtual AnalyseMelodyTones(vector<int> * aToneBufferVect);
	
	void virtual EffectiveToneRegistration(int tone, int rank);

	void virtual NextAnmiationInfo();
	void virtual NextAnmiationInfo(IndexThemeControl * theIndexStruct);
	void virtual NexAnimationObjGenerate();

	//=================================================================
	//Helper and Analysis Functions
	//=================================================================
	bool virtual BestMatchParameters(int AnimationIndex);

	float virtual EvaluateToneAcceptanceScore(int tone, int tone_rank);

	float virtual ToneClosenessEvaluation(int tone);

	float virtual TrendFittingEvaluation(int tone, int order);

	bool virtual CheckMelodyTouching(MelodyObj *otherMelody);

	void UpdateMelodyVector();

	void RemoveTooOldEntries(bool ChangeTrack = false, bool Removeall = false);

	//=================================================================
	// Attribute Getter and Setter
	//=================================================================	
	void AcceptTone(int tone, int rank);

	void SetIsTheHighestLine(bool isHighest){isTheHighestLine = isHighest;}
	bool GetIsTheHighestLine(){return isTheHighestLine;}

	bool Collidable(){return alive;}
	
	float GetAnimationSpawningX(){return x;}
	float GetAnimationSpawningY(){return y;}

	void SetAnimationSpawningX(float X) {MelodyObj :: x = X;}
	void SetAnimationSpawningY(float Y) {y = Y;}

	int GetMelodyBoundX(){return boundX;}
	int GetMelodyBoundY(){return boundY;}
	
	void SetTime(unsigned int TimeInMS){curTimeInMS = TimeInMS;}
	unsigned int GetTime(){ return curTimeInMS;}

	void SetAnimationSpawningXvelocity(float x_v){xVel = x_v;}
	void SetAnimationSpawningYvelocity(float y_v){yVel = y_v;}

	float GetAnimationSpawningXvelocity(){return xVel;}
	float GetAnimationSpawningYvelocity(){return yVel;}
	
	list<int>* GetToneList(){return &melodyLineToneList;}
	float GetCurrentEffectiveMelodyLineTone(){return effectiveMelodyLineToneList.back();}
	
	int GetThemeID() {return currentThemeID;}
	void SetThemeID(int Theme_id) { currentThemeID = Theme_id;}

	bool GetStationary () {return stationary;}
	void SetStationary (bool Stationary) {stationary = Stationary;}

	bool GetAlive() {return alive;}
	void SetAlive(bool Alive) {alive = Alive;}

	bool GetRevivable() {return revivable;}
	void SetRevivable(bool Revivable) {revivable = Revivable;}

	int GetSpecialStatus() {return specialStatus;}
	void SetSpecialStatus(int status) { specialStatus = status;}

	int GetNumOfStoredTones(){return melodyLineToneList.size();}
};
