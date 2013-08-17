/*
void ArtStoryCore::TestMelodyExtractionRender(){
	if ( this && destView != NULL )
	{
		//Initializing variables
		GetClientRect(destView, &clientRect);

		HDC Memhdc, hdc;
		HBITMAP Membitmap;

		hdc = ::GetDC( destView );

		Memhdc = CreateCompatibleDC(hdc);
		Membitmap = CreateCompatibleBitmap(hdc, screen_w, screen_h);
		SelectObject(Memhdc, Membitmap);

		//drawing code goes in here on Memhdc
		//--------------------------------------------------------------------------------------------------------

		// fill the whole view with black to start
		::FillRect( Memhdc, &clientRect, (HBRUSH) GetStockObject( WHITE_BRUSH ) );

		COLORREF color = RGB(255, 50, 20);


		int melodySize = stepMelodyTestOut.size();
		
		if(melodySize>0){
			float columnWidth = screen_w/(numberOfSecondsStored * ExtractRate);
			float squareHeight;
			
			list<vector<float>>::iterator stepMelodyIterator;
			int colInter = 0;

			
			// pick a random location to draw our little square
			for(stepMelodyIterator = stepMelodyTestOut.begin(); stepMelodyIterator != stepMelodyTestOut.end(); ++stepMelodyIterator){
				int entriesInCol = (*stepMelodyIterator).size();
				int startingInd = -minimumTone + bassMelodyDevideTone;
				squareHeight = screen_h/(entriesInCol - startingInd + 1);

				float maxP = (*stepMelodyIterator)[0];
				for(int j = 1; j < entriesInCol; j ++){
					if(maxP < (*stepMelodyIterator)[j])
						maxP = (*stepMelodyIterator)[j];
				}
				if(maxP == 0)
					maxP = 1;
				
				vector<RECT>drawRect(entriesInCol);

				for(int j = startingInd; j < entriesInCol; j ++){
					COLORREF colorVect2;
					colorVect2 = RGB((*stepMelodyIterator)[j]/maxP * 255, 255 -(*stepMelodyIterator)[j]/maxP * 255 , 255 -(*stepMelodyIterator)[j]/maxP * 255);

					drawRect[j].left	= colInter * columnWidth;
					drawRect[j].top	= drawRect[j].bottom - squareHeight;
					drawRect[j].right	= drawRect[j].left + columnWidth;
					drawRect[j].bottom = screen_h	- (j-startingInd)*squareHeight;

					HBRUSH brush2  = CreateSolidBrush( colorVect2  );
					FillRect( Memhdc, &drawRect[j], brush2 );	
					DeleteObject(brush2);
					
				}

				colInter ++;
			}
						
		}
		//---------------------------------------------------------------------------------------------------------
		// Draw a outstring message
		if ( InfoOut.size() > 0 )
		{
			int InfoX = 30;
			int InfoY = 30;
			RECT outstringRect =  {InfoX, InfoY, InfoX + 100, InfoY + 80};

			COLORREF OutTextColor = SetTextColor (Memhdc, RGB(100,150,255));
			::DrawText( Memhdc, InfoOut.c_str() , InfoOut.size(), &outstringRect ,DT_LEFT);

			SetTextColor( Memhdc, OutTextColor );
		}
		
		//---------------------------------------------------------------------------------------------------------
		//bitblt back to main display
		BitBlt(hdc, 0, 0, screen_w, screen_h, Memhdc, 0, 0, SRCCOPY);
		ReleaseDC(destView, Memhdc );
		ReleaseDC(destView, hdc );
		DeleteObject(Membitmap);
		DeleteDC    (Memhdc);
		DeleteDC    (hdc);
	}

}*/