#pragma once
#include "Global.h"


namespace museelogwriter {
	ofstream  CreateMuSeeLogFile();//create an empty log file (this also opens the file); return full pass
	//{
	////	string logfullPath = ExePath();
	////	logfullPath +=  "\\Plug-Ins\\muSee\\Data\\";
	////	logfullPath += "museeLog.txt";
	////	FILE * tempFile;
	////	tempFile = fopen (logfullPath.c_str(),"w");
	////	fclose (tempFile);

	//	
	//	//logfile.open(logfullPath);

	//}

	//void CloseFile(ofstream  logfile)
	//{
	//	if(logfile.is_open())
	//		logfile.close();
	//};//be called to close the string

	//void CloseFile(ofstream  logfile, string allData)
	//{
	//	logfile = (ofstream)allData;
	//	if(logfile.is_open())
	//		logfile.close();
	//};//be called to close the string

	//void AppendData(ofstream logfile, string data){
	//	logfile<<data;
	//};
};