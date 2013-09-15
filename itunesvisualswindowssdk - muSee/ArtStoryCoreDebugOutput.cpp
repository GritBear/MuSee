#include "ArtStoryCore.h"

//this method is called in Render() to display debugging data
void ArtStoryCore::DebugUpdate(){
	/*
	int size = (*MelodyObjList.front()).inScreenTraceX.size();
	InfoOut = num2str(size);
	InfoOut += "\n";
	if(size>0)
		InfoOut += num2str( (*MelodyObjList.front()).inScreenTraceX[size - 1]);
	
	InfoOut = num2str(theBackground->flyingAniObjList.size());
	if(theBackground->flyingAniObjList.size()>0){
		float X1,X2,Y1,Y2, X12,Y12,X22,Y22;
		InfoOut += "\n";
		InfoOut += num2str(theBackground->flyingAniObjList.back()->GetDebuggingValue2());
		InfoOut += "  ";
		InfoOut += num2str(theBackground->flyingAniObjList.back()->GetDebuggingValue());		
		InfoOut += "\n";

		InfoOut += num2str(theBackground->flyingAniObjList.back()->GetID());
		InfoOut += "  AX";		
		InfoOut += num2str(theBackground->flyingAniObjList.back()->GetAnchorX());
		InfoOut += "  AY:";
		InfoOut += num2str(theBackground->flyingAniObjList.back()->GetAnchorY());
		InfoOut += "  X:";		
		InfoOut += num2str(theBackground->flyingAniObjList.back()->GetX());
		InfoOut += "  Y:";
		InfoOut += num2str(theBackground->flyingAniObjList.back()->GetY());

		InfoOut += "\n";
		theBackground->flyingAniObjList.back()->GetFourCorners(X1,Y1,X12,Y12,X2,Y2,X22,Y22);
		InfoOut += "X1:";
		InfoOut += num2str(X1);
		InfoOut += "  ";
		InfoOut += num2str(Y1);
		InfoOut += "  X2:";
		InfoOut += num2str(X2);
		InfoOut += "  ";
		InfoOut += num2str(Y2);
		
		InfoOut += "\n";
		InfoOut += "  X12:";
		InfoOut += num2str(X12);
		InfoOut += "  ";
		InfoOut += num2str(Y12);
		InfoOut += "  X22:";
		InfoOut += num2str(X22);
		InfoOut += "  ";
		InfoOut += num2str(Y22);
		InfoOut += "  ";
	}
	*/
	/*
	//display the moving average
	for(int i = 0; i < bufferNumberOfSecondLength; i++){
		if(averageLowPassedSound[i]!=0){
			InfoOut += num2str(i);
			InfoOut += "; ";
			InfoOut += num2str(averageLowPassedSound[i]);
			InfoOut += "\n";
		}
	}
	*/
	/*
	InfoOut = "Rhythmic Testing: \n";
	for(int i = 0; i < bufferNumberOfSecondLength; i++){
		if(currentBeatMagnitudeArray[i]!=0){
			InfoOut += num2str(i);
			InfoOut += "; ";
			InfoOut += num2str(currentBeatMagnitudeArray[i]);
			InfoOut += "\n";
		}
	}
	*/

	////REVISION NEEDED!!!!!!!!!!!!!!!!!!!!!**************************
	//InfoOut = "Rhythmic Testing: \n";
	//int currentLevels = 0;
	//for(int i = 0; i < theRhythmicExtract->GetInitializedLevel(); i++){
	//	if(theRhythmicExtract->currentBeatMagnitudeArray[i]!=0){
	//		InfoOut += num2str(i);
	//		InfoOut += "; ";
	//		InfoOut += num2str(theRhythmicExtract->currentBeatMagnitudeArray[i]);
	//		InfoOut += "\n";
	//		currentLevels++;
	//	}
	//}
	//InfoOut = num2str(currentLevels) + "\n" + InfoOut;
	
	//InfoOut = "Rhythmic log loudness: ";
	//InfoOut += num2str(theCreationCore->GetRhythmicEvenCore()->GetCurrentLogLoudness());
	//InfoOut += "\nBeat Level: ";
	//InfoOut += num2str(theCreationCore->GetRhythmicEvenCore()->GetCurrentLevel());
	//InfoOut += "\nAverage Loudness: ";
	//InfoOut += num2str(theCreationCore->GetRhythmicEvenCore()->GetAverageSongLogLoudness());
	
	//InfoOut = num2str(theSpeedControl->GetFPS());
	//InfoOut = num2str(debugCnt);
	//InfoOut += "\nVQsize: ";
	//InfoOut += num2str(theReader->verticalanimationTextureDQue.size());

	//InfoOut += "\nLineSeg: ";
	//InfoOut += num2str(theBackground->numLineSeg);
}