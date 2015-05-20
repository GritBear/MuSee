define(['jquery'], function ($) {
    function modelBase(title) {
        this.title = title;

        this.BUFFERS = new Array(2);
        this.TEXTURES = {};

        this.visualModel = null;
        this.audioModel = null;
    }

    modelBase.prototype = {
        getTitle: function () {
            return this.title;
        },


        init: function () {
            this.BUFFERS[0] = $("#buffer1");
            this.BUFFERS[1] = $("#buffer2");
            
            //$(".texture").each(function (i, obj) {
            //    var alt = $(obj).attr("alt");
            //    TEXTURES[alt] = $(obj);
            //});
            
            this.TEXTURES["leaf1"] = document.getElementById("leaf1");
            this.TEXTURES["flower1"] = document.getElementById("flower1");
        },

        addVisualModel: function(visualModel){ 
            this.visualModel = visualModel;
            this.visualModel.BUFFERS = this.BUFFERS;
            this.visualModel.TEXTURES = this.TEXTURES;
            
            this.visualModel.init();
        },

        addAudioModel: function (audioModel) {
            this.audioModel = audioModel;
            
            this.audioModel.init();
        }

    };

    return modelBase;
});
