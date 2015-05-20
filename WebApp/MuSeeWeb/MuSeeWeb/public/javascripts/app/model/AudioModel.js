define(['jquery','./MelodyStore'], function ($, MelodyStore) {
    function audioModel() {
        window.AudioContext = window.AudioContext || window.webkitAudioContext;
        
        this.fftSize = 2048;
        this.audioContext = null;
        this.sourceNode = null;
        this.analyser = null;
        this.theBuffer = null;
        this.mediaStreamSource = null;
        
        this.soundNames = ["/sounds/lexion.mp3", "/sounds/cj.mp3", "/sounds/titanic.mp3", "/sounds/hp.mp3"];
        this.soundBufferArray;
        this.curSoundName;

        this.updateRate = 40;
        this.acBufferSize = 10; //every 1/4 second
        this.CURRENT_NOTE = 60;
        this.isPlaying = false;
        this.melodayStore = null;

        this.init = function () { 
            this.audioContext = new AudioContext();
            this.MAX_SIZE = Math.max(4, Math.floor(this.audioContext.sampleRate / 5000));	// corresponds to a 5kHz signal
            
            this.soundBufferArray = new Array(this.soundNames.length + 1);
            this.melodayStore = new MelodyStore();
            
            this.startNextSongLoading(0, this.soundNames.length, this);
        }

        this.startNextSongLoading = function (soundid, numSound, baseAudioModel) {
            var request = new XMLHttpRequest({ mozSystem: true });
            
            //copy reference
            var audioContext = baseAudioModel.audioContext;
            var theBuffer = baseAudioModel.theBuffer;
            var soundNames = baseAudioModel.soundNames;
            var curSoundName = baseAudioModel.curSoundName;
            var soundBufferArray = baseAudioModel.soundBufferArray;
            var startNextSongLoading = baseAudioModel.startNextSongLoading;
            
            request.open("GET", soundNames[soundid], true);
            request.responseType = "arraybuffer";
            request.onload = function () {
                audioContext.decodeAudioData(request.response, function (buffer) {
                    theBuffer = buffer;
                    console.log("load:" + soundNames[soundid]);
                    curSoundName = soundNames[soundid];
                    soundBufferArray[soundid] = buffer;
                    if (soundid < numSound - 1) {
                        startNextSongLoading(soundid + 1, numSound, baseAudioModel);
                    } else {
                        $("#state").text("ready:");
                    }
                });
            }
            request.send();
        }

        this.acceptUploadedFile = function (file, callback) {
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



    }

    
    return audioModel;
});
