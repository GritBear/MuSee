#pragma once
#include "MusicExtraction.h"
#include "RhythmicExtract.h"


namespace musee
{
	/*__declspec(dllexport) museeAPI * initMuseeAPI(MelodyExtractionPram * param){return (new museeAPI(param));}

	__declspec(dllexport) OSStatus iTunesPluginMain( OSType message, PluginMessageInfo * messageInfo, void * refCon )
{
	OSStatus		status;
	
	(void) refCon;
	
	switch ( message )
	{
		case kPluginInitMessage:
			status = RegisterVisualPlugin( messageInfo );

			break;
			
		case kPluginCleanupMessage:
			status = noErr;
			break;
			
		default:
			status = unimpErr;
			break;
	}
	
	return status;
}*/

	//this is a wrapper class (an interface/dispatcher between outside code and music processing units)
    class museeAPI
    {
	private:
		MelodyExtraction * melody;
		RhythmicExtract * rhythmic;

    public:
		museeAPI(MelodyExtractionPram * param){init(param);}

		void init(MelodyExtractionPram * param);

		void update(RenderVisualData * RenderData);


    };
}