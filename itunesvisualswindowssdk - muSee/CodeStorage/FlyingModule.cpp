/*

#include "FlyingAnimationObj.h"
//this module includes different flying mode for flying animation object
/*
	Default: acceleration adjustment mode
	also possible:
		Low pass/hard decision trace linear mode 
*/

/*
void FlyingAnimationObj::AccelerationAdjustmentModule(){
	//this is very similar to gravitional pull
	//update accelerations
	vector<float> nextCoordinate = effectiveTraceCoordinateList.front();
	float yDistance = y - nextCoordinate[1];
	float xDistance = x - nextCoordinate[0];

	int YDistanceSign = (yDistance <= 0) ? -1 : 1;
	Yacceleration = -YDistanceSign * abs(pow(YDistanceSign,distancePullPower)) * accelerationPullConst;

	int XDistanceSign = (xDistance <= 0) ? -1 : 1;
	Xacceleration = -XDistanceSign * abs(pow(XDistanceSign,distancePullPower)) * accelerationPullConst;

	//update velocity
	yVel += Yacceleration;
	xVel += Xacceleration;

	//XVelPerturbation(); not use here
}

//this is a naive x velocity perturbation rule
void FlyingAnimationObj::XVelPerturbation(){
	if((x >= maxX && xVel > -screenXVel) || (x <= minX)){
		vector<float> nextCoordinate = effectiveTraceCoordinateList.front();
		float xDistance = x - nextCoordinate[0];
		int XDistanceSign = (xDistance <= 0) ? -1 : 1;
		Xacceleration = -XDistanceSign * abs(pow(XDistanceSign,distancePullPower)) * accelerationPullConst;
	}

	if(x >= maxX && xVel > -screenXVel){
		Xacceleration = -abs(Xacceleration) *  GaussianDistribution(0.4, 0.8, 1.2);
		xVel += Xacceleration;
	}else if(x <= minX){
		//start to accelerate xVel
		Xacceleration = abs(Xacceleration * GaussianDistribution(0.4, 0.8, 1.2));
		xVel += Xacceleration;
	}
}
*/


