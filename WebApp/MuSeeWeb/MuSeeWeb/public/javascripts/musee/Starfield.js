//=====================
var numStars = 1000;
var radius = 1;
var focalLength;
var centerX, centerY;

var stars = [], star;
var i;

function initializeStars() {
    centerX = WIDTH / 2;
    centerY = HEIGHT / 2;
    focalLength = WIDTH;
    stars = [];
    for (i = 0; i < numStars; i++) {
        star = {
            x: Math.random() * WIDTH,
            y: Math.random() * HEIGHT,
            z: Math.random() * WIDTH
        };
        stars.push(star);
    }
}

function moveStars() {
    for (i = 0; i < numStars; i++) {
        star = stars[i];
        star.z--;
        
        if (star.z <= 0) {
            star.z = WIDTH;
        }
    }
}

function drawStars() {
    var pixelX, pixelY, pixelRadius;
    var c = CUR_BACK_BUFFER.getContext("2d");

    c.fillStyle = "black";
    c.fillRect(0, 0, WIDTH, HEIGHT);
    c.fillStyle = "white";
    for (i = 0; i < numStars; i++) {
        star = stars[i];
        
        pixelX = (star.x - centerX) * (focalLength / star.z);
        pixelX += centerX;
        pixelY = (star.y - centerY) * (focalLength / star.z);
        pixelY += centerY;
        pixelRadius = radius * (focalLength / star.z);
        
        c.beginPath();
        c.arc(pixelX, pixelY, pixelRadius, 0, 2 * Math.PI);
        c.fill();
    }

    flipDoubleBuffer();
}