var Boid = function () {
    
    var vector = new THREE.Vector3(),
        _acceleration, _width = 500, _height = 500, _depth = 200, _goal, _neighborhoodRadius = 100,
        _maxSpeed = 4, _maxSteerForce = 0.1, _avoidWalls = false;
    
    this.position = new THREE.Vector3();
    this.velocity = new THREE.Vector3();
    _acceleration = new THREE.Vector3();
    
    this.setGoal = function (target) {
        
        _goal = target;

    }
    
    this.setAvoidWalls = function (value) {
        
        _avoidWalls = value;

    }
    
    this.setWorldSize = function (width, height, depth) {
        
        _width = width;
        _height = height;
        _depth = depth;

    }
    
    this.run = function (boids) {
        
        if (_avoidWalls) {
            
            vector.set(0, this.position.y, this.position.z);
            vector = this.avoid(vector);
            vector.multiplyScalar(5);
            _acceleration.add(vector);
            
            vector.set(_width, this.position.y, this.position.z);
            vector = this.avoid(vector);
            vector.multiplyScalar(5);
            _acceleration.add(vector);
            
            vector.set(this.position.x, 0, this.position.z);
            vector = this.avoid(vector);
            vector.multiplyScalar(5);
            _acceleration.add(vector);
            
            vector.set(this.position.x, _height, this.position.z);
            vector = this.avoid(vector);
            vector.multiplyScalar(5);
            _acceleration.add(vector);
            
            vector.set(this.position.x, this.position.y, -_depth);
            vector = this.avoid(vector);
            vector.multiplyScalar(5);
            _acceleration.add(vector);
            
            vector.set(this.position.x, this.position.y, _depth);
            vector = this.avoid(vector);
            vector.multiplyScalar(5);
            _acceleration.add(vector);

        }/* else {

						this.checkBounds();

					}
					*/

        if (Math.random() > 0.5) {
            
            this.flock(boids);

        }
        
        this.move();

    }
    
    this.flock = function (boids) {
        
        if (_goal) {
            
            _acceleration.add(this.reach(_goal, 0.005));

        }
        
        _acceleration.add(this.alignment(boids));
        _acceleration.add(this.cohesion(boids));
        _acceleration.add(this.separation(boids));

    }
    
    this.move = function () {
        
        this.velocity.add(_acceleration);
        
        var l = this.velocity.length();
        
        if (l > _maxSpeed) {
            
            this.velocity.divideScalar(l / _maxSpeed);

        }
        
        this.position.add(this.velocity);
        _acceleration.set(0, 0, 0);

    }
    
    this.checkBounds = function () {
        
        if (this.position.x > _width) this.position.x = -_width;
        if (this.position.x < -_width) this.position.x = _width;
        if (this.position.y > _height) this.position.y = -_height;
        if (this.position.y < -_height) this.position.y = _height;
        if (this.position.z > _depth) this.position.z = -_depth;
        if (this.position.z < -_depth) this.position.z = _depth;

    }
    
    //
    
    this.avoid = function (target) {
        
        var steer = new THREE.Vector3();
        
        steer.copy(this.position);
        steer.sub(target);
        
        steer.multiplyScalar(1 / this.position.distanceToSquared(target));
        
        return steer;

    }
    
    this.repulse = function (target) {
        
        var distance = this.position.distanceTo(target);
        
        if (distance < 150) {
            
            var steer = new THREE.Vector3();
            
            steer.subVectors(this.position, target);
            steer.multiplyScalar(2 / distance); //2 is repulsion factor
            
            _acceleration.add(steer);

        }

    }
    
    this.reach = function (target, amount) {
        
        var steer = new THREE.Vector3();
        
        steer.subVectors(target, this.position);
        steer.multiplyScalar(amount);
        
        return steer;

    }
    
    this.alignment = function (boids) {
        
        var boid, velSum = new THREE.Vector3(),
            count = 0;
        
        for (var i = 0, il = boids.length; i < il; i++) {
            
            if (Math.random() > 0.6) continue;
            
            boid = boids[ i ];
            
            distance = boid.position.distanceTo(this.position);
            
            if (distance > 0 && distance <= _neighborhoodRadius) {
                
                velSum.add(boid.velocity);
                count++;

            }

        }
        
        if (count > 0) {
            
            velSum.divideScalar(count);
            
            var l = velSum.length();
            
            if (l > _maxSteerForce) {
                
                velSum.divideScalar(l / _maxSteerForce);

            }

        }
        
        return velSum;

    }
    
    this.cohesion = function (boids) {
        
        var boid, distance,
            posSum = new THREE.Vector3(),
            steer = new THREE.Vector3(),
            count = 0;
        
        for (var i = 0, il = boids.length; i < il; i++) {
            
            if (Math.random() > 0.6) continue;
            
            boid = boids[ i ];
            distance = boid.position.distanceTo(this.position);
            
            if (distance > 0 && distance <= _neighborhoodRadius) {
                
                posSum.add(boid.position);
                count++;

            }

        }
        
        if (count > 0) {
            
            posSum.divideScalar(count);

        }
        
        steer.subVectors(posSum, this.position);
        
        var l = steer.length();
        
        if (l > _maxSteerForce) {
            
            steer.divideScalar(l / _maxSteerForce);

        }
        
        return steer;

    }
    
    this.separation = function (boids) {
        
        var boid, distance,
            posSum = new THREE.Vector3(),
            repulse = new THREE.Vector3();
        
        for (var i = 0, il = boids.length; i < il; i++) {
            
            if (Math.random() > 0.6) continue;
            
            boid = boids[ i ];
            distance = boid.position.distanceTo(this.position);
            
            if (distance > 0 && distance <= _neighborhoodRadius) {
                
                repulse.subVectors(this.position, boid.position);
                repulse.normalize();
                repulse.divideScalar(distance);
                posSum.add(repulse);

            }

        }
        
        return posSum;

    }
}

