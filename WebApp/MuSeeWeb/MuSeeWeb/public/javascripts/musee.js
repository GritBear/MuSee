requirejs.config({
    //By default load any module IDs from js/musee
    baseUrl: 'js/musee',
    
    //except, if the module ID starts with "lib",
    //load it from the js/app directory. paths
    //config is relative to the baseUrl, and
    //never includes a ".js" extension since
    //the paths config could be for a directory.
    paths: {
        app: '../lib'
    }
});