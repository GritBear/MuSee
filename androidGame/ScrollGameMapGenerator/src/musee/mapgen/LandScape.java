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
		int prevY = 0;
		for(int x = 0; x < width; x++){
			int songIndex = TileUtils.tileIndexToSong(x, Consts.tileToSampleRatio);
			int toneDeviation = idealPath.getIdealPathRaw().get(songIndex) - song.minNonZeroTone;

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
