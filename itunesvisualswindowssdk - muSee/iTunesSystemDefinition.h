#pragma once

//=========================================================================================================
// iTunes System Definitions
//=========================================================================================================
#if TARGET_OS_WIN32
#include <Gdiplus.h>
#endif // TARGET_OS_WIN32

//-------------------------------------------------------------------------------------------------
//	build flags
//-------------------------------------------------------------------------------------------------
#define USE_SUBVIEW						(TARGET_OS_MAC && 1)		// use a custom NSView subview on the Mac

//-------------------------------------------------------------------------------------------------
//	typedefs, structs, enums, etc.
//-------------------------------------------------------------------------------------------------
#define	kTVisualPluginCreator			'hook'

#define	kTVisualPluginMajorVersion		1
#define	kTVisualPluginMinorVersion		0
#define	kTVisualPluginReleaseStage		developStage
#define	kTVisualPluginNonFinalRelease	0

#if TARGET_OS_MAC
#import <Cocoa/Cocoa.h>

// "namespace" our ObjC classname to avoid load conflicts between multiple visualizer plugins
#define VisualView		ComAppleExample_VisualView
#define GLVisualView	ComAppleExample_GLVisualView

@class VisualView;
@class GLVisualView;

#endif

#define kInfoTimeOutInSeconds		10							// draw info/artwork for N seconds when it changes or playback starts
#define MinScreenWidth				512
#define MinScreenHeight				512
;