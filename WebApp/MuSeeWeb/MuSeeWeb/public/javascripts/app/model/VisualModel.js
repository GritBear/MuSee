define(['jquery'], function ($) {
    function visualModel() {
        this.BUFFERS;
        this.CUR_BUFFER = null;
        this.CUR_BACK_BUFFER = null;
        this.WIDTH;
        this.HEIGHT;
        this.TEXTURES;

        this.init = function () { 
            this.CUR_BUFFER = this.BUFFERS[0][0];
            this.CUR_BACK_BUFFER = this.BUFFERS[1][0];
            this.BUFFERS[1].hide();
            this.WIDTH = $(this.CUR_BUFFER).width();
            this.HEIGHT = $(this.CUR_BUFFER).height();
        }
    }

    return visualModel;
});
