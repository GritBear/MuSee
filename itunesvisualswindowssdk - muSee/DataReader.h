#pragma once
#include "Global.h"
#include "DecodingCore.h" //cryptography
#include "lodepng.h"
//#include "TextureScriptReader\ScriptReader.h" //for reading scripts
#include "DataReaderStore.h"

/* DataReader class:
	This class provide parameters of animations and is called by all other animation related classes
	It has three model:
		1. default model: neither precompiled animations and other animations can be read
		2. precomiled animation model: precompiled animation tga pictures are available
		3. data model: read all data from encrypted files and pictures (in the "Data" folder)

	The reader will first attempt model 3. If at least one error occurs, the reader resort to previous model until it uses default model, which only support drawing squares
*/


//const unsigned int MAX_TEX = 10; //used to initialize vectors
class DataReader{
private:
	bool readingSuccessful; //if false, use model 1, if true, use model 3S
	int loadingStage; //to debug

	bool readingBackgroundSuccessful; //if false, use model 1, if true, use model 3S
	bool usingPrecompiledBackgroundVariables; //if true, use model 2
protected:
	//Animation Obj Storage Elements
	FILE *file;
	char *filename;
	int numTextures;
	string fileStringBuffer;
	string theDirectoryBuffer; //a string buffer

	////storage element
	//map<string, TextureImage*> TextureMap;
	//ReadingStatus readstatus;

	////=========================================================================================
	////String Reader functions
	////load parameters into the function from txt
	////=========================================================================================
	//void StringReadDrive();
	//TextureImage * ReadNextText(ifstream * scriptfile);
	//ReadingStatus ReadNextChunck(ifstream * scriptfile, map<string,string> * tempMap, string * NextDivider);
	//void ClearStringRead();
	//void ReadScript();

public:
	//debug value
	//temp storage parameters (need to be optimized)
	//all the textures
	deque<GLuint> loadedTexutreID;

	//keep index
	deque<int> animationTextureDQue;
	deque<int> verticalanimationTextureDQue;
	deque<int> backgroundTextureDQue;
	deque<int> flyingAnimationTextureDQue;

	deque<TextureImage> textureVec;

	DataReader();
	void Destroy();
	void Init();
	//=========================================================================================
	//Core function
	//=========================================================================================
	//-----------------------Animation Obj---------------------------------------------
	void AnimationObjFrameInfo(int Index, int &Width, int &Height, int &Max_frame, int &AnimationColumns, 
		int &MaxAnimationOrientation,bool &collidable, float &xBound, float &yBound, bool &usingTexture, GLuint &textureID, int &EntranceEffectID, bool &initialStationary, int TextureType = AnimationTexture);
	
	/*Obtain the time duration in the animation generation at melody Object*/
	void GetDiagonalLength(int AniObjIndex, float &minLength, float &maxLength,int &minTime, int &maxTime, float &MaxAngleChangeInRad);

	void AngelScaleOffsetAtOrientation(int Index, int AnimationOrientation, float &X12Radian_Scale, float &X12Radian_Offset);

	void GetStartingProbability(vector<float> &starting_probability);
	//---------------------------------------------------------------------------------


	//---------------------Vertical Animation Obj -------------------------------------
	void VerticalAnimationObjInfo(int Index, int &Width, int &Height, int &Max_frame, int &AnimationColumns, 
		int &MaxAnimationOrientation,bool &collidable, float &xBound, float &yBound, bool &usingTexture, GLuint &textureID, bool &initialStationary,
		int &EntranceEffectID);

	void GetBackgroundVerticalAnimationSpawningParameters(int Index, int &maxTime, int &minTime, int &meanTime,
		float &maxHeight, float &minHeight, float &meanHeight, float &maxAngle, float &minAngle, float &meanAngle);
	//---------------------------------------------------------------------------------


	//---------------------Background Obj----------------------------------------------
	void BackgroundInfo(int BackgroundIndex, float &percentageVel, float &XSpan, float &YSpan, int &Width, int &Height, bool &usingTexture,  GLuint &textureID);

	//---------------------------------------------------------------------------------

	//=========================================================================================
	//Loader function
	//=========================================================================================
	
	bool TextureLoadDrive();

	bool LoadRawDecodedData();
	
	bool LoadTGA(TextureImage *texture, int Option);

	bool LoadPNG(TextureImage *texture, int Option);

	void RemoveAllTextures();

	void LoadPrecompiledParameter();

	//----------------------------------------------
	//General Loader
	bool GeneralLoader(char * thename, TextureType theType, LoadingOption repeatOpt);
	bool GeneralLoader(int PreCompiledind); //specialized for pre-compiled data
	void GetPathToData(char * thename);



	//----------------------------------------------
	//General Getter
	TextureImage * GetTextureInfo(int index, TextureType texType);
	TextureImage * SimpleGetTextureInfo(int absoluteIndex);
	
	//----------------------------------------------
	//precompiled info loader
	void FillTexInfoDrive(TextureImage * texInfo);

	void FillTexInfo(TexParameter * texInfo, int index = 0);

	//=========================================================================================
	//Helper function
	//=========================================================================================
	
	bool GetUseDiagonalSprite(int Index, int &MaxDiagonalFrame, int AnimationTypeID = AnimationTexture);
	bool GetReadSuccess(){return readingSuccessful;}
	int GetMaxAniOrientation(int Index);
	void GetAniMinMaxFrameDelay(int Index, int &minDelay, int &maxDelay);
	
	int GetMaxFrame(int Index);
	int InfoOutMessage();
};

