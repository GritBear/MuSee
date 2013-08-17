/*
	Art Story Core. Root Header File
	It initiate all other objects and serves as the root core
	Functionalities:
	1. Initialize the plugin and OpenGL
	2. Spawning DataReader, Melody, Background and all other second to root objects
	3. Extract melody, and spawning a Rhythmic core for Rhythmic extraction
	4. Serves as the event center. Send and receive special event message

	Author: Li(Lee) Xiong. All rights reserved.
*/

#pragma once
#include "Global.h"

#include "logwriter.h"

#include "CreationCore.h"
#include "MelodyObj.h"
#include "Background.h"
#include "DataReader.h"
#include "SpecialEffectCore.h"

#include "RhythmicExtract.h"
#include "control.h"
#include "CreativeCore.h"


//=====================================================================================================================
//Visual Drawing State Machine (class): ArtStoryCore
//=====================================================================================================================
class ArtStoryCore{
private:
	bool displaying;
	bool textureLoaded;
	//public variable  bool playing indicates whether a song is playing

	//playing is in public variables
	
protected:
	int idlingPulseFreq;
	int playingPulseFreq;
	
	float screen_w;
	float screen_h;
	float relative_w,relative_h;
	//=================================================================
	// Melody variables
	//=================================================================
	unsigned int numSpectrumEntries;
	unsigned int numMaxChannels;
	unsigned int numWaveformEntries;
	
	int pulseIter;
	int pulseIterUpperLimit;
	int melodyExtractRate;   //extract melody N times a second

	vector<vector<float>> melodyStoragesVec;
	int melodyStoragesVecSize1;
	int melodyStoragesVecSize2;
	
	//frequency processing
	float FrequencyInterval;
	unsigned int MaxFrequencyIndex;
	float dynamicThreshold;

	int num_tones;
	int MaxTone;
	vector<int> FrequencyBandSeparatorVec;
	vector<int> ToneBandSeparatorVec;

	list<vector<float>> stepMelodyTestOut; //stepMelodyTestOut all step extracted tones to display
	list<vector<int>> stepMelody; // this is used to create melody and other things

	//MelodyLineObj
	list<MelodyObj *> MelodyObjList;
	list<MelodyObj *>::iterator melodyIter;
	list<MelodyObj *>::reverse_iterator RmelodyIter;
	//list<AnimationObj *>* anAniObjList;

	//=================================================================
	//Analysis methods
	//=================================================================
	void  SpectrumMatrixAnalysis();
	void  BandedNormalizeMatrix(vector<int> Separator);
	void  FilterDrive();
	float virtual FilteredEntry(unsigned int row, unsigned int col);
	
	void virtual StepToneRegistrationFromFreq(vector<float> abuffer);
	void virtual StepFreRegistration();

	void DynamicThresholdAdjustment(int num_tones);

	void virtual MelodyLineAssignment();
    
	//=================================================================
	// Rhythmic Module
	//=================================================================
	RhythmicExtract *theRhythmicExtract;

	//-----------------------------------------------------------------
	// Background Object
	Background * theBackground;


	ControlObj theControlObj;
	control* theControl;

	//-----------------------------------------------------------------
	// DataReader Object
	DataReader *theReader;

	//-----------------------------------------------------------------
	// Probabilistic Core Object
	CreationCore *theCreationCore;

	//-----------------------------------------------------------------
	SpeedControl *theSpeedControl;

	//-----------------------------------------------------------------
	// Screen, OpenGL and Bitmap variables
#if TARGET_OS_MAC

#else
	//windows screens
	HDC hDC;
	PIXELFORMATDESCRIPTOR pfd;
	HGLRC hRC;
	RECT clientRect;
#endif
	//Protected OpenGL Variables
	//the grand background is bottom = 0, top = 1, left = 0, right = 1;
	float resizedBottom, resizedTop, resizedLeft, resizedRight;

	//write to the log
	ofstream logWriteStream;
	int logMelody;
	void writeLog();

public:
	//------------------------------------------------
	// Additional public varibles
	//------------------------------------------------
	string InfoOut;
	//debug update
	void DebugUpdate();
	//int debugCnt;

	//=================================================================
	//Core Functions
	//=================================================================
	ArtStoryCore();
	void Destroy();

	void Init();

	void PulseUpdate(UInt32 TimeInMS, UInt32 timeStampID, const RenderVisualData * RenderData, UInt32 * ioPulseRate );

	void ObjectUpdate();

	void virtual Render();

	void virtual TestMelodyExtractionRender();

	void ActivateVisualizer(VISUAL_PLATFORM_VIEW destView, OptionBits options);

	void DeactivateVisualizer();
	
	void MoveVisual(OptionBits newOptions);

	void ResizeVisual();

	//=================================================================
	//OpenGL methods
	//=================================================================
	void CreatContext();
	void InitGL();
	void ClearBackground();
	void DeactiveGL();
	void ResizeGL();
	void LoadTexture();
	void AspectRatioPreserveResize();

	//=================================================================
	//Helper methods
	//=================================================================
	void TrackChanged();
	void InfoOutToScreen();
	void virtual LoadAnimationBitmapSeries(){};
	
	void UpdatePulseRates(UInt32 * ioPulseRate);
	void GetScreenDimension();
	//this method create band separator vector. frequency interval and max considered frequency
	void ConstructFrequencyBands();

	bool MelodyStore();
	int GetLatestPulseIter();

	int FreqToToneInd(float frequency);
	int ToneToFrequencyInd(float tone);

	void RemoveTooOldEntries(bool Removeall = false);

	//------------------------------------------------
	// Simple Matrix Operations
	//------------------------------------------------
	void PowerMatrix(int power);
	void ScalerMultiplySpectrumMatrix (float scaler);
	void Log10SpectrumMatrix();
	void ScalerAddMatrix(float value);
	// this will set all values below threshold to bottom, above or equal threshold to top
	void HardDecisionPolarizeRule(float Threshold, float bottom, float top);
	void HighFreqCompensation();

	//=================================================================
	//Getter and Setter
	//=================================================================

	void GetMatrixMaxMin(float &max, float &min);

	bool GetPlaying() {return playing;}
	void SetPlaying(bool Play) { playing = Play;}


	//=================================================================
	// Public Variables
	// backward compatible with the VisualPluginData Struct
	//=================================================================
	void *				appCookie;
	ITAppProcPtr		appProc;

#if TARGET_OS_MAC
	NSView*				destView;
	NSRect				destRect;
	#if USE_SUBVIEW
	VisualView*			subview;								// custom subview
	#endif
	NSImage *			currentArtwork;
#else
	HWND				destView;
	RECT				destRect;
	Gdiplus::Bitmap* 	currentArtwork;

	long int			lastDrawTime;
#endif
	OptionBits			destOptions;

	RenderVisualData	renderData;
	UInt32				renderTimeStampID;
	
	ITTrackInfo			trackInfo;
	ITStreamInfo		streamInfo;

	// Plugin-specific data
	
	bool				playing;								// is iTunes currently playing audio?
	bool				padding[3];

	time_t				drawInfoTimeOut;						// when should we stop showing info/artwork?

	UInt8				minLevel[kVisualMaxDataChannels];		// 0-128
	UInt8				maxLevel[kVisualMaxDataChannels];		// 0-128
	

};


