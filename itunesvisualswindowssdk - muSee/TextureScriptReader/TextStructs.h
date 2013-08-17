#pragma once
#include <string>
#include <deque>
#if TARGET_OS_MAC
#include<gl/GL.h>
#include<gl/GLU.h>

#else

#include<Windows.h>
#include<WinGDI.h>
#include<gl/GL.h>
#include<gl/GLU.h>

#endif

using namespace std;

namespace ScriptReaderStore{
	//Define path
	#if TARGET_OS_MAC
		const string intermediateFilePath = "\\Plug-Ins\\muSee\\Data\\"; //May Need Rewriting
	#else
		const string intermediateFilePath = "\\Plug-Ins\\muSee\\Data\\";
	#endif


	//const strings for string processing
	const string kvDiv = "=";
	const string scriptName = "script.dat";
	const string nextStringDivider = "TextureFinished";

	enum LoadingOption{X_Clamp, X_Repeat};
	enum TextureType {AnimationTexture, BackgroundTexture, VerticalAnimationTexture, FlyingAnimationTexture, TotalTextureTypes};
	enum EntranceEffect{NoEffect, Diagonal, Vertical, fading, MaxEntranceEffect};
	enum DrawUnitShape{rect_dia, rect_ver, triangle, circle, polygon};
	enum ReadingStatus{fileFinishedFailure,fileNotOpenFailure,successRead}; 

	//Read Struct: generic return struct
	struct TexParameter {
		//basic parameters
		unsigned int max_frame;
		unsigned int animationColumns;

		bool usingTexture;
		bool collidable;
		bool initialStationary;

		float xBound;
		float yBound;
	
		DrawUnitShape texShape;
		EntranceEffect entranceEffectID;
	
		//spawning parameters
		float minLength;
		float maxLength;

		//spawning
		float minTimeInS;
		float maxTimeInS;

		//Entrance effect
		float minEntranceTime;
		float maxEntranceTime;

		//time in seconds
		float minExitTime;
		float maxExitTime;

		//constraints
		float max_AngleChangeInRad;

		//Orientation is unnecessary because there are only 2 possible orientation

		//advanced parameters
		//orientational parameters
		//unsigned int maxAnimationOrientation;
		//deque<float> * diagonalRadianScale;
		//deque<float> * diagonalRadianOffset;
	};

	//Define the struct to store openGL texture
	struct TextureImage                             // Create A Structure
	{
		string texName;								//name of the struct

		GLuint  bpp;                                // Image Color Depth In Bits Per Pixel.
		GLuint  width;                              // Image Width
		GLuint  height;                             // Image Height
		GLuint  texID;                              // Texture ID Used To Select A Texture
	
		// enum attribute
		TextureType texTypeID;						
		LoadingOption texRepeatOpt;	

		//other loading attributes (generic attributes)
		TexParameter texParameter;

	} ;                                 // Structure Name
	
	//loaded struct
	struct LoadedTextCollection {
		deque<GLuint> loadedTexutreID;

		//keep index
		deque<int> animationTextureDQue;
		deque<int> verticalanimationTextureDQue;
		deque<int> backgroundTextureDQue;
		deque<int> flyingAnimationTextureDQue;

		deque<TextureImage> textureVec;
		map<string,TextureImage> textureMap;
	};
}