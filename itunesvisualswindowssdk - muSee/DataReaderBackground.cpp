#include "DataReader.h"

void DataReader::BackgroundInfo(int BackgroundIndex, float &percentageVel, float &XSpan, float &YSpan, int &Width, int &Height, bool &usingTexture,  GLuint &textureID){

		if(BackgroundIndex>=backgroundTextureDQue.size())
			readingSuccessful = false; //illegal index input, use roll back to default
		else{
			int theTexturePosition = backgroundTextureDQue[BackgroundIndex];
			textureID = textureVec[theTexturePosition].texID;

			Width = textureVec[theTexturePosition].width;
			Height = textureVec[theTexturePosition].height;

			percentageVel =  defaultPercentageVel;
			XSpan = defaultXSpan;
			YSpan = defaultYSpan;

			usingTexture = true;
		}

	if(!readingSuccessful){
		percentageVel = 0;
		XSpan = 0;
		YSpan = 0;

		Width = 0;
		Height = 0;

		usingTexture = false;
		textureID = NULL;
	}

}