/*


void FlyingAnimationObj::GenerateFineTrace(bool AddAPush){
	// Necessary information:
	// TraceIntervalofDistance
	// list<float> xTraceVec, yTraceVec;
	// list<float>::iterator xTraceIter, yTraceIter;
	// Algorithm: sinusoid fit
	// generated trace curve
	// list<float> generatedTraceX, generatedTraceY;
	// fluctuationAmplitudeDegree, MaxFluctuationDegree, fluctuationPeriodDegree

	if(!AddAPush){
		//-------------------------------------------------------------------------------------
		//this part is for initialization
		if(xTraceVec.size()>0 && yTraceVec.size()>0 && yTraceVec.size()==xTraceVec.size()){
			xTraceIter = xTraceVec.begin();
			yTraceIter = yTraceVec.begin();

			//initialize the start
			FreeAll(generatedTraceX);
			FreeAll(generatedTraceY);
			
			generatedTraceX.push_back((*xTraceIter));
			generatedTraceY.push_back((*yTraceIter));
			
			xTraceIter++;
			yTraceIter++;

			while(xTraceIter!= xTraceVec.end()){
				//prepare important parameters
				float previousX = generatedTraceX.back();
				float previousY = generatedTraceY.back();
				float thisX = (*xTraceIter);
				float thisY = (*yTraceIter);

				float distance = sqrt(pow(thisX - previousX,2) + pow(thisY - previousY,2));
			
				if(distance > TraceIntervalofDistance){
					GenerateSinusoidParameters(distance);
					//calculate the generated trace
					GenerateSinusoidSegment(thisX, thisY, previousX, previousY, distance);
				}else{
					//push in the end segment point
					generatedTraceX.push_back(thisX);
					generatedTraceY.push_back(thisY);
				}
				xTraceIter++;
				yTraceIter++;
			}
			//push in the final point with a bit error checking
			if(generatedTraceX.back() != xTraceVec.back() && generatedTraceY.back()!=yTraceVec.back()){
				generatedTraceX.push_back(xTraceVec.back());
				generatedTraceY.push_back(yTraceVec.back());
			}

		}//if the tracelist size is greater than 0
	}else{
		//-----------------------------------------------------------------
		//if a push is added
		if(generatedTraceX.size() == 0){
			generatedTraceX.push_back((*xTraceIter));
			generatedTraceY.push_back((*yTraceIter));
		}else{
			//prepare important parameters
			float previousX = generatedTraceX.back();
			float previousY = generatedTraceY.back();
			float thisX = xTraceVec.back();
			float thisY = yTraceVec.back();

			float distance = sqrt(pow(thisX - previousX,2) + pow(thisY - previousY,2));
			
			if(distance > TraceIntervalofDistance){
				GenerateSinusoidParameters(distance);	
				//calculate the generated trace
				GenerateSinusoidSegment(thisX, thisY, previousX, previousY, distance);
			}else{
				//push in the final point
				generatedTraceX.push_back(thisX);
				generatedTraceY.push_back(thisY);
			}

			//push in the final point with a bit error checking
			if(generatedTraceX.back() != xTraceVec.back() && generatedTraceY.back()!=yTraceVec.back()){
				generatedTraceX.push_back(xTraceVec.back());
				generatedTraceY.push_back(yTraceVec.back());
			}
		}// if size > 0
	}// else
}

// compute the sinusoid parameters
// fluctuation degree will be used to create the y = A sin(bx) function
// fluctuationPercentage is fluctuationAmplitudeDegree in unit of percent 
void FlyingAnimationObj::GenerateSinusoidParameters(float distance){
	//fluctuation degree will be used to create the y = A sin(bx) function
	// fluctuationPercentage is fluctuationAmplitudeDegree in unit of percent 
	
	if(fluctuationAmplitudeDegree == 0){
		fluctuationPercentage = 0;
		A = 0;
		b = 1;
	}else if( abs(fluctuationAmplitudeDegree) > MaxFluctuationDegree){
		fluctuationPercentage = 1;
		A = GaussianDistribution(fluctuationPercentage/2.0, fluctuationPercentage*0.75, fluctuationPercentage);
	}else{
		fluctuationPercentage = fluctuationAmplitudeDegree/MaxFluctuationDegree;
		if(fluctuationPercentage< 1.0/32.0){
			A = GaussianDistribution(0,fluctuationPercentage/2.0,fluctuationPercentage);
		}else{
			A = GaussianDistribution(fluctuationPercentage - 1.0/32.0,fluctuationPercentage,fluctuationPercentage + 1.0/32.0);
		}
	}
	numPeriod = 0.5 + GaussianDistribution(fluctuationPeriodDegree*0.8, fluctuationPeriodDegree, fluctuationPeriodDegree*1.2);
	numPeriod = (float)numPeriod * distance + 0.5;
	b = (float)numPeriod * pi / distance;			
}

// compute the sinusoid
// compute and insert coordinates into the generated trace
void FlyingAnimationObj::GenerateSinusoidSegment(float thisX, float thisY, float previousX, float previousY, float distance){
	float accomplishedDistance = TraceIntervalofDistance;
	
	while(accomplishedDistance < distance){
		float sinOutput = A * sin(b * accomplishedDistance);
		//perform trigonometry to find the trace
		float bottomAngleRad1 = atan((thisY - previousY)/(thisX - previousX));
		float bottomAngleRad2 = atan(sinOutput/accomplishedDistance);
		float bottomAngelTotal = bottomAngleRad1 + bottomAngleRad2;
		float diagonalLength = sqrt(pow(sinOutput,2) + pow(accomplishedDistance,2));

		float xIncrewment = diagonalLength * cos(bottomAngelTotal);
		float yIncrewment = diagonalLength * sin(bottomAngelTotal);
				
		generatedTraceX.push_back(previousX + xIncrewment);
		generatedTraceY.push_back(previousY + yIncrewment);

		accomplishedDistance += TraceIntervalofDistance;
	}

	//push in the end segment point
	if(generatedTraceX.back() != thisX){
		generatedTraceX.push_back(thisX);
		generatedTraceY.push_back(thisY);
	}
}


void FlyingAnimationObj::ComputeFlyingCoordinates(){
	//available information
	// diagonal length (only meaningful if it is greater not 0
	if(halfDiagonal != 0){
		if(!onEnd){
		//compute the angle here
			currentXLocationOnTrace++;
			currentYLocationOnTrace++;

			float nextX = (*currentXLocationOnTrace);
			float nextY = (*currentYLocationOnTrace);

			currentXLocationOnTrace--;
			currentYLocationOnTrace--;

			flyingAngle = atan((nextY - y) / (nextX - x)); //otherwise, flyingAngle remains the same
		}

		float xDifference, yDifference; //the difference between x and x1, y and y1
		xDifference = -halfDiagonal * cos(flyingAngle);
		yDifference = -halfDiagonal * sin(flyingAngle);
		
		x1 = x + xDifference;
		y1 = y + yDifference;

		x2 = x - xDifference;
		y2 = y - yDifference;
			
		//x1, y1 and x2, y2 should be already prepared
		AnimationObj::ComputeCoordinate();	
	}
}

//update the moving trace step by step moving on predetermined intervals
void FlyingAnimationObj::TraceMovementUpdate(){
	if( !onStart && !onMiddle && !onEnd){
		//this means the trace movement is not initialized yet
		if(generatedTraceX.size()>1 && generatedTraceY.size()>1){
			//means the object is just initiated
			onStart = true;
			currentXLocationOnTrace = generatedTraceX.begin();
			currentYLocationOnTrace = generatedTraceY.begin();

			x = (*currentXLocationOnTrace);
			y = (*currentYLocationOnTrace);

			currentXLocationOnTrace++;
			currentYLocationOnTrace++;
		}
	}else if(currentXLocationOnTrace != generatedTraceX.end() && currentYLocationOnTrace != generatedTraceY.end()){
		onMiddle = true;
		onStart = false;

		x = (*currentXLocationOnTrace);
		y = (*currentYLocationOnTrace);

		currentXLocationOnTrace++;
		currentYLocationOnTrace++;
	}
	//final checking (this includes the case generatedTraceX.size()<3)
	else{
		onStart = false;
		onMiddle = false;
		onEnd = true;

		x = (*currentXLocationOnTrace);
		y = (*currentYLocationOnTrace);
	}
}



*/