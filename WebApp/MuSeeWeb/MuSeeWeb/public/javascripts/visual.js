var AVG_X_MOVE_SPEED; //it takes 8 second to move from right to left
var dispatcher;
var renderTimer;
var MIN_NOTE = 50;
var MAX_NOTE = 100;
var NOTE_SPAN;

var artStoryEngine;
$(function () {
    //some test sample code
    //var ctx = CUR_BUFFER.getContext("2d");
    //var img = TEXTURES["leaf1"];
    //ctx.drawImage(img, 10, 10);
    //ctx.drawImage(img, 100, 100);
    
    //TODO: fix this equation
    AVG_X_MOVE_SPEED = -WIDTH / 25 / updateRate;
    NOTE_SPAN = MAX_NOTE - MIN_NOTE;
    console.log(AVG_X_MOVE_SPEED);
    artStoryEngine = new artStoryEngine();
    startPainting();
})

var renderTimer;
//function startPainting() {
//    renderTimer = window.setInterval(function () {
//        //render update goes here
//        if (isPlaying) { 
//            artStoryEngine.frameUpdate();
//        }
//        console.log("timer clicked");
//    }, 1000 / FRAME_RATE);
//}

function stopPainting() {
    window.clearInterval(renderTimer);
}

function artStoryEngine() { 
    this.dispatcher = new dispatcher({maxSize : 50});
    this.aniFactory = new makeAnimationFac();

    this.frameUpdate = function () {
        var newAni = this.aniFactory.make();
        if (newAni) {
            this.dispatcher.add(newAni);
        }
        
        //move elements
        this.dispatcher.apply(function (obj) { 
            obj.move(AVG_X_MOVE_SPEED, 0);
        });
        
        this.aniFactory.move(AVG_X_MOVE_SPEED, 0);
        
        var ctx = CUR_BACK_BUFFER.getContext("2d");
        //paint elements
        this.dispatcher.apply(function (obj) {
            obj.render(ctx);
        });

        flipDoubleBuffer();
    }
}

function makeAnimationFac() {
    this.prevMade = 0;
    this.minTimeDiff = 0.6 * 1000; // 0.6 seconds
    this.prevXEnd = -1;
    this.prevYEnd = -1;

    this.make = function () {
        if (Date.now() - this.prevMade < this.minTimeDiff || !CURRENT_NOTE) { 
            return null;
        }

        var curXEnd = WIDTH / 2.2;
        var curYEnd = HEIGHT - ((CURRENT_NOTE - MIN_NOTE) / NOTE_SPAN) * HEIGHT;
        
        if (this.prevXEnd == -1 && this.prevYEnd == -1) {
            this.prevMade = Date.now();
            this.prevXEnd = curXEnd;
            this.prevYEnd = curYEnd;
            return null;
        }
    
        //making a new element
        var xDis = curXEnd - this.prevXEnd;
        var yDis = curYEnd - this.prevYEnd;

        var diagSquare = Math.pow(xDis, 2) + Math.pow(xDis, 2);
        var width = Math.sqrt(diagSquare / 2);
        var height = width; //for leaf1
        
        var angle = Math.PI / 4 - Math.atan(yDis / xDis);
        var x = xDis / 2 + this.prevXEnd;
        var y = yDis / 2 + this.prevYEnd;
        
        //console.log("make image");
        //console.log(HEIGHT);
        //console.log(CURRENT_NOTE - MIN_NOTE);
        //console.log(this.prevYEnd);
        //console.log(curXEnd);

        //console.log(xDis);
        //console.log(yDis);

        //console.log(x);
        //console.log(y);
        //console.log(width);
        //console.log(height);

        //var newSquare = new AnimatedObject({
        //    name : "leaf", 
        //    width : width, 
        //    height : height, 
        //    textureImg : TEXTURES["leaf1"],
        //    x : x,
        //    y : y,
        //    angle : angle
        //});
        
        var newSquare = new AnimatedObject({
            name : "leaf", 
            width : 60, 
            height : 60, 
            textureImg : TEXTURES["leaf1"],
            x : WIDTH / 2,
            y : HEIGHT / 2,
            angle : 0
        });

        this.prevMade = Date.now();
        this.prevXEnd = curXEnd;
        this.prevYEnd = curYEnd;
        return newSquare;
    }

    this.move = function (dx, dy) {
        this.prevXEnd += dx;
        this.prevYEnd += dy;
    }
}

function dispatcher(param) {
    this.maxSize = param.maxSize;
    this.elements = new Array();

    this.add = function (obj) {
        this.elements.push(obj);
        this.maintainSize();
    }

    this.purge = function () { 
        this.elements = new Array();
    }
    
    this.maintainSize = function () {
        while (this.elements.length > this.maxSize) { 
            this.elements.shift();
        }
    }

    this.apply = function (visitorFunction) {
        for (i = 0; i < this.elements.length; i++) {
            var aniElement = this.elements[i];
            visitorFunction(aniElement); //skip host accept step
        }
    }
}


function AnimatedObject(params) {
    this.name = params.name;
    this.width = params.width;
    this.height = params.height;
    this.textureImg = params.textureImg;
    
    this.x = params.x;
    this.y = params.y;
    this.angle = params.angle; //in radia
    
    //in the future implement velocity element
    this.move = function (dx, dy) {
        this.x += dx;
        this.y += dy;
    }

    this.render = function ( ctx ) {
        //console.log("print image");
        //console.log(this.x);
        //console.log(this.y);
        //console.log(this.width);
        //console.log(this.height);

        ctx.translate(this.x, this.y);
        ctx.rotate(this.angle);
        ctx.drawImage(this.textureImg, -this.width / 2, -this.height / 2, this.width, this.height);
        ctx.rotate(this.angle);
        ctx.translate(-this.x, -this.y);
    }

}