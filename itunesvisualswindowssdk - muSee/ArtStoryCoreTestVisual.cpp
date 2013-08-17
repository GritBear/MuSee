#include "ArtStoryCore.h"

//------------------------------------------------------
//	DrawVisual
//------------------------------------------------------
void ArtStoryCore::TestMelodyExtractionRender(){
	if (destView != NULL )
	{
		ClearBackground();
		
		//OpenGL drawing code
		//--------------------------------------------------------------------------------------------------------
		
		//draw moving red squares at extracted melody
		int melodySize = stepMelodyTestOut.size();
		if(melodySize>0){
			//obtain the dimension of each square
			int columnWidth = screen_w/(numberOfSecondsStored * ExtractRate);
			int squareHeight;
			
			list<vector<float>>::iterator stepMelodyIterator;
			int colInter = 0;
			
			GLboolean textureEnabled;
			glGetBooleanv(GL_TEXTURE_2D, &textureEnabled);

			if(textureEnabled)
				glDisable(GL_TEXTURE_2D);

			//iterate through the stored step melodies
			for(stepMelodyIterator = stepMelodyTestOut.begin(); stepMelodyIterator != stepMelodyTestOut.end(); ++stepMelodyIterator){
				int entriesInCol = (*stepMelodyIterator).size();
				int startingInd = -minimumTone + bassMelodyDevideTone;
				squareHeight = screen_h/(entriesInCol - startingInd + 1);

				GLint left, right, top, bottom;
				for(int j = startingInd; j < entriesInCol; j ++){
					//only draw the extracted tones
					if((*stepMelodyIterator)[j]>0){

						left	= colInter * columnWidth;
						bottom  = screen_h	- (j-startingInd)*squareHeight;

						top	    = bottom - squareHeight;
						right	= left + columnWidth;

						glPushMatrix();
						glBegin(GL_QUADS);
						glColor4f(1.0,1.0,1.0,1.0);	//empty buffur color

						glColor3f(1.0,0.0,0.0);	
						glVertex3i(left,top,0);
						glVertex3i(right,top,0);
						glVertex3i(right,bottom,0);
						glVertex3i(left,bottom,0);
						
						glEnd();
						glPopMatrix();
					}
				}

				colInter ++;
			}
			
		if(textureEnabled)
			glEnable(GL_TEXTURE_2D);
		}

		SwapBuffers( hDC );
		//---------------------------------------------------------------------------------------------------------
		// Draw an out string message
		InfoOutToScreen();
		
		//---------------------------------------------------------------------------------------------------------
	}
}
