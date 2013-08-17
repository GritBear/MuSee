/*
			// pick a random location to draw our little square
			for(stepMelodyIterator = stepMelodyTestOut.begin(); stepMelodyIterator != stepMelodyTestOut.end(); ++stepMelodyIterator){
				int entriesInCol = (*stepMelodyIterator).size();
				int StartingToneInd = (-minimumTone)/toneResolution -1 ;
				int validEntry = entriesInCol - StartingToneInd;
				squareHeight = screen_h/validEntry;

				float maxP = (*stepMelodyIterator)[0];
				for(int j = 1; j < entriesInCol; j ++){
					if(maxP < (*stepMelodyIterator)[j])
						maxP = (*stepMelodyIterator)[j];
				}
				if(maxP == 0)
					maxP = 1;
				InfoOut = num2str(entriesInCol - StartingToneInd);

				vector<RECT>drawRect(entriesInCol);
				
				for(int j = StartingToneInd; j < entriesInCol; j ++){
					COLORREF colorVect2 = RGB((*stepMelodyIterator)[j]/maxP * 255, 255 -(*stepMelodyIterator)[j]/maxP * 255 , 255 -(*stepMelodyIterator)[j]/maxP * 255);
										
					drawRect[j].left	= colInter * columnWidth;
					drawRect[j].top	= drawRect[j].bottom - squareHeight;
					drawRect[j].right	= drawRect[j].left + columnWidth;
					drawRect[j].bottom = screen_h	- (j-StartingToneInd)*squareHeight;

					HBRUSH brush2  = CreateSolidBrush( colorVect2  );
					FillRect( Memhdc, &drawRect[j], brush2 );	
					DeleteObject(brush2);
				}

				colInter ++;
			}
			*/
/*


//this is melodylineslope adjustment implementation, not good
void MelodyObj::EffectiveToneRegistration(int tone, int rank){
	/*Available info: 	list<int> melodyLineToneList;
						list<float> effectiveMelodyLineToneList; 
	
	int size_effectiveMelody = effectiveMelodyLineToneList.size();
	if(size_effectiveMelody < minimumBuffer){
		effectiveMelodyLineToneList.push_back((float)tone);
	}else if(size_effectiveMelody == minimumBuffer){
		// an arbitrary slope initialization
		effectiveMelodyLineToneList.push_back((float)tone);
		float aSum = 0;
		list<float> ::iterator anIter;
		for(anIter = effectiveMelodyLineToneList.begin(); anIter != effectiveMelodyLineToneList.end(); ++anIter)
			aSum += (*anIter);

		melodySlope =  (tone - aSum/minimumBuffer)/(curToneTimeInMS - previousToneTimeInMS);
	}else{
		/*The Effective Melody Rule:
			upward preference
			high score preference
			function like a biased low pass filter

			top line/non top line discrimination?
			Fine tuning needed here

			use a simple averaging climbing scheme tomorrow
		
		int sizeScore = curScore.size();
		float theScore = curScore[sizeScore - rank - 1];
		float effectiveFactor;
		float socreEffect;
		
		if(tone > effectiveMelodyLineToneList.back()){
			effectiveFactor = upwardEffect;
		}else{
			effectiveFactor = downwardEffect;}
		if(theScore <= 0)
			socreEffect = negativeScoreEffect;
		else
			socreEffect = theScore * scoreEffect;


		float targetTone = effectiveMelodyLineToneList.back() + melodySlope * (curToneTimeInMS - previousToneTimeInMS);
		float toneDeviation = tone - targetTone;

		//update melody slope and calculate new target
		melodySlope += toneDeviation * deviationEffect * effectiveFactor * socreEffect;
		float newTargetTone = effectiveMelodyLineToneList.back() + melodySlope * (curToneTimeInMS - previousToneTimeInMS);

		//avoid melody line overshot
		if((tone >= effectiveMelodyLineToneList.back() && newTargetTone >= tone) ||(tone < effectiveMelodyLineToneList.back() && newTargetTone < tone )){
			effectiveMelodyLineToneList.push_back((float)tone);
		}else if((tone >= effectiveMelodyLineToneList.back() && newTargetTone < tone)||(tone < effectiveMelodyLineToneList.back() && newTargetTone > tone )){
			effectiveMelodyLineToneList.push_back((float)tone);
		}else{
			effectiveMelodyLineToneList.push_back(newTargetTone);
		}
	}
}


*/

