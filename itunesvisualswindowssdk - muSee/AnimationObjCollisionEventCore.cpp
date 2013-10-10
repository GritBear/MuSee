//#include "AnimationObjCollisionEventCore.h"
////initial O(n^2) computation
//void AnimationObjCollisionEventCore::Init(){
//	//call destroy to free the entire queue
//	EventDrivenCore::Destroy();
//	
//	float timeToHit;
//	//initialize the collision queue for every element
//	for(aniListIter = (*ptrToAniList).begin(); aniListIter!= (*ptrToAniList).end(); ++aniListIter){
//		for(raniListIter = (*ptrToAniList).rbegin(); (*raniListIter)!=(*aniListIter);++raniListIter){
//			timeToHit = TimeToHit((*aniListIter), (*raniListIter));
//			InsertEvent(timeToHit);
//		}
//	}
//}
//
////individual prediction
//void AnimationObjCollisionEventCore::Predict(AnimationObj * TargetObj){
//	//this will loop through the entire list, and then push into the priority queue
//	float timeToHit;
//	for(aniListIter = (*ptrToAniList).begin(); aniListIter!= (*ptrToAniList).end(); ++aniListIter){
//		timeToHit = TimeToHit(TargetObj, (*aniListIter));
//		InsertEvent(timeToHit);
//	}
//}
//
////pair collision time computation
//float AnimationObjCollisionEventCore::TimeToHit(AnimationObj * thisParticle, AnimationObj * thatParticle){
//	//first level test
//	if( thisParticle == thatParticle) return INF;
//	
//	//define the parameters before call for time to hit
//	//float dx, dy, dvx, dvy, r1, r2;
//	dx = thisParticle->GetX() - thatParticle->GetX();
//	dy = thisParticle->GetY() - thatParticle->GetY();
//
//	dvx = thisParticle->GetXVel() - thatParticle->GetXVel();
//	dvy = thisParticle->GetYVel() - thatParticle->GetYVel();
//
//	r1 = thisParticle->GetCollisionRadius();
//	r2 = thatParticle->GetCollisionRadius();
//
//	return EventDrivenCore::TimeToHit();
//}