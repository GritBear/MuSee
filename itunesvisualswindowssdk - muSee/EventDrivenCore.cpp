#include "EventDrivenCore.h"

//constructor
void EventDrivenCore::Init(float TimeIncrew, float StartingTime){
	timeIncrew = TimeIncrew;
	startingTime = curTime = StartingTime;
};

void EventDrivenCore::Destroy(){
	//clear the priority queues
	FreeAll<priority_queue<float,vector<float>, compare>>(pq);
}

//individual time computation
float EventDrivenCore::TimeToHit(){	
	//second level test
	float dvdr = dx * dvx + dy * dvy;
	if (dvdr > 0) return INF;

	//third level test
	float dvdv = dvx * dvx + dvy * dvy;
	float drdr = dx*dx + dy*dy;
	float sigma = r1 + r2;
	float d = (dvdr * dvdr) - dvdv * (drdr - sigma * sigma);
	if(d<0) return INF;

	//final return time
	return -(dvdr + sqrt(d)) / dvdv;
}

//Checking while update the times
bool EventDrivenCore::CheckingUpdate(){
	float nextMin, hasNext;
	curTime += timeIncrew;
	//check if it has next min and pop the value if exist
	if(!NextMin(nextMin)) return false;

	//check if the next min time is less than current time
	if(nextMin <= curTime){
		//pop all nextMin that is less than curTime
		do{
			pq.pop();
			hasNext = NextMin(nextMin);
		}while(hasNext && (nextMin <= curTime));
		return true;
	}
	else
		return false;
}

//return the next min value (if exist, return TRUE)
bool EventDrivenCore::NextMin(float &minValue){
	//return a boolean to validate the result
	if(pq.empty()){
		minValue = NULL;
		return false;
	}

	minValue = pq.top();
	return true;
};

void EventDrivenCore::InsertEvent(float happeningTime){
	if(happeningTime!= INF){
		pq.push(happeningTime);
	}
}