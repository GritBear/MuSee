function melodayStoreEngine() {
    this.melodySize = 100;
    this.curInd = 0;
    this.movingAvgWindowSize = 3;
    this.melodies = new Array(this.melodySize);
    this.movingAvgs = new Array(this.melodySize);
    
    this.addNote = function (note) {
        this.melodies[this.curInd] = note;
        var temp = 0;

        for (i = 0; i < this.movingAvgWindowSize; i++) {
            var ind = (this.curInd - i + this.melodySize) % this.melodySize;
            temp += this.melodies[ind];
        }

        this.movingAvgs[this.curInd] = temp / this.movingAvgWindowSize;
        this.curInd = (this.curInd + 1) % this.melodySize;
    }

    this.getNote = function () { 
        return this.melodies[this.curInd];
    }

    this.getMovingAvg = function () { 
        return this.movingAvgs[this.curInd];
    }
}