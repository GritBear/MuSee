var BUFFERS = new Array(2);
var CUR_BUFFER = null;
var CUR_BUFFER_IND = 0;
var WIDTH, HEIGHT;

$(function () { 
    BUFFERS[0] = $("#buffer1");
    BUFFERS[1] = $("#buffer2");
    CUR_BUFFER = BUFFERS[0];
    BUFFERS[0].hide();
    WIDTH = CUR_BUFFER.width();
    HEIGHT = CUR_BUFFER.height();
})