#include "StatisticalFunctions.h"
using namespace std;

//this returns a random value based on Gaussian Distribution and 
float GaussianDistribution(float LowerBound, float Mean, float UpperBound, float StandardDeviation){
	int rand_precision_int;
	float rand_select, range, second_rand, pdf_output;
	bool notDone = true;

	if( (StandardDeviation == NULL) || (StandardDeviation <= 0) ){
		StandardDeviation = min(Mean - LowerBound, UpperBound -  Mean);
	}
	//check errors
	if(StandardDeviation <= 0 || UpperBound<=LowerBound || LowerBound>Mean)
		return Mean;

	rand_precision_int = 1000;
	range = UpperBound - LowerBound;

	/* initialize random seed: it is already initialized and repeated initialization is a bad practice*/
	//srand((unsigned)time(NULL));

	//producing the gaussian distributed value
	while(notDone){
		rand_select = rand() % (rand_precision_int + 1);
		rand_select = rand_select/((float)rand_precision_int) * range;
		rand_select += LowerBound;

		second_rand = rand() % (rand_precision_int + 1);
		second_rand = second_rand / ((float)rand_precision_int);
		pdf_output = 0.3989422804014326779399460599/StandardDeviation * exp(-0.5 * pow((Mean - rand_select)/StandardDeviation ,2));

		if(second_rand <= pdf_output){
			notDone = false;
		}
	}

	return rand_select;
}

//return a value from uniform distribution
float UniformDistribution(float LowerBound, float UpperBound){
	float value = (rand() % 10000)/10000.0;
	return (LowerBound + (UpperBound - LowerBound) * value);
}
