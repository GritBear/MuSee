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
var fftSize = 1024;
var audioContext = null;
var isPlaying = false;
var sourceNode = null;
var analyser = null;
var theBuffer = null;
var DEBUGCANVAS = null;
var mediaStreamSource = null;
var detectorElem, 
    canvasElem,
    waveCanvas,
    pitchElem,
    noteElem,
    detuneElem,
    detuneAmount;


var debug;

window.onload = function () {
    audioContext = new AudioContext();
    MAX_SIZE = Math.max(4, Math.floor(audioContext.sampleRate / 5000));	// corresponds to a 5kHz signal
    var request = new XMLHttpRequest({ mozSystem: true });
    request.open("GET", "/sounds/cj.mp3", true);
    request.responseType = "arraybuffer";
    request.onload = function () {
        audioContext.decodeAudioData(request.response, function (buffer) {
            theBuffer = buffer;
        });
    }
    request.send();
    
    detectorElem = document.getElementById("detector");
    canvasElem = document.getElementById("output");
    DEBUGCANVAS = document.getElementById("waveform");
    if (DEBUGCANVAS) {
        waveCanvas = DEBUGCANVAS.getContext("2d");
        waveCanvas.strokeStyle = "black";
        waveCanvas.lineWidth = 1;
    }
    pitchElem = document.getElementById("pitch");
    noteElem = document.getElementById("note");
    detuneElem = document.getElementById("detune");
    detuneAmount = document.getElementById("detune_amt");
    
    detectorElem.ondragenter = function () {
        this.classList.add("droptarget");
        return false;
    };
    detectorElem.ondragleave = function () { this.classList.remove("droptarget"); return false; };
    detectorElem.ondrop = function (e) {
        this.classList.remove("droptarget");
        e.preventDefault();
        theBuffer = null;
        
        var reader = new FileReader();
        reader.onload = function (event) {
            audioContext.decodeAudioData(event.target.result, function (buffer) {
                theBuffer = buffer;
            }, function () { alert("error loading!"); });

        };
        reader.onerror = function (event) {
            alert("Error: " + reader.error);
        };
        reader.readAsArrayBuffer(e.dataTransfer.files[0]);
        return false;
    };
    
    for (i = 0; i < fbufDuration2D; i++) {
        fbuf2D[i] = new Float32Array(fbuflen);
    }
}

//function error() {
//    alert('Stream generation failed.');
//}

//function getUserMedia(dictionary, callback) {
//    try {
//        navigator.getUserMedia = 
//        	navigator.getUserMedia ||
//        	navigator.webkitGetUserMedia ||
//        	navigator.mozGetUserMedia;
//        navigator.getUserMedia(dictionary, callback, error);
//    } catch (e) {
//        alert('getUserMedia threw exception :' + e);
//    }
//}



function toggleOscillator() {
    if (isPlaying) {
        //stop playing and return
        sourceNode.stop(0);
        sourceNode = null;
        analyser = null;
        isPlaying = false;
        stopPainting();
        return "play oscillator";
    }
    sourceNode = audioContext.createOscillator();
    
    analyser = audioContext.createAnalyser();
    analyser.fftSize = fftSize;
    sourceNode.connect(analyser);
    analyser.connect(audioContext.destination);
    sourceNode.start(0);
    isPlaying = true;
    isLiveInput = false;
    startPainting();
    
    return "stop";
}

//function gotStream(stream) {
//    // Create an AudioNode from the stream.
//    mediaStreamSource = audioContext.createMediaStreamSource(stream);

//    // Connect it to the destination.
//    analyser = audioContext.createAnalyser();
//    analyser.fftSize = fftSize;
//    mediaStreamSource.connect(analyser);
//    updatePitch();
//}
//function toggleLiveInput() {
//    if (isPlaying) {
//        //stop playing and return
//        sourceNode.stop( 0 );
//        sourceNode = null;
//        analyser = null;
//        isPlaying = false;
//		if (!window.cancelAnimationFrame)
//			window.cancelAnimationFrame = window.webkitCancelAnimationFrame;
//        window.cancelAnimationFrame( rafID );
//    }
//    getUserMedia(
//    	{
//            "audio": {
//                "mandatory": {
//                    "googEchoCancellation": "false",
//                    "googAutoGainControl": "false",
//                    "googNoiseSuppression": "false",
//                    "googHighpassFilter": "false"
//                },
//                "optional": []
//            },
//        }, gotStream);
//}

