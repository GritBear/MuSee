package musee.mapgen;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.nio.ByteBuffer;
import java.util.ArrayList;

import musee.mapgen.utils.IOUtils;
import musee.mapgen.utils.TileUtils;

/**
 * generate a side scroll game readable binary map file
 * @author Grit
 *
 */
public class MapGen {
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		testGen();
		final String fileName = "museeLog.txt";
		SongInfo songResult = new SongInfo();
		IOUtils.readFromFile(fileName, songResult);	
		songTestGen(songResult);
	}

	public static void songTestGen(SongInfo song){
		String name = "neon_pegasus_map.bin";
		final int songDurationSeconds = 2*60+54;
		int songLength = song.levels.size();
		Consts.sampleRatePerSecond = (double)songLength/(double)songDurationSeconds;
		Consts.tileToSampleRatio = Consts.tileSpeed / Consts.sampleRatePerSecond; //ideally this is 1
		
		int tileLength = (int)(songLength * Consts.tileToSampleRatio + 0.5);
		System.out.println("sample per second:" + Consts.sampleRatePerSecond + ";songLength seconds:" +songDurationSeconds + ";songLength:" + songLength
				+";tileToSampleRatio:" + Consts.tileToSampleRatio + ";tile length:" + tileLength);
		
		int toneRange = song.maxTone - song.minNonZeroTone;
		int tileDownRange = (toneRange * Consts.tilesPerTone + 2 > Consts.MIN_TILE_DOWN)?toneRange * Consts.tilesPerTone + 2:Consts.MIN_TILE_DOWN;

		final int height = tileDownRange;
		final int width = tileLength;

		int[][] backgroundIndexArray = initArray(width, height);
		int[][] ObjectArray = initArray(width, height);
		int[][] ObjectArray2 = initArray(width, height);

		//add boundaries
		for(int x = 0; x < width; x++){
			if(x<10){
				backgroundIndexArray[x][0] = 34;
				backgroundIndexArray[x][height - 1] = 34;
			}else if(width - x < 10){
				backgroundIndexArray[x][0] = 26;
				backgroundIndexArray[x][height - 1] = 26;
			}else{
				backgroundIndexArray[x][0] = 17;
				backgroundIndexArray[x][height - 1] = 17;
			}
		}

		IdealPathCalc pathCalc = new IdealPathCalc(song);
		ArrayList<Integer> idealPath = pathCalc.getIdealPathRaw();

		//generate cold coin
		for(int x = 0; x < width; x++){
			int songIndex = TileUtils.tileIndexToSong(x, Consts.tileToSampleRatio);
			int toneDeviation = idealPath.get(songIndex) - song.minNonZeroTone;
			int level = song.levels.get(songIndex);
			if(toneDeviation > 0 && level > 0 && x > 15){
				int yInverted = toneDeviation * Consts.tilesPerTone;
				if(0<yInverted && yInverted< height - 1){
					int y = (height - 1) - yInverted;
//					System.out.println("x:" + x +";y:" + y + ";height:" + height);
					ObjectArray2[x][y] = Consts.COIN;
				}
			}
		}
		
		LandScape landScape = new LandScape(song, pathCalc);
		landScape.processBackground(backgroundIndexArray, ObjectArray2, width, height);
		
		//add in player
		ObjectArray[2][height/2] = Consts.PLAYER_INDEX;

		//now needs to write into a file
		int levelSignature = 96;
		int layercount = 4;
		int backgroundindex = 2; // this is for island	

		try {
			OutputStream fstream = new FileOutputStream(name);
			fstream.write(intToByte1(levelSignature));
			fstream.write(intToByte1(layercount));
			fstream.write(intToByte1(backgroundindex));

			writeALayer(fstream, 42, 1, backgroundindex, 1.0f, backgroundIndexArray, width, height);
			writeALayer(fstream, 42, 0, backgroundindex, 1.0f, backgroundIndexArray, width, height);
			writeALayer(fstream, 42, 2, backgroundindex, 1.0f, ObjectArray, width, height);
			writeALayer(fstream, 42, 2, backgroundindex, 1.0f, ObjectArray2, width, height);

			fstream.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		System.out.println("test song generate successful");
	}

	public static void testGen(){
		final String name = "test_map.bin";
		final int width = 800;
		final int height = 20;
		int[][] backgroundIndexArray = initArray(width, height);
		int[][] ObjectArray = initArray(width, height);
		int backgroundIndex  = 17; //square block for testing

		for(int x = 0; x < width; x++){
			if(x<10){

				backgroundIndexArray[x][0] = 34;
				backgroundIndexArray[x][height - 1] = 34;

			}else if(width - x < 10){
				backgroundIndexArray[x][0] = 26;
				backgroundIndexArray[x][height - 1] = 26;
			}else{

				backgroundIndexArray[x][0] = backgroundIndex;
				backgroundIndexArray[x][height - 1] = backgroundIndex;
			}
		}

		ObjectArray[2][2] = 0; //this is the player spawning location

		ObjectArray[2][18] = 1;
		ObjectArray[20][17] = 1;

		//now needs to write into a file
		int levelSignature = 96;
		int layercount = 3;
		int backgroundindex = 2; // this is for island	

		try {
			OutputStream fstream = new FileOutputStream(name);
			fstream.write(intToByte1(levelSignature));
			fstream.write(intToByte1(layercount));
			fstream.write(intToByte1(backgroundindex));

			writeALayer(fstream, 42, 1, 2, 1.0f, backgroundIndexArray, width, height);
			writeALayer(fstream, 42,0, 2, 1.0f, backgroundIndexArray, width, height);
			writeALayer(fstream, 42,2, 2, 1.0f, ObjectArray, width, height);

			fstream.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		System.out.println("test generate successful");
	}

	public static int[][] initArray( int width, int height){
		int[][] tempArray = new int[width][height];
		for(int x = 0; x < width; x++){
			for(int y=0; y< height; y++){
				tempArray[x][y] = -99;
			}
		}
		return tempArray;
	}

	public static byte intToByte1(int value){
		ByteBuffer b = ByteBuffer.allocate(4);
		b.putInt(value);
		return b.array()[3];
	}

	public static byte[] intToByte4(int value){
		ByteBuffer b = ByteBuffer.allocate(4);
		b.putInt(value);
		return invertByte4(b.array());
	}

	public static byte[] invertByte4(byte[] input){
		byte[] temp = new byte[4];
		int cnt = 0;
		for(int i = 3; i >=0 ; i--){
			temp[i] = input[cnt];
			cnt++;
		}
		return temp;
	}

	public static byte[] floatToByte4(float value){
		ByteBuffer b = ByteBuffer.allocate(4);
		b.putFloat(value);

		return invertByte4(b.array());
	}

	public static void writeALayer(OutputStream fstream, int signiture, int type, int theme, float scrollSpeed, int[][] array, int width, int height) throws IOException
	{
		fstream.write(intToByte1(type));
		fstream.write(intToByte1(theme));
		fstream.write(floatToByte4(scrollSpeed));


		//TODO height and width have to be longer, or the game would require dynamic loading
		fstream.write(intToByte1(signiture));
		fstream.write(intToByte4(width));
		fstream.write(intToByte4(height));

		for(int y = 0; y <height; y++ ){
			for(int x = 0; x < width; x++){
				fstream.write(array[x][y]);
			}
		}
	}
}
