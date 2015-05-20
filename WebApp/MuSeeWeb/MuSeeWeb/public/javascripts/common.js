//The build will inline common dependencies into this file.

//For any third party dependencies, like jQuery, place them in the lib folder.

//Configure loading modules from the lib directory,
//except for 'app' ones, which are in a sibling
//directory.
requirejs.config({
    baseUrl: 'javascripts/lib',
    paths: {
        app: '../app',
        "react": "react-with-addons.min",
        "JSXTransformer": "JSXTransformer",
        "jsx": "jsx"
    },

    shim : {
        "react": {
            "exports": "React"
        },
        "JSXTransformer": "JSXTransformer",

        "jquery" : {
        "exports" : "$"
        },
    },
    
    jsx: {
        fileExtension: ".jsx",
        transformOptions: {
            harmony: true,
            stripTypes: false,
            inlineSourceMap: true
        },
        usePragma: false
    }
});