function togglePlayback() {
    if (isPlaying) {
        //stop playing and return
        sourceNode.stop(0);
        sourceNode = null;
        analyser = null;
        isPlaying = false;
        stopPainting();
        return "start";
    }
    
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
    
    return "stop";
}

var paintTimer;
function startPainting() {
    paintTimer = window.setInterval(function () {
        rafID = window.requestAnimationFrame(updatePitch);
    }, 1000 / updateRate);
}

function stopPainting() {
    window.clearInterval(paintTimer);
    if (!window.cancelAnimationFrame)
        window.cancelAnimationFrame = window.webkitCancelAnimationFrame;
    window.cancelAnimationFrame(rafID);
}

var rafID = null;
var tracks = null;
var buflen = 1024;
var buf = new Float32Array(buflen);

//frequency domain buffer
var fbuflen = fftSize / 2 / 10; //limit storage to up to 2k Hz
var fbuf = new Float32Array(fbuflen);
var fbufDuration2D = 5;
var fbuf2D = new Array(fbufDuration2D);
var fbuf2DCnt = 0;

var highestAc = 0;
var acCnt = 0;

function updatePitch(time) {
    var cycles = new Array;
    analyser.getFloatTimeDomainData(buf);
    var ac = autoCorrelate(buf, audioContext.sampleRate);
    // TODO: Paint confidence meter on canvasElem here.
    
    //conduct processing in frequency domain
    //analyser.getFloatFrequencyData(fbuf);
    //var matrixFull = storeMatrix(fbuf);
    //if (matrixFull) {
    //    melodyExtract();
    //}
    
    if (acCnt == 0) {
        highestAc = ac;
    } else {
        highestAc = Math.max(ac, highestAc);
    }
    
    acCnt = (acCnt + 1) % acBufferSize;
    
    ac = highestAc;
    
    //store values into global variables
    CURRENT_NOTE = ac;
    NOTE_HISTORY[note_buffer_cnt] = CURRENT_NOTE;
    note_buffer_cnt = (note_buffer_cnt + 1) % Note_bufferSize;
    
    if (ac == -1) {
        detectorElem.className = "vague";
        pitchElem.innerText = "--";
        noteElem.innerText = "-";
        detuneElem.className = "";
        detuneAmount.innerText = "--";
    } else {
        detectorElem.className = "confident";
        pitch = ac;
        pitchElem.innerText = Math.round(pitch);
        var note = noteFromPitch(pitch);
        //noteElem.innerHTML = noteStrings[note % 12];
        noteElem.innerHTML = note;
        //var detune = centsOffFromPitch(pitch, note);
        //if (detune == 0) {
        //    detuneElem.className = "";
        //    detuneAmount.innerHTML = "--";
        //} else {
        //    if (detune < 0)
        //        detuneElem.className = "flat";
        //    else
        //        detuneElem.className = "sharp";
        //    detuneAmount.innerHTML = Math.abs(detune);
        //}
    }
}

function storeMatrix(fbuf) {
    //copy the 1D frequency buffer into the 2D matrix, at the CNT
    for (i = 0; i < fbuflen; i++) {
        fbuf2D[fbuf2DCnt][i] = fbuf[i];
    }
    
    fbuf2DCnt = (fbuf2DCnt + 1) % fbufDuration2D;
    
    return fbuf2DCnt == 0;
}

//this can be machine learned or simple matrix operation
function melodyExtract() {
    SpectrumMatrixAnalysis();
}

var max, min;
var NormalizationPower = 3;
var endCompensationScale = 4;
var startCompensationScale = 1;
var FrequencySeparationPoint = [100, 300, 1200, 20000];
var FrequencyInterval = 40000 / fftSize;
var MaxNormalizedValue = 10;
var dynamicThreashold = 8;

