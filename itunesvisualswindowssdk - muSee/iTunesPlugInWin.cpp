/*
 File:       iTunesPlugInWin.cpp

 Abstract:   Visual plug-in for iTunes on Windows. Modified from Apple's sample code by Lee Xiong

*/

//-------------------------------------------------------------------------------------------------
//	includes
//-------------------------------------------------------------------------------------------------
#include "iTunesPlugIn.h"
#include "Time.h"
#include <GdiPlus.h>
#include <string>

//-------------------------------------------------------------------------------------------------
//	constants, etc.
//-------------------------------------------------------------------------------------------------

#define kTVisualPluginName              L"¦ÌSee"

static	WNDPROC			 giTunesWndProc			= NULL;
static ArtStoryCore* gArtStoryCore		= NULL;

//-------------------------------------------------------------------------------------------------
//	WndProc
//-------------------------------------------------------------------------------------------------
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CHAR:			// handle key pressed messages
			if (wParam == 'I' || wParam == 'i')
			{			}
			break;

		default:
			break;
	}

	// call iTunes with the event.  Always send space bar, ESC key, TAB key and the arrow keys: iTunes reserves those keys
	return CallWindowProc( giTunesWndProc, hWnd, message, wParam, lParam );
}


//-------------------------------------------------------------------------------------------------
//	UpdateArtwork
//-------------------------------------------------------------------------------------------------
//
void UpdateArtwork( ArtStoryCore * aStory, VISUAL_PLATFORM_DATA coverArt, UInt32 coverArtSize, UInt32 coverArtFormat )
{
	aStory->currentArtwork = NULL;

	HGLOBAL	globalMemory  = GlobalAlloc( GMEM_MOVEABLE, coverArtSize );
	if ( globalMemory ) 
	{
		void* pBuffer = ::GlobalLock(globalMemory);
		if ( pBuffer )
		{
			memcpy( pBuffer, coverArt, coverArtSize );

			IStream* stream = NULL;
			CreateStreamOnHGlobal( globalMemory, TRUE, &stream );

			if ( stream )
			{
				Gdiplus::Bitmap* gdiPlusBitmap = Gdiplus::Bitmap::FromStream( stream );
				stream->Release();

				if ( gdiPlusBitmap && gdiPlusBitmap->GetLastStatus() == Gdiplus::Ok)
				{
					aStory->currentArtwork = gdiPlusBitmap;
				}
			}
		}
	}
}




//-------------------------------------------------------------------------------------------------
//	GetVisualOptions
//-------------------------------------------------------------------------------------------------
//
OptionBits GetVisualOptions( void )
{
	return 0 ;// + kVisualWantsIdleMessages;// + kPluginWantsDisplayNotification;
}

//-------------------------------------------------------------------------------------------------
//	ConfigureVisual
//-------------------------------------------------------------------------------------------------
//
OSStatus ConfigureVisual( ArtStoryCore * aStory )
{
	aStory->currentArtwork = NULL;
	return noErr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//	GetVisualName
//-------------------------------------------------------------------------------------------------
//
void GetVisualName( ITUniStr255 name )
{	
	name[0] = (UniChar)wcslen( kTVisualPluginName );
	wcscpy_s( (wchar_t *)&name[1], 255, kTVisualPluginName );
}


//-------------------------------------------------------------------------------------------------
//	iTunesPluginMain start of .DLL
//-------------------------------------------------------------------------------------------------
//
extern "C" __declspec(dllexport) OSStatus iTunesPluginMain( OSType message, PluginMessageInfo * messageInfo, void * refCon )
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
}

