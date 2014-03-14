package musee.mapgen;

import musee.mapgen.utils.TileUtils;

public class LandScape {
	SongInfo song;
	IdealPathCalc idealPath;

	public LandScape(SongInfo aSong, IdealPathCalc idealPath){
		song = aSong;
		this.idealPath = idealPath;
	}

	public void processBackground(int[][] background, int[][]ObjectArray, int width, int height){
		double prevYTarget = 0;
		int prevType = 0;

		for(int x = 0; x < width; x++){
			int songIndex = TileUtils.tileIndexToSong(x, Consts.tileToSampleRatio);
			double toneDeviation = idealPath.getIdealPathDouble().get(songIndex) - song.minNonZeroTone;
			double yTarget = toneDeviation * Consts.tilesPerTone;

			yTarget = Math.min(yTarget, height -1);

			if(0<=yTarget && yTarget<= height -1){
				int yIndex = (int) Math.ceil(yTarget);	
				for(int tempY = yIndex; tempY > 0; tempY--){
					background[x][tempY] = 17;
				}

				//smoothing processing here
				if(x == 0){
					prevType = Consts.normalSquare;
					prevYTarget = yIndex;
				}else{
					double YTargetDiff = yIndex - prevYTarget;
					YTargetDiff = TileLookUp.singleton.findClosestValueIndex(YTargetDiff);

					if(prevType == Consts.normalSquare){
						//choose the type here
						if(YTargetDiff == 0){
							//do nothing
						}else if(YTargetDiff == -2){
							//some error checking for unknonw reason where glitches happen
							int checkValue = yIndex + 3;
							while(checkValue < height - 1 && background[x-1][checkValue] == 17){
								checkValue ++;
							}
							int numberShift = checkValue - 3 - yIndex;

							for(int i = numberShift + yIndex; i >= yIndex - numberShift; i--){
								if(i >= 0)
									background[x][i] = 17;
							}

							background[x][yIndex + 2 + numberShift] = 44;
							background[x][yIndex + 1 + numberShift] = 60;

						}else if(YTargetDiff == -1){
							background[x][yIndex + 1] = 37;
						}else if(YTargetDiff == 1){
							background[x][yIndex] = 36;		
						}else if(YTargetDiff == 2){
							//some error checking for unknonw reason where glitches happen
							int checkValue = yIndex - 2;
							while(checkValue > 0 && background[x-1][checkValue] == Consts.nonIndex){
								checkValue --;
							}
							int numberShift = checkValue - (yIndex - 2);

							for(int i = numberShift + yIndex; i <= yIndex - numberShift; i++){
								if(i >= 0 && i < height)
									background[x][i] = Consts.nonIndex;
							}

							background[x][yIndex + numberShift] = 43;
							background[x][yIndex-1 + numberShift] = 59;
						}else if(YTargetDiff == -1.5){
							background[x][yIndex + 1] = 44;
							background[x][yIndex] = 53;
							prevType = Consts.normalHalfSquare;
						}else if(YTargetDiff == -0.5){
							background[x][yIndex + 1] = 57;
							prevType = Consts.normalHalfSquare;
						}else if(YTargetDiff == 0.5){
							background[x][yIndex] = 55;
							prevType = Consts.normalHalfSquare;
						}else if(YTargetDiff == 1.5){
							background[x][yIndex] = 51;
							background[x][yIndex-1] = 59;
							prevType = Consts.normalHalfSquare;
						}
					}else{
						//the tile must end in a 0.5 Y location
						if(YTargetDiff == 0){
							background[x][yIndex] = 48;
//						}else if(YTargetDiff == -2){
//							//some error checking for unknonw reason where glitches happen
//							int checkValue = yIndex + 3;
//							while(checkValue < height - 1 && background[x-1][checkValue] == 17){
//								checkValue ++;
//							}
//							int numberShift = checkValue - 3 - yIndex;
//
//							for(int i = numberShift + yIndex; i >= yIndex - numberShift; i--){
//								if(i >= 0)
//									background[x][i] = 17;
//							}
//
//							background[x][yIndex + 2 + numberShift] = 54;
//							background[x][yIndex + 1 + numberShift] = 40;
//							background[x][yIndex + numberShift] = 53;
						}else if(YTargetDiff == -1){
							background[x][yIndex + 1] = 54;
							background[x][yIndex] = 53;
						}else if(YTargetDiff == 1){
							background[x][yIndex - 1] = 52;	
							background[x][yIndex] = 51;	
//						}else if(YTargetDiff == 2){
//							//some error checking for unknonw reason where glitches happen
//							int checkValue = yIndex - 2;
//							while(checkValue > 0 && background[x-1][checkValue] == Consts.nonIndex){
//								checkValue --;
//							}
//							int numberShift = checkValue - (yIndex - 2);
//
//							for(int i = numberShift + yIndex; i <= yIndex - numberShift; i++){
//								if(i >= 0 && i < height)
//									background[x][i] = Consts.nonIndex;
//							}
//
//							background[x][yIndex + numberShift] = 51;
//							background[x][yIndex-1 + numberShift] = 25;
//							background[x][yIndex-2 + numberShift] = 52;
						}else if(YTargetDiff <= -1.5){
							background[x][yIndex + 1] = 54;
							background[x][yIndex] = 60;
//							YTargetDiff = -1.5;
							prevType = Consts.normalSquare;
						}else if(YTargetDiff == -0.5){
							background[x][yIndex + 1] = 54;
							prevType = Consts.normalSquare;
						}else if(YTargetDiff == 0.5){
							background[x][yIndex] = 56;
							prevType = Consts.normalSquare;
						}else if(YTargetDiff >= 1.5){
							background[x][yIndex] = 43;
							background[x][yIndex-1] = 52;
//							YTargetDiff = 1.5;
							prevType = Consts.normalSquare;
						}
					}
					prevYTarget += YTargetDiff;
				}
			}
		}
		invertVerticalIndexs(background,  width, height);
	}

	private void invertVerticalIndexs(int[][] array, int width, int height){
		int[] bufferArray = new int[height];
		for(int col = 0; col < width; col++){
			for(int row = 0; row < height; row++){
				bufferArray[row] = array[col][row];
			}
			for(int row = 0; row < height; row++){
				array[col][row] = bufferArray[height - 1 - row];
			}
		}
	}

	
	
	

}
