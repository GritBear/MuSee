#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include "TextStructs.h"
#include "stdafx.h"


//Memory free function for containers with swap() method


double str2double(string valuestr);

using namespace ScriptReaderStore;

namespace ReaderCore{

template <typename T>
void FreeAll( T & t ) {
    T tmp;
    t.swap( tmp );
}


class DataReaderCore{
protected:
	//Animation Obj Storage Elements
	FILE *file;
	char *filename;
	int numTextures;

	//storage element
	string fileStringBuffer;
	string thisDirectoryBuffer; //a string buffer
	map<string, TextureImage*> TextureMap;
	ReadingStatus readstatus;
	
	//=========================================================================================
	//String Reader functions
	//load parameters into the function from txt
	//=========================================================================================
public:
	DataReaderCore();
	~DataReaderCore(){Destroy();};
	void Destroy();
	void init(string thisDirectoryBuffer);
	void ReadingDrive();//start reading script and texture info

	//script reading
	void StringReadDrive();//driver for reading the script
	TextureImage * ReadNextText(ifstream * scriptfile);
	ReadingStatus ReadNextChunck(ifstream * scriptfile, map<string,string> * tempMap, string * NextDivider);
	void ClearStringRead();
	void ReadScript();

	//texture reading
	void TextLoaderDrive();


	//getter
	LoadedTextCollection * GetLoadedCollection(){
		return &LoadedTextureInfo;
	}

private:
	LoadedTextCollection LoadedTextureInfo; 
};
};