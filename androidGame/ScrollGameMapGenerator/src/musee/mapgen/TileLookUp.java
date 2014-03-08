package musee.mapgen;

import java.util.HashMap;
import java.util.Map;

public class TileLookUp {
	public static TileLookUp singleton = new TileLookUp();
	
	Map<Integer, Double> tileEndPointMap;
	Map<Integer, Double> tileStartPointMap;
	
	TileLookUp(){
		tileEndPointMap = new HashMap<Integer,Double>();
		tileStartPointMap = new HashMap<Integer,Double>();
		init();
	}
	
	public double getStartY(int tileIndex){
		return tileStartPointMap.get(tileIndex);
	}
	
	public double getEndY(int tileIndex){
		return tileEndPointMap.get(tileIndex);
	}
	
	private void init(){
		tileStartPointMap.put(17, 1.0);
		tileEndPointMap.put(17, 1.0);
		
		tileStartPointMap.put(36, 0.0);
		tileEndPointMap.put(36, 1.0);
		
		tileStartPointMap.put(37, 1.0);
		tileEndPointMap.put(37, 0.0);
		
		tileStartPointMap.put(43, -0.5); //this means x starts at 0.5, Y = 0.0
		tileEndPointMap.put(43, 1.0);
		
		tileStartPointMap.put(59, 0.0); 
		tileEndPointMap.put(59, -1.5);//this means x starts at 0.5, Y = 1.0
		
		tileStartPointMap.put(44, 1.0);
		tileEndPointMap.put(44, -0.5);//this means x starts at 0.5
		
		tileStartPointMap.put(59, -1.5);//this means x starts at 0.5, Y = 1.0 
		tileEndPointMap.put(59, 0.0);

		
	}
	
}
