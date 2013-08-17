//this class provide the event driven collision
#pragma once;
#include "Global.h"


struct compare  
{  
  bool operator()(const float& l, const float& r)  
  {  
      return l > r;  
  }  
}; 

//has to be implemented in templates
const float DefaultStartingTime = -10e5;

//an implementation without using template
class EventDrivenCore{
private:
protected:
	//this is a min queue
	priority_queue<float,vector<float>, compare> pq;

	//priority_queue<float,vector<float>, greater<float>> * currentPQueuePtr; 
	float curTime, timeIncrew, startingTime;
	float dx, dy, dvx, dvy, r1, r2;
public:
	//constructor and destructor
	EventDrivenCore(){};
	void Init(float TimeIncrew = TimeStepInSecond, float StartingTime = DefaultStartingTime);
	~EventDrivenCore(){Destroy();}
	void Destroy();

	virtual void Predict(){}; //this will loop through the entire list, and then push into the priority queue
	virtual float TimeToHit();
	bool NextMin(float &minValue);
	bool CheckingUpdate();
	void InsertEvent(float happeningTime);

	//getter and setter
	float GetCurTime(){return curTime;}
	void SetTimeInc(float inc){timeIncrew = inc;}
	float GetStartingTime(){return startingTime;}
};