#include "AnimationComponents.h"
/*
//add acceleration to speed queue
void AMotion::AccelerationAll(deque<float>* acc){
	for(int i = 0; i < speed.size(); i++){
		speed[i] += (*acc)[i];
	}
}

void AMotion::Acceleration2(float dim1Acc, float dim2Acc){
	speed[0] += dim1Acc;
	speed[1] += dim2Acc;
}

void AMotion::PullForce(deque<float>* force){
	for(int i = 0; i < speed.size(); i++){
		speed[i] += (*force)[i] * inverseMass;
	}
}

//set speed constraints
void AMotion::SetConstriant(deque<float>* MinSpeed, deque<float>* MaxSpeed){
	if((*MinSpeed).size() == totalDimension && (*MaxSpeed).size() == totalDimension){
		speedConstrainted = true;
		minSpeed.resize(totalDimension);
		maxSpeed.resize(totalDimension);
		for(int i = 0; i < totalDimension; i++){
			minSpeed[i] = (*MinSpeed)[i];
			maxSpeed[i] = (*MaxSpeed)[i];
		}
	}
}

//powerful overwriter (redefining the animation object)
void AMotion::SetPosition(deque<float> * newPositionQ){
	for(int i = 0; i < totalDimension; i++){
		position[i] = (*newPositionQ)[i];
	}
}

void AMotion::SetSpeed(deque<float> * newSpeedQ){
	for(int i = 0; i < totalDimension; i++){
		speed[i] = (*newSpeedQ)[i];
	}
}
*/




