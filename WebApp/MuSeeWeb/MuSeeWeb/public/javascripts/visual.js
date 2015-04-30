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
    this.dispatcher = new dispatcher({maxSize : 50});
    this.aniFactory = new makeAnimationFac();
    this.time = Date.now();

    this.frameUpdate = function () {
        var newAni = this.aniFactory.make();
        var timeNow = Date.now();
        var timeDiff = (timeNow - this.time) / 1000;
        this.time = timeNow;
        if (newAni) {
            this.dispatcher.add(newAni);
        }
        
        //move elements
        var xSpeed = timeDiff * AVG_X_MOVE_SPEED;

        this.dispatcher.apply(function (obj) { 
            obj.move(xSpeed, 0);
        });
        
        this.aniFactory.move(xSpeed, 0);
        
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
    this.maxAngleChange = Math.PI / 4;

    this.prevMade = 0;
    this.prevXEnd = WIDTH / 2.2;
    this.prevYEnd = HEIGHT / 2;
    this.prevAngle = Math.PI / 4;
    this.delayCnt = 2;

    this.make = function () {
        if (Date.now() - this.prevMade < this.minTimeDiff || !CURRENT_NOTE) { 
            return null;
        }
        
        var curPitch = melodayStore.getMovingAvg();
        
        var curXEnd = WIDTH / 2.2;
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
        }

        var width = Math.sqrt(diagSquare / 2);
        var height = width; //for leaf1, which is a square

        var x = xDis / 2 + this.prevXEnd;
        var y = yDis / 2 + this.prevYEnd;
        
        console.log("make image");
        console.log(this.prevXEnd);
        console.log(this.prevYEnd);
        console.log(xDis);
        console.log(yDis);
        console.log(diagSquare);
        console.log(width);
        console.log(angle);

        var newSquare = new AnimatedObject({
            name : "leaf", 
            width : width, 
            height : height, 
            textureImg : TEXTURES["leaf1"],
            x : x,
            y : y,
            angle : angle
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
        ctx.save();
        ctx.translate(this.x, this.y);
        ctx.rotate(this.angle);
        //ctx.fillRect(-this.width / 2, -this.height / 2, this.width, this.height);
        ctx.drawImage(this.textureImg, -this.width / 2, -this.height / 2, this.width, this.height);
        ctx.restore();
        //ctx.rotate(-this.angle);
        //ctx.translate(-this.x, -this.y);
    }

}