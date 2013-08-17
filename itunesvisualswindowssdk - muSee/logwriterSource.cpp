#include "logwriter.h"

namespace museelogwriter {
	ofstream CreateMuSeeLogFile()//create an empty log file (this also opens the file); return full pass
	{
		string logfullPath = ExePath();
		logfullPath +=  "\\Plug-Ins\\muSee\\Data\\";
		logfullPath += "museeLog.txt";
		FILE * tempFile;
		tempFile = fopen (logfullPath.c_str(),"w");
		fclose (tempFile);

		ofstream logfile;
		logfile.open(logfullPath);
		return logfile;
	}


}