//
// File:       iTunesPlugIn.cpp
// Abstract:   Visual plug-in for iTunes.  Cross-platform code. With explicit message handler and major plug-in driver
//			   Modified from Apple's sample code by Lee Xiong
//


//-------------------------------------------------------------------------------------------------
//	includes
//-------------------------------------------------------------------------------------------------

#include "iTunesPlugIn.h"
#include "ArtStoryCore.h"
#include <string.h>

//-------------------------------------------------------------------------------------------------
//	UpdateTrackInfo
//-------------------------------------------------------------------------------------------------
//
void UpdateTrackInfo( ArtStoryCore * aStory, ITTrackInfo * trackInfo, ITStreamInfo * streamInfo )
{
	if ( trackInfo != NULL )
		aStory->trackInfo = *trackInfo;
	else
		memset( &aStory->trackInfo, 0, sizeof(aStory->trackInfo) );

	if ( streamInfo != NULL )
		aStory->streamInfo = *streamInfo;
	else
		memset( &aStory->streamInfo, 0, sizeof(aStory->streamInfo) );
}

//-------------------------------------------------------------------------------------------------
//	RequestArtwork
//-------------------------------------------------------------------------------------------------
//
static void RequestArtwork( ArtStoryCore * aStory )
{
	// only request artwork if this plugin is active
	if ( aStory->destView != NULL )
	{
		OSStatus		status;
		status = PlayerRequestCurrentTrackCoverArt( aStory->appCookie, aStory->appProc );
	}
}


//-------------------------------------------------------------------------------------------------
//	VisualPluginHandler
//-------------------------------------------------------------------------------------------------
//
static OSStatus VisualPluginHandler(OSType message,VisualPluginMessageInfo *messageInfo,void *refCon)
{
	OSStatus			status;
	// Get the Visual Plugin Data Handler
	ArtStoryCore *	aStory;
	aStory = (ArtStoryCore*) refCon;
	
	status = noErr;

	switch ( message )
	{
		/*
			Sent when the visual plugin is registered.  The plugin should do minimal
			memory allocations here.

			Here the aStory is initialized.
		*/		
		case kVisualPluginInitMessage:
		{
			//---------------------------------------------------------------
			// Initialize the ArtStoryCore as an Class Object
			aStory = new ArtStoryCore();
			//---------------------------------------------------------------
			if ( aStory == NULL )
			{
				status = memFullErr;
				break;
			}
			aStory->appCookie	= messageInfo->u.initMessage.appCookie;
			aStory->appProc	= messageInfo->u.initMessage.appProc;

			messageInfo->u.initMessage.refCon = (void *)aStory;
			break;
		}
		/*
			Sent when the visual plugin is unloaded.
		*/		
		case kVisualPluginCleanupMessage:
		{
			if ( aStory != NULL )
				aStory->Destroy();
				delete( aStory );
			break;
		}
		/*
			Sent when the visual plugin is enabled/disabled.  iTunes currently enables all
			loaded visual plugins at launch.  The plugin should not do anything here.
		*/
		case kVisualPluginEnableMessage:
		case kVisualPluginDisableMessage:
		{
			break;
		}
		/*
			Sent if the plugin requests idle messages.  Do this by setting the kVisualWantsIdleMessages
			option in the RegisterVisualMessage.options field.
			
			DO NOT DRAW in this routine.  It is for updating internal state only.
		*/
		case kVisualPluginIdleMessage:
		{
			break;
		}			
		/*
			Sent if the plugin requests the ability for the user to configure it.  Do this by setting
			the kVisualWantsConfigure option in the RegisterVisualMessage.options field.
		*/
		case kVisualPluginConfigureMessage:
		{
			//status = ConfigureVisual( aStory );
			break;
		}
		/*
			Sent when iTunes is going to show the visual plugin.  At this
			point, the plugin should allocate any large buffers it needs.
		*/
		case kVisualPluginActivateMessage:
		{
			// note: do not draw here if you can avoid it, a draw message will be sent as soon as possible
			if ( status == noErr )
				RequestArtwork( aStory );
			
			//-------------------------------------------------
			// Additional Activation Functions
			aStory->ActivateVisualizer(messageInfo->u.activateMessage.view, messageInfo->u.activateMessage.options);
			//-------------------------------------------------

			break;
		}	
		/*
			Sent when this visual is no longer displayed.
		*/
		case kVisualPluginDeactivateMessage:
		{
			
			//-------------------------------------------------
			// Additional Functions
			aStory->DeactivateVisualizer();
			//-------------------------------------------------

			break;
		}
		/*
			Sent when iTunes is moving the destination view to a new parent window (e.g. to/from fullscreen).
		*/
		case kVisualPluginWindowChangedMessage:
		{
			//-------------------------------------------------
			// Additional Functions
			aStory->MoveVisual(messageInfo->u.windowChangedMessage.options);
			//-------------------------------------------------
			break;
		}
		/*
			Sent when iTunes has changed the rectangle of the currently displayed visual.

			Note: for custom NSView subviews, the subview's frame is automatically resized.
		*/
		case kVisualPluginFrameChangedMessage:
		{
			//-------------------------------------------------
			// Additional Functions
			aStory->ResizeVisual();
			//-------------------------------------------------
			break;
		}
		/*
			Sent for the visual plugin to update its internal animation state.
			Plugins are allowed to draw at this time but it is more efficient if they
			wait until the kVisualPluginDrawMessage is sent OR they simply invalidate
			their own subview.  The pulse message can be sent faster than the system
			will allow drawing to support spectral analysis-type plugins but drawing
			will be limited to the system refresh rate.
		*/
		case kVisualPluginPulseMessage:
		{
			//-------------------------------------------------------------
			// Additional Functions
			aStory->PulseUpdate(messageInfo->u.pulseMessage.currentPositionInMS, messageInfo->u.pulseMessage.timeStampID,
						messageInfo->u.pulseMessage.renderData,
						&messageInfo->u.pulseMessage.newPulseRateInHz);
			//-------------------------------------------------------------
			break;
		}
		/*
			It's time for the plugin to draw a new frame.
			
			For plugins using custom subviews, you should ignore this message and just
			draw in your view's draw method.  It will never be called if your subview 
			is set up properly.
		*/
		case kVisualPluginDrawMessage:
		{
			#if !USE_SUBVIEW
			//-------------------------------------------------
			// Additional Functions
			aStory->Render();
			//aStory->TestMelodyExtractionRender();
			//-------------------------------------------------
			#endif
			break;
		}
		/*
			Sent when the player starts.
		*/
		case kVisualPluginPlayMessage:
		{
			aStory->playing = true;
			
			UpdateTrackInfo( aStory, messageInfo->u.playMessage.trackInfo, messageInfo->u.playMessage.streamInfo );
			aStory->GetScreenDimension();
			RequestArtwork( aStory );
			break;
		}
		/*
			Sent when the player changes the current track information.  This
			is used when the information about a track changes.
		*/
		case kVisualPluginChangeTrackMessage:
		{
			UpdateTrackInfo( aStory, messageInfo->u.changeTrackMessage.trackInfo, messageInfo->u.changeTrackMessage.streamInfo );

			RequestArtwork( aStory );
			
			aStory->TrackChanged();
			break;
		}
		/*
			Artwork for the currently playing song is being delivered per a previous request.
			
			Note that NULL for messageInfo->u.coverArtMessage.coverArt means the currently playing song has no artwork.
		*/
		case kVisualPluginCoverArtMessage:
		{
			aStory->GetScreenDimension();
			UpdateArtwork(	aStory,
							messageInfo->u.coverArtMessage.coverArt,
							messageInfo->u.coverArtMessage.coverArtSize,
							messageInfo->u.coverArtMessage.coverArtFormat );
			break;
		}
		/*
			Sent when the player stops or pauses.
		*/
		case kVisualPluginStopMessage:
		{
			aStory->playing = false;

			break;
		}
		/*
			Sent when the player changes the playback position.
		*/
		case kVisualPluginSetPositionMessage:
		{
			aStory->GetScreenDimension();
			break;
		}
		default:
		{
			status = unimpErr;
			break;
		}
	}

	return status;	
}

