#pragma once
#include "Global.h"
#include "DataReader.h"
#include "CreationCore.h"
#include "control.h"

class MelodyToneAccept{
private:
protected:
	//Storage members
	// melody collision bounds
	int boundY;
	int boundX;

public:
	MelodyToneAccept(){Init();}
	~MelodyToneAccept(){Destroy();}
	
	void Destroy();
	void Init();
	/*
	float virtual EvaluateToneAcceptanceScore(int tone, int tone_rank);

	float virtual ToneClosenessEvaluation(int tone);

	float virtual TrendFittingEvaluation(int tone, int order);

	bool virtual CheckMelodyTouching(MelodyObj *otherMelody);
	
	void UpdateMelodyVector();

	void AcceptTone(int tone, int rank, unsigned time);

	void Clean(bool ChangeTrack = false, bool Removeall = false);

	int GetMelodyBoundX(){return boundX;}
	int GetMelodyBoundY(){return boundY;}
	*/

};