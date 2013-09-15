#include "ObjSpeedCore.h"

void SpeedControl::Init(){
	soundPlaying = nullptr;
	visualPlaying = nullptr;

	Reset();

	speedScaler = 1;
	mainSpeed = -1000.0/(float)defaultDisplayTimeInMS; //per second
	normalizedCurSpeed = mainSpeed / kPlayingPulseRateInHz; //default
}

void SpeedControl::Reset(){
	time_total = 0;
	time_visualsound = 0;
	time_visual = 0;
	time_elapsed = 0;
	time_visualsound_elapsed = 0;
	time_visual_elapsed = 0;

	fps = 0;
	ftpTemp = 0;
	fps_t = 0;

	t = clock();
	VectorZero(&screenSpeed);
	screenSpeed.X = -1000.0/(float)defaultDisplayTimeInMS;
}

void SpeedControl::Update(){
	clock_t tempT = clock() - t;
	t = clock();
	time_elapsed = ((float)tempT)/CLOCKS_PER_SEC ;

	if(time_elapsed < maxTimeStep){ //otherwise consider activat/de-activate interval
		time_total += time_elapsed;
		//update conditional time variables
		if((*visualPlaying)){
			time_visual += time_elapsed;
			time_visual_elapsed = time_elapsed;

			if((*soundPlaying))
				time_visualsound += time_elapsed;
				time_visualsound_elapsed = time_elapsed;
		}

		//update ftp here
		if(fps_t < 1){
			ftpTemp++;
			fps_t += time_elapsed;
		}else{
			fps = ftpTemp;
			fps_t = 0;
			ftpTemp = 0;
		}

		//scale the time
		timeScaled = speedScaler * time_elapsed;
		timeVisualScaled = speedScaler * time_visual_elapsed;
		timeVisualSoundScaled = speedScaler * time_visualsound_elapsed;

		noramlizedScreenSpeed = screenSpeed;
		VectorMult(&noramlizedScreenSpeed,time_elapsed);
	}
}

//speed only when it is playing and displaying
float SpeedControl::SpeedNormalization(float SpeedPerSecond,SpeedActiveTime amode){
	switch(amode){
	case Playing:
		return SpeedPerSecond * timeVisualSoundScaled;
			//(visualPlaying != nullptr && (*soundPlaying)) ? time_visualsound_elapsed:0;
	case Rendering:
		return SpeedPerSecond * timeVisualScaled;
			//(visualPlaying != nullptr && (*visualPlaying)) ? time_visual_elapsed : 0;
	case Both:
		return SpeedPerSecond * timeScaled;
			//( (visualPlaying != nullptr) ? time_visualsound_elapsed:time_elapsed);
	}
	
	//reset the time values
	time_visualsound_elapsed = 0;
	time_visual_elapsed = 0;
	time_elapsed= 0;
	
	//normalize the background speed
	normalizedCurSpeed = mainSpeed * timeScaled;
}

void SpeedControl::SpeedScaleHold(SpeedHolder aHolder){


}



//returns speed per Pulse
float MelodyAnimationObjSpeed(){ //return velocity per second
	return -1000.0/(float)defaultDisplayTimeInMS;//1000.0 /((float)kPlayingPulseRateInHz);
}

//returns speed per Pulse
float BackgroundSpeed(int UpdatePerSecond, float BackgroundXSpan, float OpenGLXRange){
	return -MelodyAnimationObjSpeed()/2.0;
}

//return speed per Pulse (this has to be synchronized with the background)
float BackGroundVerticalAnimationObjSpeed(){
	return MelodyAnimationObjSpeed();
}