function birdflock(params) {
    this.numBird = params.numBird;
    this.width = params.width;
    this.height = params.height;

    this.color = "#66D0FE";
    this.boid;
    this.bird;
    this.birds = [];
    this.boids = [];
    this.goal = new THREE.Vector3();
    this.goal.x = this.width / 2;
    this.goal.y = this.height / 2;
    
    for (var i = 0; i < this.numBird; i++) {
        
        this.boid = this.boids[ i ] = new Boid();
        this.boid.position.x = Math.random() * 400 - 200;
        this.boid.position.y = Math.random() * 400 - 200;
        this.boid.position.z = Math.random() * 400 - 200;
        this.boid.velocity.x = Math.random() * 2 - 1;
        this.boid.velocity.y = Math.random() * 2 - 1;
        this.boid.velocity.z = Math.random() * 2 - 1;
        this.boid.setAvoidWalls(true);
        this.boid.setWorldSize(this.width, this.height, 200);
        this.boid.setGoal(this.goal);

        this.bird = this.birds[ i ] = new Bird({
            color: "#66D0FE",
            width: this.width,
            height:this.height
        });
    }
    
    this.updateGoal = function (goal) {
        if (!isNaN(goal.x) && !isNaN(goal.y) &&
            goal.x > 0 && goal.x < this.width && goal.y > 0 && goal.y < this.height) { 

            this.goal.x = goal.x;
            this.goal.y = goal.y;
        }
    }
    
    this.render = function (ctx) { 
        for (var i = 0, il = this.birds.length; i < il; i++) {
            
            this.boid = this.boids[ i ];
            this.boid.run(this.boids);
            
            this.bird = this.birds[ i ];
            this.bird.position = this.boids[ i ].position;
            
            //bird.rotation.y = Math.atan2(-boid.velocity.z, boid.velocity.x);
            //bird.rotation.z = Math.asin(boid.velocity.y / boid.velocity.length());

            this.bird.render(ctx);
        }
    
    }
}

function Bird(params) {
    this.color = params.color;
    this.width = params.width;
    this.width = params.height;

    this.render = function (ctx) { 
        //draw a small circle at position
        var x = this.position.x;
        var y = this.position.y;

        ctx.save();
        ctx.beginPath();
        ctx.fillStyle = this.color;
        // Draws a circle of radius 20 at the coordinates 100,100 on the canvas
        ctx.arc(x , y, 5, 0, Math.PI * 2, true);
        ctx.closePath();
        ctx.fill();
        ctx.restore();
    }
}