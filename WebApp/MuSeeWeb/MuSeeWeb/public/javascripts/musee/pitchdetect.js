/*
The MIT License (MIT)

Copyright (c) 2014 Chris Wilson

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

window.AudioContext = window.AudioContext || window.webkitAudioContext;
var fftSize = 2048;
var audioContext = null;
var sourceNode = null;
var analyser = null;
var theBuffer = null;
var mediaStreamSource = null;

var debug;
var soundNames = ["/sounds/lexion.mp3", "/sounds/cj.mp3", "/sounds/titanic.mp3", "/sounds/hp.mp3"];
var soundBufferArray;
var curSoundName;

window.onload = function () {
    audioContext = new AudioContext();
    MAX_SIZE = Math.max(4, Math.floor(audioContext.sampleRate / 5000));	// corresponds to a 5kHz signal
    
    soundBufferArray = new Array(soundNames.length + 1);

    startNextSongLoading(0, soundNames.length);

    melodayStore = new melodayStoreEngine();
}

function startNextSongLoading(soundid, numSound) {
    var request = new XMLHttpRequest({ mozSystem: true });
    
    request.open("GET", soundNames[soundid], true);
    request.responseType = "arraybuffer";
    request.onload = function () {
        audioContext.decodeAudioData(request.response, function (buffer) {
            theBuffer = buffer;
            console.log("load:" + soundNames[soundid]);
            curSoundName = soundNames[soundid];
            soundBufferArray[soundid] = buffer;
            if (soundid < numSound - 1) {
                startNextSongLoading(soundid + 1, numSound);
            } else { 
                $("#state").text("ready:"); 
            }
        });
    }
    request.send();
}

function acceptUploadedFile(file, callback) { 
    var reader = new FileReader();
    reader.onload = function (event) {
        audioContext.decodeAudioData(event.target.result, function (buffer) {
            theBuffer = buffer;
            soundBufferArray[soundNames.length] = buffer;

            //upload ready
            callback();
            $("#upload_play_btn").prop("disabled", false);

        }, function () { alert("error loading!"); });
    };
    reader.onerror = function (event) {
        alert("Error: " + reader.error);
    };
    reader.readAsArrayBuffer(file);

}

function toggleOscillator() {
    if (isPlaying && curSoundName == "Oscillator") {
        //stop playing and return
        stopStream();
        stopPainting();
        return "oscillator";
    } else if (isPlaying) { 
        //stop playing and return
        stopStream();
        stopPainting();
    }

    curSoundName = "Oscillator";
    sourceNode = audioContext.createOscillator();
    
    analyser = audioContext.createAnalyser();
    analyser.fftSize = fftSize;
    sourceNode.connect(analyser);
    
    var gainNode = audioContext.createGain();
    gainNode.gain.value = 0.1;

    analyser.connect(gainNode);
    gainNode.connect(audioContext.destination);

    sourceNode.start(0);
    isPlaying = true;
    isLiveInput = false;
    startPainting();

    return "Constant Pitch";
}

function error() {
    alert('Stream generation failed.');
}

function getUserMedia(dictionary, callback) {
    try {
        navigator.getUserMedia = 
        	navigator.getUserMedia ||
        	navigator.webkitGetUserMedia ||
        	navigator.mozGetUserMedia;
        navigator.getUserMedia(dictionary, callback, error);
    } catch (e) {
        alert('getUserMedia threw exception :' + e);
    }
}

var liveStream;
function toggleLiveInput() {
    if (isPlaying && curSoundName == "liveInput") {
        //stop playing and return
        stopStream();
        stopPainting();
        return "Microphone input";
    } else if (isPlaying) {
        //stop playing and return
        stopStream();
        stopPainting();
    }
    
    curSoundName = "liveInput";
    getUserMedia(
    	{
            "audio": {
                "mandatory": {
                    "googEchoCancellation": "false",
                    "googAutoGainControl": "true",
                    "googNoiseSuppression": "true",
                    "googHighpassFilter": "false"
                },
                "optional": []
            },
        }, function (stream) {
            liveStream = stream;
            // Create an AudioNode from the stream.
            mediaStreamSource = audioContext.createMediaStreamSource(stream);
            // Connect it to the destination.
            analyser = audioContext.createAnalyser();
            analyser.fftSize = fftSize;
            mediaStreamSource.connect(analyser);
            isPlaying = true;
            startPainting();
        });
}

function stopStream() {
    if(sourceNode)
        sourceNode.stop(0);
    sourceNode = null;
    analyser = null;
    isPlaying = false;
    stopPainting();

    if(liveStream)
        liveStream.stop();
    liveStream = null;
}

function togglePlayback(soundid) {
    if (isPlaying && curSoundName == soundNames[soundid]) {
        //stop playing and return
        stopStream();
        stopPainting();
        return "Demo " + soundid;
    } else if (isPlaying) {
        //stop playing and return
        stopStream();
        stopPainting();
    }
    
    curSoundName = soundNames[soundid]
    theBuffer = soundBufferArray[soundid];
    sourceNode = audioContext.createBufferSource();
    sourceNode.buffer = theBuffer;
    sourceNode.loop = true;
    
    analyser = audioContext.createAnalyser();
    analyser.fftSize = fftSize;
    sourceNode.connect(analyser);
    analyser.connect(audioContext.destination);
    sourceNode.start(0);
    isPlaying = true;
    isLiveInput = false;
    startPainting();

    return "Demo " + soundid;
}

var paintTimer;
function startPainting() {
    artStoryEngine.syncTimeToNow();

    paintTimer = window.setInterval(function () {
        if (!isPlaying) {
            artStoryEngine.syncTimeToNow();
            return;
        }
        updatePitch();
        artStoryEngine.frameUpdate();
    }, 1000 / updateRate);
    executeFrame();
}

function stopPainting() {
    window.clearInterval(paintTimer);
    if (!window.cancelAnimationFrame)
        window.cancelAnimationFrame = window.webkitCancelAnimationFrame;
    window.cancelAnimationFrame(rafID);
}

function executeFrame() {
    if (isPlaying)
        rafID = requestAnimFrame(executeFrame);
    artStoryEngine.render();
}

var rafID = null;
var tracks = null;
var buflen = 1024;
var buf = new Float32Array(buflen);

var highestAc = 0;
var acCnt = 0;

function updatePitch(time) {
    var cycles = new Array;
    analyser.getFloatTimeDomainData(buf);
    var ac = autoCorrelate(buf, audioContext.sampleRate);
    
    //preferrably choose the highest note
    if (acCnt == 0) {
        highestAc = ac;
    } else {
        highestAc = Math.max(ac, highestAc);
    }
        
    acCnt = (acCnt + 1) % acBufferSize;

    ac = highestAc;
    
    //store values into global variables
    CURRENT_NOTE = noteFromPitch(Math.round(ac));
    melodayStore.addNote(CURRENT_NOTE);
}

function noteFromPitch(frequency) {
	var noteNum = 12 * (Math.log( frequency / 440 )/Math.log(2) );
	return Math.round( noteNum ) + 69;
}

function frequencyFromNoteNumber( note ) {
	return 440 * Math.pow(2,(note-69)/12);
}

function centsOffFromPitch( frequency, note ) {
	return Math.floor( 1200 * Math.log( frequency / frequencyFromNoteNumber( note ))/Math.log(2) );
}

var MIN_SAMPLES = 0;  // will be initialized when AudioContext is created.

function autoCorrelate( buf, sampleRate ) {
	var SIZE = buf.length;
	var MAX_SAMPLES = Math.floor(SIZE/2);
	var best_offset = -1;
	var best_correlation = 0;
	var rms = 0;
	var foundGoodCorrelation = false;
	var correlations = new Array(MAX_SAMPLES);

	for (var i=0;i<SIZE;i++) {
		var val = buf[i];
		rms += val*val;
	}
	rms = Math.sqrt(rms/SIZE);
	if (rms<0.01) // not enough signal
		return -1;

	var lastCorrelation=1;
	for (var offset = MIN_SAMPLES; offset < MAX_SAMPLES; offset++) {
		var correlation = 0;

		for (var i=0; i<MAX_SAMPLES; i++) {
			correlation += Math.abs((buf[i])-(buf[i+offset]));
		}
		correlation = 1 - (correlation/MAX_SAMPLES);
		correlations[offset] = correlation; // store it, for the tweaking we need to do below.
		if ((correlation>0.9) && (correlation > lastCorrelation)) {
			foundGoodCorrelation = true;
			if (correlation > best_correlation) {
				best_correlation = correlation;
				best_offset = offset;
			}
		} else if (foundGoodCorrelation) {
			// short-circuit - we found a good correlation, then a bad one, so we'd just be seeing copies from here.
			// Now we need to tweak the offset - by interpolating between the values to the left and right of the
			// best offset, and shifting it a bit.  This is complex, and HACKY in this code (happy to take PRs!) -
			// we need to do a curve fit on correlations[] around best_offset in order to better determine precise
			// (anti-aliased) offset.

			// we know best_offset >=1, 
			// since foundGoodCorrelation cannot go to true until the second pass (offset=1), and 
			// we can't drop into this clause until the following pass (else if).
			var shift = (correlations[best_offset+1] - correlations[best_offset-1])/correlations[best_offset];  
			return sampleRate/(best_offset+(8*shift));
		}
		lastCorrelation = correlation;
	}
	if (best_correlation > 0.01) {
		// console.log("f = " + sampleRate/best_offset + "Hz (rms: " + rms + " confidence: " + best_correlation + ")")
		return sampleRate/best_offset;
	}
	return -1;
//	var best_frequency = sampleRate/best_offset;
}


