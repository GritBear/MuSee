#include "DataReader.h"

void DataReader::AnimationObjFrameInfo(int Index, int &Width, int &Height, int &Max_frame, int &AnimationColumns, 
		int &MaxAnimationOrientation, bool &collidable, float &xBound, float &yBound, bool &usingTexture, GLuint &textureID, int &EntranceEffectID, bool &initialStationary, int TextureType)
{
	//use pre-compiled values value if data reading is unsuccessful
	if(Index>=textureVec.size())
		readingSuccessful = false; //illegal index input, use roll back to default
	else{
		int texturePosition = animationTextureDQue[Index];
		textureID = textureVec[texturePosition].texID;
		
		Width = textureVec[texturePosition].width;
		Height = textureVec[texturePosition].height;

		Max_frame = abstractLeefMaxFrame;

		AnimationColumns = abstractLeefAnimationColumns;
		MaxAnimationOrientation = abstractLeefMaxAnimationOrientation;

		collidable = abstractLeefCollidable;
		xBound = abstractLeefXBound;
		yBound = abstractLeefYBound;

		EntranceEffectID = Diagonal; 

		usingTexture = true;
		initialStationary = false;
	}
	//default square drawing
	if(!readingSuccessful){
		//use default value if data reading is unsuccessful
		Width = defaultFrameWidth;
		Height = defaultFrameHeight;
		Max_frame = defaultMaxFrame;
		AnimationColumns = defaultAnimationColumns;
		MaxAnimationOrientation = defaultMaxAnimationOrientation;
		collidable = defaultCollidable;
		xBound = defaultXBound;
		yBound = defaultYBound;

		EntranceEffectID = NoEffect;

		usingTexture = false;
		textureID = NULL;
		initialStationary = true;	
	}
}

void DataReader::GetDiagonalLength(int AniObjIndex, float &minLength, float &maxLength,int &minTime, int &maxTime, float &MaxAngleChangeInRad){
	if(!readingSuccessful){
		//use default value if data reading is unsuccessful
		minLength = defaultMinDiagLength;
		maxLength = defaultMaxDiagLength;
		minTime = defaultMinTimeInMS;
		maxTime = defaultMaxTimeInMS;

		MaxAngleChangeInRad = NotAnAngle;

	}else{
		//use default value if data reading is unsuccessful
		minLength = verticalDefaultMinDiagLength;
		maxLength = verticalDefaultMaxDiagLength;
		minTime = verticalDefaultMinTimeInMS;
		maxTime = verticalDefaultMaxTimeInMS;

		MaxAngleChangeInRad = 0.25 * pi;
	}
}

void DataReader::AngelScaleOffsetAtOrientation(int Index, int AnimationOrientation, float &X12Radian_Scale, float &X12Radian_Offset){
	
	if(!readingSuccessful){
		// by default this is y axis flip (not tested) and is square (otherwise, zero ground angle need to be considered)
		X12Radian_Scale = 1;

		if(AnimationOrientation == 0){
			X12Radian_Offset = 0;
		}else{
			X12Radian_Offset = 0.5 * pi;
		}
	}else{
		// by default this is y axis flip (not tested) and is square (otherwise, zero ground angle need to be considered)
		X12Radian_Scale = 1;

		if(AnimationOrientation == 0){
			X12Radian_Offset = 0;
		}else{
			X12Radian_Offset = 0.5 * pi;
		}
	}
	
}

void DataReader::GetStartingProbability(vector<float> &starting_probability){
	//an error prevention check
	if(numTextures<1)
		numTextures = 1;

	vector<float>astarting_probability(numTextures,0);

	astarting_probability[0] = 1;
	starting_probability = astarting_probability;
	

	if(!readingSuccessful){
		astarting_probability[0] = 1;
		starting_probability = astarting_probability;

	}
};

int DataReader::GetMaxAniOrientation(int Index){
	int maxOrientations;
	if(!readingSuccessful){
		return defaultMaxAnimationOrientation;
	}else {
		return abstractLeefMaxAnimationOrientation;
	}
}

void DataReader::GetAniMinMaxFrameDelay(int Index, int &minDelay, int &maxDelay){
	if(!readingSuccessful){
		maxDelay = defaultMaxDelayFrame;
		minDelay = defaultMinDelayFrame;
	}else{
		maxDelay = abstractLeefMaxDelayFrame;
		minDelay = abstractLeefMinDelayFrame;
	}
}

//use simple diagonal sprite animation (default is true)
bool DataReader::GetUseDiagonalSprite(int Index, int &MaxDiagonalFrame, int AnimationTypeID){
	MaxDiagonalFrame = abstractLeefMaxFrame;
	return true;
}