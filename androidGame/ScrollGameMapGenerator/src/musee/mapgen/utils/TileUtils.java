package musee.mapgen.utils;

public class TileUtils {
	public static int tileIndexToSong(int tileIndex, double tileToSampleRatio){
		int indexSong = (int)Math.round((tileIndex/tileToSampleRatio));
		return indexSong;
	}
}
