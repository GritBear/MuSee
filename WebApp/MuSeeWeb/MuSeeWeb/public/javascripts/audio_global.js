var updateRate = 40;
var acBufferSize = 10; //every 1/4 second
var CURRENT_NOTE = 60;

var Note_bufferSize = 4 * 10; // for 10 seconds
var NOTE_HISTORY = new Array(Note_bufferSize);
var note_buffer_cnt = 0;
var isPlaying = false;

