//#pragma once
//#include "SpecialEffectCore.h"
//#include "CollidableBubble.h"
//#include "AnimationObjCollisionEventCore.h"
//
//struct BubbleEffectStruct {
//	SpecialEffectStruct *aSpecialEffectStruct;
//	bool useDefaultParameters;
//	float minRotation, meanRotation, maxRotation, RotationDeviation;
//};
//
////Background Special Effects
//class BubbleEffect : SpecialEffectObj{
//	//support collidable and uncollidabes: uncollidables can also be used for cloud
//private:
//protected:
//	int ID;
//	AnimationObjCollisionEventCore * theCollisionCore;
//	float minRotation, meanRotation, maxRotation, RotationDeviation;
//
//	//BubbleEffectStruct theBubbleEffectStruct;
//	list<AnimationObj *> AnimationObjList;
//	list<AnimationObj *>::iterator AnimationObjListIter;
//public:
//	BubbleEffect(BubbleEffectStruct * aBubbleStruct);
//	~BubbleEffect();
//
//	//Override Methods----------------------------------------------------
//	void Update();
//	void Render(float Screen_w = 0, float Screen_h = 0, bool Updating = true);
//
//	void AnimationIndex();
//	void MovementCenter();
//	void EventMove();
//
//	void NextObjGeneration();
//	void BestMatchParameters();
//
//	void RemoveTooOldEntries(bool Removeall = false);
//	//---------------------------------------------------------------------
//	// Getter and Setter
//	int GetID(){return ID;}
//	void SetID(int id){ID = id;}
//};
