

function handleFileSelect(evt) {
    evt.stopPropagation();
    evt.preventDefault();
    
    var file;
    if (typeof evt.dataTransfer !== 'undefined') {
        var files = evt.dataTransfer.files; // FileList object.
        // files is a FileList of File objects. List some properties.
        $("#fileSelet").html("");
        file = files[0];
    } else { 
        var files = evt.target.files; // FileList object.
        // files is a FileList of File objects. List some properties.
        file = files[0];
    }
    
    $("#drop_zone").removeClass("drop_on");
    $("#drop_zone").html("Uploading: " + file.name);
    acceptUploadedFile(file, function () { 
        $("#drop_zone").html(file.name);
    });
}

function handleDragOver(evt) {
    evt.stopPropagation();
    evt.preventDefault();
    evt.dataTransfer.dropEffect = 'copy'; // Explicitly show this is a copy.
}

// Setup the dnd listeners.
var dropZone = document.getElementById('drop_zone');
dropZone.addEventListener('dragover', handleDragOver, false);
dropZone.ondragenter = function () {
    this.classList.add("drop_on");
    return false;
};
dropZone.ondragleave = function () {
    this.classList.remove("drop_on");
    return false;
};

dropZone.addEventListener('drop', handleFileSelect, false);
//document.getElementById('fileSelet').addEventListener('change', handleFileSelect, false);