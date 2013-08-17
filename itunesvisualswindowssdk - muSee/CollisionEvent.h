#pragma once
#include "EventDrivenCore.h"

class CollisionEvent : public EventDrivenCore<AnimationObj>{
private:
protected:
public:
	CollisionEvent(list<AnimationObj *> * theAniList);
	void Predict(AnimationObj * TargetObj);

};