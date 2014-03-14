package musee.mapgen;

import java.util.ArrayList;

public class SongInfo{
	public ArrayList<Integer> levels;
	public ArrayList<Double> beatMagnitudes;
	public ArrayList<Integer> tones;
	public int minNonZeroTone = 9999;
	public int maxTone = -1;
	public double maxMagnitudes = -1;
	public int maxLevel = -1;
	
	public SongInfo(){
		levels = new ArrayList<Integer> ();
		beatMagnitudes = new ArrayList<Double> ();
		tones = new ArrayList<Integer> ();
	}
}
