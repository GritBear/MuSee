#include "ArtStoryCore.h"

//write to the log
void ArtStoryCore::writeLog(){
	if(!logWriteStream.is_open()){
		logWriteStream = museelogwriter::CreateMuSeeLogFile();
		logWriteStream << "SampleRate:" + num2str(ExtractRate) +" Per Second";
		logWriteStream << "\n";//separater
	}else{
		//write stuffs in
		//first write melody
		string tempString;
		
		int currentLevels = 0;
		float tempFloat = 0;
		for(int i = 0; i < theRhythmicExtract->GetInitializedLevel(); i++){
			if(theRhythmicExtract->currentBeatMagnitudeArray[i]!=0){
				//InfoOut += num2str(i);
				tempFloat = theRhythmicExtract->currentBeatMagnitudeArray[i];
				currentLevels++;
			}
		}
		tempString = "beatlevel:" + num2str(currentLevels) + ";beatmagnitude:" + num2str(tempFloat)+";ToneIndex:" + num2str(logMelody);
		
		logWriteStream << tempString;
		logWriteStream << "\n";//separater
	}
}