package musee.mapgen;

import java.io.FileOutputStream;
import java.io.FileWriter;
import java.io.IOException;
import java.io.OutputStream;
import java.nio.ByteBuffer;

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
		// TODO Auto-generated method stub
		testGen();
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
				tempArray[x][y] = -1;
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
