#pragma once
//class creation parameters
struct MelodyExtractionPram {
	int melodyExtractRate;   //extract melody N times a second
	int pulseFreq; //N update per second
	int numSpectrumEntries;
	int numWaveformEntries;
	int numMaxChannels;
};

//music feature processing update data
struct RenderVisualData {
	unsigned __int8		numWaveformChannels;
	unsigned __int8 **	waveformData;
	
	unsigned __int8		numSpectrumChannels;
	unsigned __int8	**	spectrumData;
};

//music result return struct
struct MusicFeature {


};