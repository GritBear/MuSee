var BUFFERS = new Array(2);
var CUR_BUFFER = null;
var CUR_BACK_BUFFER = null;
var WIDTH, HEIGHT;
var TEXTURES = {};

//var FRAME_RATE = 20;

if (!Date.now) {
    Date.now = function () { return new Date().getTime(); }
}

$(function () { 
    BUFFERS[0] = $("#buffer1");
    BUFFERS[1] = $("#buffer2");
    CUR_BUFFER = BUFFERS[0][0];
    CUR_BACK_BUFFER = BUFFERS[1][0];
    BUFFERS[1].hide();
    WIDTH = $(CUR_BUFFER).width();
    HEIGHT = $(CUR_BUFFER).height();

    //$(".texture").each(function (i, obj) {
    //    var alt = $(obj).attr("alt");
    //    TEXTURES[alt] = $(obj);
    //});

    TEXTURES["leaf1"] = document.getElementById("leaf1");
    TEXTURES["flower1"] = document.getElementById("flower1");
})

function flipDoubleBuffer() {
    $(CUR_BACK_BUFFER).show();
    $(CUR_BUFFER).hide();
    var temp = CUR_BACK_BUFFER;
    CUR_BACK_BUFFER = CUR_BUFFER;

    var ctx = CUR_BUFFER.getContext("2d");
    ctx.clearRect(0, 0, WIDTH, HEIGHT);

    CUR_BUFFER = temp;
}
