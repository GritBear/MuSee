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
		
		for(int x = 0; x < width; x++){
			int songIndex = TileUtils.tileIndexToSong(x, Consts.tileToSampleRatio);
			double toneDeviation = idealPath.getIdealPathRaw().get(songIndex) - song.minNonZeroTone;
			double yTarget = toneDeviation * Consts.tilesPerTone;

			yTarget = Math.min(yTarget, height -1);
			
			if(0<=yTarget && yTarget<= height -1){
				int yIndex = (int) Math.round(yTarget);	
				for(int tempY = yIndex; tempY > 0; tempY--){
					background[x][tempY] = 17;
				}
				
				//smoothing processing here
				if(x == 0){
					prevYTarget = yIndex;
				}else{
					double YTargetDiff = yIndex - prevYTarget;
					int tileIndex = -1;
					YTargetDiff = TileLookUp.singleton.findClosestValueIndex(YTargetDiff);
					
					//choose the type here
					if(YTargetDiff == 0){
						//do nothing
					}else if(YTargetDiff == -2){
						background[x][yIndex + 2] = 44;
						background[x][yIndex + 1] = 60;
					}else if(YTargetDiff == -1){
						background[x][yIndex + 1] = 37;
					}else if(YTargetDiff == 1){
						background[x][yIndex] = 36;
					}else if(YTargetDiff == 2){
						background[x][yIndex] = 43;
						background[x][yIndex-1] = 59;
					}
					prevYTarget = yIndex;
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

	public void processBackgroundRaw(int[][] background, int[][]ObjectArray, int width, int height){
		int prevY = 0;
		for(int x = 0; x < width; x++){
			int songIndex = TileUtils.tileIndexToSong(x, Consts.tileToSampleRatio);
			int toneDeviation = idealPath.getIdealPath().get(songIndex) - song.minNonZeroTone;

			int yInverted = toneDeviation * Consts.tilesPerTone;
			int y = height - 1 - yInverted + 2; //2 is an offset
			y = Math.min(y, height -1);
			if(0<=y && y<= height -1){
				if(x == 0){
					prevY = y;
					for(int tempY = y; tempY < height; tempY++){
						background[x][tempY] = 17;
					}
				}else{
					for(int tempY = y; tempY < height; tempY++){
						background[x][tempY] = 17;
					}
					int divY = y - prevY;
					switch(divY){
					case -1:
						background[x][y] = 36;
						break;
					case -2:
						background[x][y] = 43;
						background[x][y+1] = 59;
						break;
					case 1:
						background[x][y] = 37;
						break;
					case 2:
						background[x][y] = 60						;
						background[x][y-1] = 44;
						break;
					default:
					}
					prevY = y;
				}
			}

		}
	}


}
