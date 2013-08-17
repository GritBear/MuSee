#include "ArtStoryCore.h"
//=================================================================
//Windows Specific OpenGL methods
//Author: Li(Lee) Xiong. All rights reserved.
//=================================================================
void ArtStoryCore::InfoOutToScreen(){
	if ( InfoOut.size() > 0 )
	{
		int InfoX = 30;
		int InfoY = 30;
		RECT outstringRect =  {InfoX, InfoY, InfoX + 400, InfoY + 80};

		COLORREF OutTextColor = SetTextColor (hDC, RGB(100,150,255));
		COLORREF OutBkColor = SetBkColor (hDC, RGB(255, 255, 255));
		::DrawText( hDC, InfoOut.c_str() , InfoOut.size(), &outstringRect ,DT_LEFT);

		SetTextColor( hDC, OutTextColor );
		SetBkColor( hDC, OutBkColor );
	}
}

void ArtStoryCore::CreatContext(){
	if ( destView != NULL ){
		if(!textureLoaded){
			//set flags for OpenGL
			// Describes the pixel format of the drawing surface
			memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
			pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
			pfd.nVersion = 1; // Version Number
			pfd.dwFlags = PFD_DRAW_TO_WINDOW |  // Draws to a window
					PFD_SUPPORT_OPENGL |  // The format must support OpenGL
					PFD_DOUBLEBUFFER;     // Support for float buffering
			pfd.iPixelType = PFD_TYPE_RGBA;     // Uses an RGBA pixel format
			pfd.cColorBits = 24;                // 24 bits colors
			pfd.cAlphaBits = 8;					// 8 bits alpha channel
			pfd.cDepthBits = 16;				// 16 bits depth
			pfd.iLayerType = PFD_MAIN_PLANE;
		}

		hDC = GetDC(destView);
		int PixelFormat;
		// Do Windows find a matching pixel format ?
		PixelFormat = ChoosePixelFormat(hDC,&pfd);

		// Set the new pixel format
		SetPixelFormat(hDC,PixelFormat,&pfd);

		// Create the OpenGL rendering context
		hRC = wglCreateContext(hDC);

		// Activate the rendering context
		wglMakeCurrent(hDC,hRC);
	}
};

void ArtStoryCore::DeactiveGL()
{
    if ( hRC )
    {
        wglMakeCurrent( NULL, NULL );
        wglDeleteContext( hRC );
    }
    if ( destView && hDC )
    {
        ReleaseDC( destView, hDC );
    }
	if(hDC != NULL)
		DeleteDC(hDC);

    destView = NULL;
    hDC = NULL;
    hRC = NULL;
	
	theReader->RemoveAllTextures();
	textureLoaded = false;
}