//------------------------------------------------
//Evaluate the likelyhood score
// tone_rank = 0 means the tone is the highest, 1 means second highest and so on
//------------------------------------------------
/*float MelodyObj::EvaluateToneAcceptanceScore(int tone, int tone_rank){
	float score = startingOffset;
	//first keep the internal vector up-to-date with the tone list
	UpdateMelodyVector();
	 Evaluation based on:
		1: if it the The Highest Tone
		2: how close the tone is to its previous one
		3: how good it fits with the trend of past N tone
	
	// 1st step
	if(isTheHighestLine && tone_rank == 0)
		score += isHighestOffset;
	else if(isTheHighestLine && tone_rank!=0)
		score += negativeHighestNotPickedScale * tone_rank;

	// 2ed step
	score += ToneClosenessEvaluation(tone);

	// 3rd step
	score += TrendFittingEvaluation(tone, regressionOrder);

	return score;
}*/

/*void ArtStoryCore::Render(){
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

		// pick a random location to draw our little square
		float randomX	= 0.5;    //(rand() / 32767.0);		// [0, 1]
		float randomY	= 0.5;    //(rand() / 32767.0);		// [0, 1]
		LONG whereX	= (LONG) (randomX * ( screen_w ));
		LONG whereY	= (LONG) (randomY * ( screen_h ));

		COLORREF color;
		if ( playing )
		{
			// if playing, draw a square whose color is dictated by the current max levels
			int		red		= maxLevel[1];
			int		green	= maxLevel[1];
			int		blue	= maxLevel[0];

			color = RGB(red, green, blue);
		}
		else
		{
			color = RGB(0, 0, 255);		// a blue square
		}

		RECT drawRect;
		drawRect.left	= whereX - 50;
		drawRect.top	= whereY - 50;
		drawRect.right	= drawRect.left + 100;
		drawRect.bottom = drawRect.top	+ 100;
				
		HBRUSH brush = CreateSolidBrush( color );
		FillRect( Memhdc, &drawRect, brush );
		DeleteObject( brush );

		// Draw the art work
		// should we draw the info/artwork in the bottom-left corner?
		time_t		theTime = time( NULL );
		if ( theTime < drawInfoTimeOut )
		{
			whereX = 30;
			whereY = clientRect.bottom - 30;

			// if we have a song title, draw it (prefer the stream title over the regular name if we have it)
			wchar_t* theString;
			int stringSize(0);

			if ( streamInfo.streamTitle[0] != 0 )
			{
				theString = (wchar_t*) streamInfo.streamTitle[1];
				stringSize = streamInfo.streamTitle[0];
			}
			else if ( trackInfo.name[0] != 0 )
			{
				theString	= (wchar_t*) &trackInfo.name[1];
				stringSize	= trackInfo.name[0];
			}

			// Draw the string
			if ( theString != NULL )
			{
				RECT stringRect = {whereX, whereY, whereX + 400, whereY + 20};

				COLORREF oldTextColor = SetTextColor (Memhdc, RGB(100, 100, 100));
				COLORREF oldBkColor = SetBkColor (Memhdc, RGB(255, 255, 255));
				::TextOutW( Memhdc, whereX, whereY, theString, stringSize );

				SetTextColor( Memhdc, oldTextColor );
				SetBkColor( Memhdc, oldBkColor );
			}

			if ( currentArtwork )
			{
				Gdiplus::Graphics graphics(Memhdc);

				int imageSizeOnScreen_Width		= 120;
				int imageSizeOnScreen_Height	= imageSizeOnScreen_Width * ( currentArtwork->GetHeight() / currentArtwork->GetWidth() );
				
				whereY = whereY - 10 - imageSizeOnScreen_Height;
				
				graphics.DrawImage(currentArtwork, whereX, whereY, imageSizeOnScreen_Width, imageSizeOnScreen_Height);
			}

			
		} 

		//---------------------------------------------------------------------------------------------------------
		// Draw a outstring message
		if ( InfoOut.size() > 0 )
		{
			int InfoX = 30;
			int InfoY = 30;
			RECT outstringRect =  {InfoX, InfoY, InfoX + 400, InfoY + 80};

			COLORREF OutTextColor = SetTextColor (Memhdc, RGB(100,150,255));
			COLORREF OutBkColor = SetBkColor (Memhdc, RGB(255, 255, 255));
			::DrawText( Memhdc, InfoOut.c_str() , InfoOut.size(), &outstringRect ,DT_LEFT);

			SetTextColor( Memhdc, OutTextColor );
			SetBkColor( Memhdc, OutBkColor );
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
};



*/

