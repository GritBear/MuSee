#include "stdafx.h"
#include "ScriptReader.h"

//functions copied from the Global Header in the main solution
double str2double(string valuestr){
	double valuedouble = atof(valuestr.c_str());  //c_str is needed to convert string to const char
	return valuedouble;
}

namespace ReaderCore{
DataReaderCore::DataReaderCore(){

};

void DataReaderCore::Destroy(){

}

void DataReaderCore::init(string thisDirectoryBuffer){
	//fileStringBuffer.clear();
	thisDirectoryBuffer = thisDirectoryBuffer;
	//memset(&LoadedTextureInfoPtr,0,sizeof(LoadedTextCollection));
}

void DataReaderCore::ReadingDrive(){
	StringReadDrive(); //first read script file
	TextLoaderDrive(); //then read the texture
};

void DataReaderCore::StringReadDrive(){
	ReadScript();

}

void DataReaderCore::ClearStringRead(){
	TextureMap.clear();
}

//this reads "scriptName" file
void DataReaderCore::ReadScript(){
	map<string,string> tempMap;
	ifstream scriptfile;
	string fullPass = thisDirectoryBuffer + intermediateFilePath + scriptName;
	TextureImage * nextText = NULL;

	scriptfile.open (fullPass);
	do{
		nextText = ReadNextText(&scriptfile);
		if(nextText!= NULL){
			TextureMap[nextText->texName] = nextText;
		}
	}while(nextText != NULL);
	
	//now capable of read and write
	if(scriptfile.is_open())
		scriptfile.close();
}

TextureImage * DataReaderCore::ReadNextText(ifstream * scriptfile){
	map<string,string> tempMap;
	string divider = nextStringDivider;
	//create the new texture struct
	TextureImage * newTexture = NULL;

	readstatus = ReadNextChunck(scriptfile, &tempMap, &divider);
	
	if(readstatus == successRead){
		newTexture = new TextureImage();
		newTexture->texName = tempMap["texName"];

		//fill in texParameter struct inside newTexture
		TexParameter* tempTexptr = &(newTexture->texParameter);
		tempTexptr->animationColumns = str2double(tempMap["animationColumns"]);
		tempTexptr->collidable = str2double(tempMap["collidable"]);
		tempTexptr->entranceEffectID = (EntranceEffect)(int)str2double(tempMap["entranceEffectID"]);
		tempTexptr->initialStationary = str2double(tempMap["initialStationary"]);
		tempTexptr->maxEntranceTime = str2double(tempMap["maxEntranceTime"]);
		tempTexptr->maxExitTime = str2double(tempMap["maxExitTime"]);
		tempTexptr->maxLength = str2double(tempMap["maxLength"]);
		tempTexptr->maxTimeInS = str2double(tempMap["maxTimeInS"]);
		tempTexptr->max_AngleChangeInRad = str2double(tempMap["max_AngleChangeInRad"]);
		tempTexptr->max_frame = str2double(tempMap["max_frame"]);
		tempTexptr->minEntranceTime = str2double(tempMap["minEntranceTime"]);
		tempTexptr->minExitTime = str2double(tempMap["minExitTime"]);
		tempTexptr->minLength = str2double(tempMap["minLength"]);
		tempTexptr->minTimeInS = str2double(tempMap["minTimeInS"]);
		tempTexptr->texShape = (DrawUnitShape)(int)str2double(tempMap["texShape"]);
		tempTexptr->usingTexture = str2double(tempMap["usingTexture"]);
		tempTexptr->xBound = str2double(tempMap["xBound"]);
		tempTexptr->yBound = str2double(tempMap["yBound"]);
	}
	FreeAll(tempMap);
	return newTexture;
}

ReadingStatus DataReaderCore::ReadNextChunck(ifstream * scriptfile, map<string,string> * tempMap, string * NextDivider){
	if((*scriptfile).is_open()){
		string line;
		string keystr;
		string valuestr;
		while ((*scriptfile).good())
		{
			getline ((*scriptfile),line);
			if(line.compare((*NextDivider))){
				return successRead;
			}
			string::size_type  pos = line.find_first_of(kvDiv);
			//seperate two strings
			keystr = line.substr( 0, pos);
			valuestr = line.substr(pos+1,line.length());

			(*tempMap)[keystr] = valuestr;
		}
		return fileFinishedFailure;
	}else{
		return fileNotOpenFailure;
	}
}

};