package musee.mapgen.utils;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.FileInputStream;
import java.io.InputStreamReader;

import musee.mapgen.SongInfo;


public class IOUtils {
	public static void readFromFile(String fileName, SongInfo result){
		try{
			// Open the file that is the first 
			// command line parameter
			FileInputStream fstream = new FileInputStream(fileName);
			// Get the object of DataInputStream
			DataInputStream in = new DataInputStream(fstream);
			BufferedReader br = new BufferedReader(new InputStreamReader(in));
			String strLine;
			//Read File Line By Line
			while ((strLine = br.readLine()) != null)   {
				// parseEachLine
				String[] subStrings = strLine.split(";");
				
				int level = Integer.parseInt(subStrings[0]);
				double magnitude = Double.parseDouble(subStrings[1]);	
				int tone = Integer.parseInt(subStrings[2]);
				
				if(level > result.maxLevel){
					result.maxLevel = level;
				}
				if(magnitude > result.maxMagnitudes){
					result.maxMagnitudes = magnitude;
				}
				if(tone > result.maxTone){
					result.maxTone = tone;
				}
				if(tone > 0 && tone < result.minNonZeroTone){
					result.minNonZeroTone = tone;
				}
				
				result.levels.add(level);
				result.beatMagnitudes.add(magnitude);
				result.tones.add(tone);
			}
			//Close the input stream
			in.close();
		}catch (Exception e){//Catch exception if any
			System.err.println("Error: " + e.getMessage());
		}
	}
}