//-------------------------------------------------------------------------------------------------
//	RegisterVisualPlugin
//-------------------------------------------------------------------------------------------------
//
OSStatus RegisterVisualPlugin( PluginMessageInfo * messageInfo )
{
	PlayerMessageInfo	playerMessageInfo;
	OSStatus			status;
		
	memset( &playerMessageInfo.u.registerVisualPluginMessage, 0, sizeof(playerMessageInfo.u.registerVisualPluginMessage) );

	GetVisualName( playerMessageInfo.u.registerVisualPluginMessage.name );

	SetNumVersion( &playerMessageInfo.u.registerVisualPluginMessage.pluginVersion, kTVisualPluginMajorVersion, kTVisualPluginMinorVersion, kTVisualPluginReleaseStage, kTVisualPluginNonFinalRelease );

	playerMessageInfo.u.registerVisualPluginMessage.options					= GetVisualOptions();
	playerMessageInfo.u.registerVisualPluginMessage.handler					= (VisualPluginProcPtr)VisualPluginHandler;
	playerMessageInfo.u.registerVisualPluginMessage.creator					= kTVisualPluginCreator;
	
	playerMessageInfo.u.registerVisualPluginMessage.pulseRateInHz			= kPlayingPulseRateInHz;	// update my state N times a second
	playerMessageInfo.u.registerVisualPluginMessage.numWaveformChannels		= 2;
	playerMessageInfo.u.registerVisualPluginMessage.numSpectrumChannels		= 2;

	
	playerMessageInfo.u.registerVisualPluginMessage.minWidth				= MinScreenWidth;
	playerMessageInfo.u.registerVisualPluginMessage.minHeight				= MinScreenHeight;
	playerMessageInfo.u.registerVisualPluginMessage.maxWidth				= 0;	// no max width limit
	playerMessageInfo.u.registerVisualPluginMessage.maxHeight				= 0;	// no max height limit

	/* Registering Reference-------------------------------------------------------------------------------------------------------*/
	playerMessageInfo.u.registerVisualPluginMessage.registerRefCon			= 0;
	/*------------------------------------------------------------------------------------------------------*/

	status = PlayerRegisterVisualPlugin( messageInfo->u.initMessage.appCookie, messageInfo->u.initMessage.appProc, &playerMessageInfo );
		
	return status;
}

