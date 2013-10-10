//#pragma once
//#include "EventDrivenCore.h"
//#include "AnimationObj.h"
//
//class AnimationObjCollisionEventCore : public EventDrivenCore{
//private:
//	list<AnimationObj *> * ptrToAniList;
//	list<AnimationObj *>::iterator aniListIter;
//	list<AnimationObj *>::reverse_iterator raniListIter;
//protected:
//public:
//	AnimationObjCollisionEventCore(){};
//	AnimationObjCollisionEventCore(list<AnimationObj *> * PtrToAniList, float TimeIncrew = TimeStepInSecond, float StartingTime = DefaultStartingTime)
//	{ptrToAniList = PtrToAniList;EventDrivenCore::Init(TimeIncrew, StartingTime);}
//
//	~AnimationObjCollisionEventCore(){EventDrivenCore::Destroy();}
//
//	void Init(); //initial O(n^2) operation
//	void Predict(AnimationObj * TargetObj); //this will loop through the entire list, and then push into the priority queue
//	float TimeToHit(AnimationObj * thisParticle, AnimationObj * thatParticle);
//};