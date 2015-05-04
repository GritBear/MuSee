/*

    Algorithmic Tree - 1.0.0
    drawing trees algorithmically on the HTML5 canvas

    License       : GPL
    Developer     : Sameer Borate: http://codediesel.com, Lee Xiong
    Web Site      : http://codediesel.com

 *  This version is modified to be deterministic (the tree is generated once and persisted in the storage)
 */
function deterministicTree(params) {
    this.MAX_BRANCH_WIDTH = 15;
    this.SMALL_LEAVES = 10;
    this.MEDIUM_LEAVES = 200;
    this.BIG_LEAVES = 500;
    this.THIN_LEAVES = 900;
    this.drawLeaves = true;
    this.leavesColor = '';
    this.leaveType = this.THIN_LEAVES;
    
    if (params.spread >= 0.3 && params.spread <= 1) {
        this.spread = params.spread;
    } else {
        this.spread = 0.5;
    }
    
    if (params.leaves === true || params.leaves === false) {
        this.drawLeaves = params.leaves;
    } else {
        this.leaves = true;
    }
    
    if (params.leaveType == this.SMALL_LEAVES || 
           params.leaveType == this.MEDIUM_LEAVES || 
           params.leaveType == this.BIG_LEAVES || 
           params.leaveType == this.THIN_LEAVES) {
        this.leaveType = leaveType;
    } else {
        this.leaveType = this.MEDIUM_LEAVES;
    }
    
    //set location
    this.height = params.h;
    this.width = params.w;
    this.root_x = params.x;
    this.root_y = params.y;
    this.branchHeight = this.height / 5;

    // Set the leaves to a random color and tree width
    this.leavesColor = '#' + (0x1000000 + (Math.random()) * 0xffffff).toString(16).substr(1, 6);
    this.lineWidth = Math.max(Math.min(5 + (Math.random() * this.MAX_BRANCH_WIDTH), this.branchHeight / 5), 2);
    this.stemColor = "#ED9E37";
    
    this.root = new treeStemElement({
        canvasHeight: this.height,
        stemColor : this.stemColor,
        spread : this.spread,
        drawLeaves : this.drawLeaves,
        leaveType : this.leaveType,
        leavesColor : this.leavesColor,
        THIN_LEAVES : this.THIN_LEAVES,
        branchHeight : this.branchHeight,
    }, 0, "root");
    
    this.root.createChildren();
    
    //in the future implement velocity element
    this.move = function (dx, dy) {
        this.root_x += dx;
        this.root_y += dy;
    }
    
    this.render = function (ctx) {
        if (this.root_x < -400) {
            return;
        }
        ctx.save();
        // Center the tree in the window, and set line width
        ctx.translate(this.root_x, this.root_y);
        ctx.lineWidth = this.lineWidth;
        ctx.lineJoin = 'round';
        this.root.render(ctx);
        ctx.restore();
    }
}

function treeStemElement(params, depth, name) {
    this.name = name;

    this.params = params;
    this.canvasHeight = params.canvasHeight;
    this.stemColor = params.stemColor;
    this.angle = -(Math.random() * 0.2) + 0.1;
    this.isSpread = ((Math.random() * 1) < params.spread);
    this.drawLeaves = params.drawLeaves;
    this.depth = depth;
    this.branchHeight = params.branchHeight;
    
    this.entranceDuration = 0.15;
    this.startRenderingTime = 0;
    this.fadein = true;

    this.leftBranch = null;
    this.rightBranch = null;
    this.nextBranch = null;
    
    this.createChildren = function () { 
        if (this.isSpread) {
            if (this.depth < 5) {
                this.leftBranch = new treeStemElement(this.params, this.depth + 1, "left");
                this.rightBranch = new treeStemElement(this.params, this.depth + 1, "right");

                this.rightBranch.createChildren();
                this.leftBranch.createChildren();

            } else if (this.drawLeaves) {
                this.leftBranch = new treeLeafElement(this.params);
                this.rightBranch = new treeLeafElement(this.params);
            }
        } else {
            this.nextBranch = new treeStemElement(this.params, this.depth, "next");
            this.nextBranch.createChildren();
        }
    }
   
    this.render = function (ctx) {
        if (this.fadein) {
            if (this.startRenderingTime == 0) {
                this.startRenderingTime = Date.now();
            }
            var alpha = Math.min((Date.now() - this.startRenderingTime) / 1000 / this.entranceDuration + 0.5, 1);
            ctx.save();
            ctx.globalAlpha = alpha;
        
            ctx.beginPath();
            ctx.moveTo(0, 0);
            ctx.lineTo(0, -this.branchHeight);
            ctx.strokeStyle = this.stemColor;
            ctx.stroke();
            ctx.restore();
            this.fadein = alpha < 1;
            return;
        }

        ctx.beginPath();
        ctx.moveTo(0, 0);
        ctx.lineTo(0, -this.branchHeight);
        ctx.strokeStyle = this.stemColor;
        ctx.stroke();
        ctx.translate(0, -this.branchHeight);
        ctx.rotate(this.angle);

        if (this.isSpread) {
            // Draw the left branches
            if(this.leftBranch != null && this.rightBranch != null){
                ctx.rotate(-0.35);
                ctx.scale(0.7, 0.7);
                ctx.save();
                this.leftBranch.render(ctx);
                ctx.restore();
                // Draw the right branches
                ctx.rotate(0.6);
                ctx.save();
                this.rightBranch.render(ctx);
                ctx.restore();
            }
        } else { 
            this.nextBranch.render(ctx);
        }
    }
}

function treeLeafElement(params) {
    this.lengthFactor = 200;
    this.leaveType = params.leaveType;
    this.leavesColor = params.leavesColor;

    if (this.leaveType === params.THIN_LEAVES) {
        this.lengthFactor = 10;
    }
    
    this.render = function (ctx) { 
        ctx.fillStyle = this.leavesColor;
        ctx.fillRect(0, 0, this.leaveType * 0.3, this.lengthFactor * 0.3);
        ctx.stroke();
    }
}