var AVG_X_MOVE_SPEED; //it takes 8 second to move from right to left
var dispatcher;
var renderTimer;
var MIN_NOTE = 50;
var MAX_NOTE = 100;
var NOTE_SPAN;
var ENTRANCE_DURATION = 1;

var artStoryEngine;
$(function () {
    //some test sample code
    //var ctx = CUR_BUFFER.getContext("2d");
    //var img = TEXTURES["leaf1"];
    //ctx.drawImage(img, 10, 10);
    //ctx.drawImage(img, 100, 100);
    
    //TODO: fix this equation
    //speed per second
    AVG_X_MOVE_SPEED = -WIDTH / 25; 
    NOTE_SPAN = MAX_NOTE - MIN_NOTE;
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

//function stopPainting() {
//    window.clearInterval(renderTimer);
//}

function artStoryEngine() { 
    this.dispatcher = new dispatcher({maxSize : 20});
    this.aniFactory = new makeAnimationFac();
    this.backObjFac = new makeBackgroundObjFac();
    this.time = Date.now();
    
    this.syncTimeToNow = function () { 
        this.time = Date.now();
    }

    this.frameUpdate = function () {
        var newAni = this.aniFactory.make();
        var newBackObj = this.backObjFac.make();

        var timeNow = Date.now();
        var timeDiff = (timeNow - this.time) / 1000;
        this.time = timeNow;

        if (newAni) {
            this.dispatcher.add(newAni);
        }
        
        if (newBackObj) { 
            this.dispatcher.add(newBackObj);
        }
        
        //move elements
        var xSpeed = timeDiff * AVG_X_MOVE_SPEED;

        this.dispatcher.apply(function (obj) { 
            obj.move(xSpeed, 0);
        });
        
        this.aniFactory.move(xSpeed, 0);
        this.backObjFac.move(xSpeed, 0);
    }

    this.render = function () {
        var ctx = CUR_BACK_BUFFER.getContext("2d");
        //paint elements
        this.dispatcher.apply(function (obj) {
            obj.render(ctx);
        });
        
        flipDoubleBuffer();
    }
}

function makeAnimationFac() {
    this.minTimeDiff = 1.0 * 1000; // 0.6 seconds
    this.minDiaDistanceSquare = 100 * 100;
    this.maxDiaDistanceSquare = 120 * 120;
    this.maxDiaDistance = Math.sqrt(this.maxDiaDistanceSquare);
    this.maxAngleChange = Math.PI / 4;

    this.prevMade = 0;
    this.prevXEnd = WIDTH * 0.75;
    this.prevYEnd = HEIGHT / 2;
    this.prevAngle = Math.PI / 4;
    this.delayCnt = 1;

    this.make = function () {
        if (Date.now() - this.prevMade < this.minTimeDiff || !CURRENT_NOTE) { 
            return null;
        }
        
        var curPitch = melodayStore.getMovingAvg();
        
        var curXEnd = WIDTH * 0.75;
        var curYEnd = HEIGHT - ((curPitch - MIN_NOTE) / NOTE_SPAN) * HEIGHT;
        
        if (isNaN(curYEnd)) { 
            curYEnd = this.prevYEnd;
        }

        if (this.delayCnt-- > 0) {
            //enforce initial delay
            this.prevMade = Date.now();
            this.prevXEnd = curXEnd;
            this.prevYEnd = curYEnd;
            return null;
        }
    
        //making a new element
        var xDis = curXEnd - this.prevXEnd;
        var yDis = curYEnd - this.prevYEnd;
        
        if (xDis == 0) { 
            xDis = 2; //2 pixel
        }

        var angle = Math.PI / 4 + Math.atan(yDis / xDis);
        
        //smoothing the angle
        if (Math.abs(angle - this.prevAngle) > this.maxAngleChange) {
            angle = (angle - this.prevAngle > 0) ? this.prevAngle + this.maxAngleChange : this.prevAngle - this.maxAngleChange;
            yDis = Math.tan(angle - Math.PI / 4) * xDis;
            curYEnd = this.prevYEnd + yDis;
        }

        var diagSquare = xDis * xDis + yDis * yDis;
        if (diagSquare < this.minDiaDistanceSquare) {
            return null;
        } else if (diagSquare > this.maxDiaDistanceSquare) { 
            //need to reduce the length
            var factor = this.maxDiaDistance / Math.sqrt(diagSquare);
            xDis = xDis * factor;
            yDis = yDis * factor;
            curXEnd = this.prevXEnd + xDis;
            curYEnd = this.prevYEnd + yDis;
            diagSquare = this.maxDiaDistanceSquare;
        }

        var width = Math.sqrt(diagSquare / 2);
        var height = width; //for leaf1, which is a square

        var x = xDis / 2 + this.prevXEnd;
        var y = yDis / 2 + this.prevYEnd;

        var newSquare = new AnimatedObject( {
            name : "leaf", 
            width : width, 
            height : height, 
            textureImg : TEXTURES["leaf1"],
            x : x,
            y : y,
            angle : angle,
            entranceDuration : ENTRANCE_DURATION
        });

        this.prevMade = Date.now();
        this.prevXEnd = curXEnd;
        this.prevYEnd = curYEnd;
        this.prevAngle = angle;
        return newSquare;
    }

    this.move = function (dx, dy) {
        this.prevXEnd += dx;
        this.prevYEnd += dy;
    }
}

function makeBackgroundObjFac() {
    this.minTimeDiff = 12.0 * 1000;
    this.heighOffset = 1.2;
    this.XRange = 50;
    
    this.prevMade = 0;
    this.prevXEnd = WIDTH / 2;
    this.prevYEnd = HEIGHT / 2;
    this.delayCnt = 1;

    this.make = function () {
        if (Date.now() - this.prevMade < this.minTimeDiff || !CURRENT_NOTE) {
            return null;
        }
        
        var curPitch = melodayStore.getMovingAvg();
        
        var curYEnd = ((curPitch - MIN_NOTE) / NOTE_SPAN) * HEIGHT * this.heighOffset;
        
        // Random integer centered at WIDTH * 0.75
        var curXEnd = WIDTH * 0.75 + (Math.random() * this.XRange);

        if (isNaN(curYEnd)) {
            curYEnd = this.prevYEnd;
        }
        
        if (this.delayCnt-- > 0) {
            //enforce initial delay
            this.prevMade = Date.now();
            this.prevXEnd = curXEnd;
            this.prevYEnd = curYEnd;
            return null;
        }
        
        var newTree = new deterministicTree({
            w : WIDTH,
            h : curYEnd,
            x : curXEnd,
            y : HEIGHT,
        });
        
        this.prevMade = Date.now();
        this.prevXEnd = curXEnd;
        this.prevYEnd = curYEnd;
        this.minTimeDiff = -(Math.random() * 4000) + 16000;
        return newTree;
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
    this.entranceDuration = params.entranceDuration;
    this.creationTime = Date.now();
    
    this.x = params.x;
    this.y = params.y;
    this.angle = params.angle; //in radia
    
    //in the future implement velocity element
    this.move = function (dx, dy) {
        this.x += dx;
        this.y += dy;
    }

    this.render = function (ctx) {
        if (this.x < -120) {
            return;
        }

        ctx.save();

        ctx.translate(this.x, this.y);
        ctx.rotate(this.angle);
        ctx.globalAlpha = Math.min((Date.now() - this.creationTime) / 1000 / this.entranceDuration, 1); // set fade in effect
        //ctx.fillRect(-this.width / 2, -this.height / 2, this.width, this.height); //use to debug
        ctx.drawImage(this.textureImg, -this.width / 2, -this.height / 2, this.width, this.height);
        ctx.restore();
        //ctx.rotate(-this.angle);
        //ctx.translate(-this.x, -this.y);
    }

}