var dynamicIncreaseFactor = 0.8; //both factor has to be less than 1
var dynamicDecreaseFactor = 0.9;
var dynamicPointsMinimum = FrequencySeparationPoint.length - 1;
var dynamicPointsMaximum = FrequencySeparationPoint.length + 1;

var bassMelodyDevideTone = 2;
var curBaseNote = 0;
var curNonBaseNote = 0;

function SpectrumMatrixAnalysis() {
    PowerMatrix(NormalizationPower); //power up and normalize
    ScalerAddMatrix(); //integrate HighFreqCompensation inside
    
    StepFreRegistration(); //Including the Tone Translation
}

function PowerMatrix(power) {
    min = fbuf2D[0][0];
    max = fbuf2D[0][0];
    
    for (i = 0; i < fbufDuration2D; i++) {
        for (j = 0; j < fbuflen; j++) {
            var temp = Math.pow(fbuf2D[i][j], power);
            max = (max < temp)? temp : max;
            min = (min > temp)? temp : min;
            fbuf2D[i][j] = temp;
        }
    }
}

function ScalerAddMatrix() {
    var slope = (endCompensationScale - startCompensationScale) / fbuflen;
    
    for (i = 0; i < fbuflen; i++) {
        var scaleFreq = (i + 1.0) * slope + startCompensationScale;
        for (j = 0; j < fbufDuration2D; j++) {
            fbuf2D[j][i] = (fbuf2D[j][i] - min) / (max - min) * scaleFreq;
        }
    }

}

function StepFreRegistration() {
    //Sum the melodyStorage into Buffer
    for (i = 1; i < fbufDuration2D; i++) {
        for (j = 0; j < fbuflen; j++) {
            fbuf2D[0][j] += fbuf2D[i][j];
        }
    }
    
    // perform Banded Normalization
    var cntBand = 0;
    var BandMax = 0;
    var BandStart = 0;
    for (i = 0; i <= fbuflen; i++) {
        if (i * FrequencyInterval > FrequencySeparationPoint[cntBand]) {
            if (BandMax != 0) {
                var factor = MaxNormalizedValue / BandMax;
                for (j = BandStart; j < i; j++) {
                    fbuf2D[0][j] = fbuf2D[0][i] * factor;
                    if (fbuf2D[0][j] > dynamicThreashold) {
                        fbuf2D[0][j] = MaxNormalizedValue;
                    } else {
                        fbuf2D[0][j] = 0;
                    }
                }
            }
            cntBand++;
            BandStart = i;
            BandMax = 0;
        }
        BandMax = (BandMax < fbuf2D[0][i]) ? fbuf2D[0][i] : BandMax;
    }//Bands
    
    StepToneRegistrationFromFreq();
}

function StepToneRegistrationFromFreq() {
    var num_tones = 0;
    
    // the extracted tone indexes are from low to high
    for (i = 0; i < fbuflen; i++) {
        if (fbuf2D[0][i] == MaxNormalizedValue) {
            var tonePoint = noteFromPitch(i * FrequencyInterval);
            debug.innerText = i * FrequencyInterval;
            //push the tone into either bass or melody
            if (tonePoint < bassMelodyDevideTone) {
                curBaseNote = tonePoint;
                num_tones++;
            }
            else {
                curNonBaseNote = tonePoint;
                num_tones++;
            }
        }
    }
    
    //perform dynamic threshold adjustment
    DynamicThresholdAdjustment(num_tones);
}

function DynamicThresholdAdjustment(num_tones) {
    if (num_tones < dynamicPointsMinimum) {
        dynamicThreshold = dynamicThreshold * dynamicDecreaseFactor;
    } else if (num_tones > dynamicPointsMaximum) {
        dynamicThreshold += (MaxNormalizedValue - dynamicThreshold) * dynamicIncreaseFactor;
    }
}

var noteStrings = ["C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"];

function noteFromPitch(frequency) {
    var noteNum = 12 * (Math.log(frequency / 440) / Math.log(2));
    return Math.round(noteNum) + 69;
}

