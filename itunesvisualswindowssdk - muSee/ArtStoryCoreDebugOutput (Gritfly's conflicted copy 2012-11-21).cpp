#include "ArtStoryCore.h"

//this method is called in Render() to display debugging data
void ArtStoryCore::DebugUpdate(){
	/*
	int size = (*MelodyObjList.front()).inScreenTraceX.size();
	InfoOut = NumberToString(size);
	InfoOut += "\n";
	if(size>0)
		InfoOut += NumberToString( (*MelodyObjList.front()).inScreenTraceX[size - 1]);
	
	InfoOut = NumberToString(theBackground->flyingAniObjList.size());
	if(theBackground->flyingAniObjList.size()>0){
		float X1,X2,Y1,Y2, X12,Y12,X22,Y22;
		InfoOut += "\n";
		InfoOut += NumberToString(theBackground->flyingAniObjList.back()->GetDebuggingValue2());
		InfoOut += "  ";
		InfoOut += NumberToString(theBackground->flyingAniObjList.back()->GetDebuggingValue());		
		InfoOut += "\n";

		InfoOut += NumberToString(theBackground->flyingAniObjList.back()->GetID());
		InfoOut += "  AX";		
		InfoOut += NumberToString(theBackground->flyingAniObjList.back()->GetAnchorX());
		InfoOut += "  AY:";
		InfoOut += NumberToString(theBackground->flyingAniObjList.back()->GetAnchorY());
		InfoOut += "  X:";		
		InfoOut += NumberToString(theBackground->flyingAniObjList.back()->GetX());
		InfoOut += "  Y:";
		InfoOut += NumberToString(theBackground->flyingAniObjList.back()->GetY());

		InfoOut += "\n";
		theBackground->flyingAniObjList.back()->GetFourCorners(X1,Y1,X12,Y12,X2,Y2,X22,Y22);
		InfoOut += "X1:";
		InfoOut += NumberToString(X1);
		InfoOut += "  ";
		InfoOut += NumberToString(Y1);
		InfoOut += "  X2:";
		InfoOut += NumberToString(X2);
		InfoOut += "  ";
		InfoOut += NumberToString(Y2);
		

		InfoOut += "\n";
		InfoOut += "  X12:";
		InfoOut += NumberToString(X12);
		InfoOut += "  ";
		InfoOut += NumberToString(Y12);
		InfoOut += "  X22:";
		InfoOut += NumberToString(X22);
		InfoOut += "  ";
		InfoOut += NumberToString(Y22);
		InfoOut += "  ";
	}
	*/
	/*
	//display the moving average
	for(int i = 0; i < bufferNumberOfSecondLength; i++){
		if(averageLowPassedSound[i]!=0){
			InfoOut += NumberToString(i);
			InfoOut += "; ";
			InfoOut += NumberToString(averageLowPassedSound[i]);
			InfoOut += "\n";
		}
	}
	*/

	/*
	InfoOut = "Rhythmic Testing: \n";
	for(int i = 0; i < bufferNumberOfSecondLength; i++){
		if(currentBeatMagnitudeArray[i]!=0){
			InfoOut += NumberToString(i);
			InfoOut += "; ";
			InfoOut += NumberToString(currentBeatMagnitudeArray[i]);
			InfoOut += "\n";
		}
	}


	//REVISION NEEDED!!!!!!!!!!!!!!!!!!!!!**************************
	InfoOut = "Rhythmic Testing: \n";
	int currentLevels = 0;
	for(int i = 0; i < theRhythmicExtract->GetInitializedLevel(); i++){
		if(theRhythmicExtract->currentBeatMagnitudeArray[i]!=0){
			InfoOut += NumberToString(i);
			InfoOut += "; ";
			InfoOut += NumberToString(theRhythmicExtract->currentBeatMagnitudeArray[i]);
			InfoOut += "\n";
			currentLevels++;
		}
	}
	InfoOut = NumberToString(currentLevels) + "\n" + InfoOut;
	*/

	//infoout = "rhythmic log loudness: ";
	//infoout += numbertostring(thecreationcore->getrhythmicevencore()->getcurrentlogloudness());
	//infoout += "\nbeat level: ";
	//infoout += numbertostring(thecreationcore->getrhythmicevencore()->getcurrentlevel());
	//infoout += "\naverage loudness: ";
	//infoout += numbertostring(thecreationcore->getrhythmicevencore()->getaveragesonglogloudness());


}