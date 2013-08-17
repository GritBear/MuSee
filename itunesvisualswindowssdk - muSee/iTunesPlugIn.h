/*	File:       iTunesPlugIn.h

	Abstract:   Visual plug-in for iTunes
	First Story Telling Art Itune Visual Plug-in:

		Core Header Files for the Plug-in Driver

	Version:    1.0
*/
#pragma once
#ifndef ITUNESPLUGIN_H
#define ITUNESPLUGIN_H
#include "ArtStoryCore.h"

//=====================================================================================================================
//Plugin Default Functions
//=====================================================================================================================
void		GetVisualName( ITUniStr255 name );
OptionBits	GetVisualOptions( void );
OSStatus	RegisterVisualPlugin( PluginMessageInfo * messageInfo );

void		UpdateInfoTimeOut( ArtStoryCore * aStory );
void		UpdateTrackInfo( ArtStoryCore * aStory, ITTrackInfo * trackInfo, ITStreamInfo * streamInfo );
void		UpdateArtwork( ArtStoryCore * aStory, VISUAL_PLATFORM_DATA coverArt, UInt32 coverArtSize, UInt32 coverArtFormat );
void		UpdatePulseRate( ArtStoryCore * aStory, UInt32 * ioPulseRate );

void		PulseVisual( ArtStoryCore * aStory, UInt32 timeStampID, const RenderVisualData * renderData, UInt32 * ioPulseRate );
void		InvalidateVisual( ArtStoryCore * aStory );

OSStatus	ConfigureVisual( ArtStoryCore * aStory );

#endif