function frequencyFromNoteNumber(note) {
    return 440 * Math.pow(2, (note - 69) / 12);
}

function centsOffFromPitch(frequency, note) {
    return Math.floor(1200 * Math.log(frequency / frequencyFromNoteNumber(note)) / Math.log(2));
}

// this is a float version of the algorithm below - but it's not currently used.
/*
function autoCorrelateFloat( buf, sampleRate ) {
	var MIN_SAMPLES = 4;	// corresponds to an 11kHz signal
	var MAX_SAMPLES = 1000; // corresponds to a 44Hz signal
	var SIZE = 1000;
	var best_offset = -1;
	var best_correlation = 0;
	var rms = 0;

	if (buf.length < (SIZE + MAX_SAMPLES - MIN_SAMPLES))
		return -1;  // Not enough data

	for (var i=0;i<SIZE;i++)
		rms += buf[i]*buf[i];
	rms = Math.sqrt(rms/SIZE);

	for (var offset = MIN_SAMPLES; offset <= MAX_SAMPLES; offset++) {
		var correlation = 0;

		for (var i=0; i<SIZE; i++) {
			correlation += Math.abs(buf[i]-buf[i+offset]);
		}
		correlation = 1 - (correlation/SIZE);
		if (correlation > best_correlation) {
			best_correlation = correlation;
			best_offset = offset;
		}
	}
	if ((rms>0.1)&&(best_correlation > 0.1)) {
		console.log("f = " + sampleRate/best_offset + "Hz (rms: " + rms + " confidence: " + best_correlation + ")");
	}
//	var best_frequency = sampleRate/best_offset;
}
*/

var MIN_SAMPLES = 0;  // will be initialized when AudioContext is created.

function autoCorrelate(buf, sampleRate) {
    var SIZE = buf.length;
    var MAX_SAMPLES = Math.floor(SIZE / 2);
    var best_offset = -1;
    var best_correlation = 0;
    var rms = 0;
    var foundGoodCorrelation = false;
    var correlations = new Array(MAX_SAMPLES);
    
    for (var i = 0; i < SIZE; i++) {
        var val = buf[i];
        rms += val * val;
    }
    rms = Math.sqrt(rms / SIZE);
    if (rms < 0.01) // not enough signal
        return -1;
    
    var lastCorrelation = 1;
    for (var offset = MIN_SAMPLES; offset < MAX_SAMPLES; offset++) {
        var correlation = 0;
        
        for (var i = 0; i < MAX_SAMPLES; i++) {
            correlation += Math.abs((buf[i]) - (buf[i + offset]));
        }
        correlation = 1 - (correlation / MAX_SAMPLES);
        correlations[offset] = correlation; // store it, for the tweaking we need to do below.
        if ((correlation > 0.9) && (correlation > lastCorrelation)) {
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
            var shift = (correlations[best_offset + 1] - correlations[best_offset - 1]) / correlations[best_offset];
            return sampleRate / (best_offset + (8 * shift));
        }
        lastCorrelation = correlation;
    }
    if (best_correlation > 0.01) {
        // console.log("f = " + sampleRate/best_offset + "Hz (rms: " + rms + " confidence: " + best_correlation + ")")
        return sampleRate / best_offset;
    }
    return -1;
//	var best_frequency = sampleRate/best_offset;
}


//========================
var engine = new RainyDay({
    image: element,         // Image element
    // This value is required
    parentElement: someDiv, // Element to be used as a parent for the canvas
    // If not provided assuming the 'body' element
    crop: [0, 0, 50, 60],   // Coordinates if only a part of the image should be used
    // If not provided entire image will be used
    blur: 10,               // Defines blur due to rain effect
    // Assuming 10 if not provided
    // Use 0 value to disable the blur
    opacity: 1              // Opacity of rain drops
                            // Assuming 1 if not provided
});
engine.rain(
    [
        [1, 0, 20],         // add 20 drops of size 1...
        [3, 3, 1]           // ... and 1 drop of size from 3 - 6 ...
    ],                       
    100);                   // ... every 100ms