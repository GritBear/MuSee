#include "Global.h"

//=========================================================================================
//Outside DataReader Class Helper function
//Author: Li(Lee) Xiong. All rights reserved.
//=========================================================================================
string num2str(float number)
{
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

//Obtain the current execution path
string ExePath() {
    char buffer[MAX_PATH];
    GetModuleFileName( NULL, buffer, MAX_PATH );
    string::size_type pos = string( buffer ).find_last_of( "\\/" );
    return string( buffer ).substr( 0, pos);
}


double str2double(string valuestr){
	double valuedouble = atof(valuestr.c_str());  //c_str is needed to convert string to const char
	return valuedouble;
}
