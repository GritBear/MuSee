package musee.mapgen;

import java.util.ArrayList;

public class IdealPathCalc {
	SongInfo song;
	int length;
	ArrayList<Integer> idealPathRaw;
	ArrayList<Double> idealPathDouble;
	ArrayList<Integer> idealPath;
	
	final int upDownJumpLimit = 2;
	
	public IdealPathCalc(SongInfo aSong){
		song = aSong;
		length = song.tones.size();
		idealPathRaw = new ArrayList<Integer>(length);
		idealPathDouble = new ArrayList<Double>(length);
		idealPath = new ArrayList<Integer>(length);
		processDrive();
	}
	
	private void processDrive(){
		changeLimiting();
		lowPassPath();
		castToIntPath();
	}
	
	private void changeLimiting(){
		idealPathRaw.add(0, song.tones.get(0));
		for(int i = 1; i<length; i++){
			int prevTone =  idealPathRaw.get(i-1);
			int diff = song.tones.get(i) - prevTone;
			if(diff > upDownJumpLimit){
				diff = upDownJumpLimit;
			}else if(diff < (-upDownJumpLimit + 1)){
				diff = (-upDownJumpLimit + 1);
			}
			idealPathRaw.add(i, prevTone + diff);
		}		
	}
	
	public ArrayList<Integer> getIdealPathRaw(){
		return idealPath;
	}
		
	private void lowPassPath(){
		idealPathDouble.add(0, (double)idealPathRaw.get(0));
		for(int i = 1; i < length-1;i++){
			double value = (idealPathRaw.get(i) + idealPathRaw.get(i-1) + idealPathRaw.get(i+1))/3.0;
			idealPathDouble.add(i, value);
		}		
		idealPathDouble.add(length-1, (double)idealPathRaw.get(length-1));
	}
	
	private void castToIntPath(){
		for(int i = 0; i < length;i++){
			idealPath.add(i, (int)Math.round(idealPathDouble.get(i)));
		}		
	}
	
}
