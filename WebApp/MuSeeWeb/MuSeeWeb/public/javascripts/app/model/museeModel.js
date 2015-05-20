define(['./Base', './VisualModel', './AudioModel'], function (Base, VisualModel, AudioModel) {
    var baseModel = new Base('MuSee Demo');
    var vmodel = new VisualModel();
    var amodel = new AudioModel();

    baseModel.init();
    baseModel.addVisualModel(vmodel);
    baseModel.addAudioModel(amodel);

    return baseModel;
});
