/*
enum FlyingMode{AccelerationAdjust, MaxFlyingMode};


*/


	//float accelerationPullConst;
	
	//float distancePullPower; //default 1: the acceleration is linear with distance (can be 0.5, 2 or other value)

	//trace storage elements
	/*
	list<vector<float>> effectiveTraceCoordinateList; //this only store the up-coming trace coordinates (not removed by traceAchievingLimit), each vector should have three values (X, Y)
	list<vector<float>> fullTraceCoordinateList; //stores the trace coordinates for a period of time (usually to x = -1.5), each vector should have three values (X, Y)
	list<vector<float>> ::iterator TraceIter;
	*/

	//float traceAchievingLimit;

	
	//bool onlyUseXTraceAchievingTest; //if true, only test the x coordinates to remove effective trace coordinate (otherwise, test the radius distance)

	//FlyingAnimationTexture Module
	//void AccelerationAdjustmentModule();
	//void XVelPerturbation();

	//book keeping functions
	//void RemoveTooOldEntries(bool removeall);
	//void EffectiveCoordinateRemove();
	//void AcceptTraceCoordinate(float X, float Y);

	//void SetFlyingMode(int Mode){flyingModeInd = Mode;}
	/*
	float GetTraceAchievingRadius(){return traceAchievingLimit;}
	void SetTraceAchievingRadius(float R){traceAchievingLimit = R;}
	*/

/*void FlyingAnimationObj::AcceptTraceCoordinate(float X, float Y){
	vector<float> coordinatePair(2,0);
	coordinatePair[0] = X;
	coordinatePair[1] = Y;

	effectiveTraceCoordinateList.push_back(coordinatePair);
	fullTraceCoordinateList.push_back(coordinatePair);
}


void FlyingAnimationObj::RemoveTooOldEntries(bool removeall){
	//keep the trace list organized
	if(fullTraceCoordinateList.size()>0 && fullTraceCoordinateList.front()[0] < -1.5)
		fullTraceCoordinateList.pop_front();
}

void FlyingAnimationObj::EffectiveCoordinateRemove(){
	bool keepRemoving = true;
	//remove all achieved coordinates
	while(keepRemoving){
		if(effectiveTraceCoordinateList.size()>0){
			vector<float> coordinatePair = effectiveTraceCoordinateList.front();
			float distancesquare;
			if(!onlyUseXTraceAchievingTest){
				distancesquare = pow(coordinatePair[0] - x,2)+ pow(coordinatePair[1] - y,2);
			}else
				distancesquare = pow(coordinatePair[0] - x,2);
		
			//remove achieved coordinates
			if(distancesquare < pow(traceAchievingLimit,2))
				effectiveTraceCoordinateList.pop_front();
			else
				keepRemoving = false;

		}else{keepRemoving = false;}
	}
}

*/




//inside update
//update the trace coordinates
	/*
	if(effectiveTraceCoordinateList.size()>0){
		for(TraceIter = effectiveTraceCoordinateList.begin();TraceIter != effectiveTraceCoordinateList.end(); ++TraceIter){
			(*TraceIter)[0] += screenXVel;
			(*TraceIter)[1] += screenYVel;
		}
		EffectiveCoordinateRemove();
	}

	if(fullTraceCoordinateList.size()>0){
		for(TraceIter = fullTraceCoordinateList.begin();TraceIter != fullTraceCoordinateList.end(); ++TraceIter){
			(*TraceIter)[0] += screenXVel;
			(*TraceIter)[1] += screenYVel;
		}
	}
	*/
