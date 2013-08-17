#include "DataReader.h"

void DataReader::VerticalAnimationObjInfo(int Index, int &Width, int &Height, int &Max_frame, int &AnimationColumns, 
	int &MaxAnimationOrientation,bool &collidable, float &xBound, float &yBound, bool &usingTexture, GLuint &textureID, bool &initialStationary,
	int &EntranceEffectID){
	
	bool thisReadingSuccessful = true;

	//use pre-compiled values value if data reading is unsuccessful
	if(Index>=verticalanimationTextureDQue.size()){
		thisReadingSuccessful = false; //illegal index input, use roll back to default
	}else{
		int texturePosition = verticalanimationTextureDQue[Index];
		textureID = textureVec[texturePosition].texID;

		Width = textureVec[texturePosition].width;
		Height = textureVec[texturePosition].height;

		Max_frame = verticalDefaultMaxFrame;

		AnimationColumns = verticalDefaultAnimationColumns;
		MaxAnimationOrientation = verticalDefaultMaxAnimationOrientation;

		collidable = verticalDefaultCollidable;
		xBound = verticalDefaultXBound;
		yBound = verticalDefaultYBound;

		EntranceEffectID = Vertical;
		usingTexture = true;
		initialStationary = false;
	}
	

	////default square drawing
	//if(!thisReadingSuccessful){
	//	//use default value if data reading is unsuccessful
	//	Width = defaultFrameWidth;
	//	Height = defaultFrameHeight;
	//	Max_frame = defaultMaxFrame;
	//	AnimationColumns = defaultAnimationColumns;
	//	MaxAnimationOrientation = defaultMaxAnimationOrientation;
	//	collidable = defaultCollidable;
	//	xBound = defaultXBound;
	//	yBound = defaultYBound;

	//	EntranceEffectID = NoEffect;

	//	usingTexture = false;
	//	textureID = NULL;
	//	initialStationary = true;	
	//}
}


void DataReader::GetBackgroundVerticalAnimationSpawningParameters(int Index, int &maxTime, int &minTime, int &meanTime,
	float &maxHeight, float &minHeight, float &meanHeight, float &maxAngle, float &minAngle, float &meanAngle){
	maxTime = defaultMaxTime;
	minTime = defaultMinTime;
	meanTime = defaultMeanTime;

	maxHeight = defaultMaxHeight;
	minHeight = defaultMinHeight;
	meanHeight = defaultMeanHeight;

	maxAngle = defaultMaxAngle;
	minAngle = defaultMinAngle;
	meanAngle = defaultMeanAngle;
	
}