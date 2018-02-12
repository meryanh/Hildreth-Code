(function(){
const remote = require('electron').remote;
document.getElementById("min-btn").addEventListener("click", function () {
   remote.getCurrentWindow().minimize(); 
});
document.getElementById("max-btn").addEventListener("click", function () {
   var window = remote.getCurrentWindow();
   if (!window.isMaximized()) {
       window.maximize();          
   } else {
       window.unmaximize();
   }
});
document.getElementById("close-btn").addEventListener("click", function () {
   var window = remote.getCurrentWindow().close();
});
var fs = require('fs');

// if (process.argv.length <= 2) {
    // console.log("Usage: " + __filename + " path/to/directory");
    // process.exit(-1);
// }
//window.fpath = process.argv[2];
window.fpath = __dirname.replace(/\\/g, '/');
window.path_up = function(){
    var oldpath = window.fpath;
    window.fpath = window.fpath.substr(0,window.fpath.lastIndexOf('/'));
    try {
        get_dir();
    } catch (ex){
        window.fpath = oldpath;
    }
};
window.path_down = function(path){
    var oldpath = window.fpath;
    window.fpath += '/' + path;
    try {
        get_dir();
    } catch (ex){
        window.fpath = oldpath;
    }};
window.get_dir = function(){
    var items = fs.readdirSync(fpath);
    window.fdata = [];
    for (var i=0; i<items.length; i++){
        var file = fpath + '/' + items[i];
        var stats = fs.statSync(file);
        window.fdata.push({
            name:file.substr(file.lastIndexOf('/')+1),
            isDirectory:stats.isDirectory(),
            createdDate:stats.birthtime,
            modifiedDate:stats.mtime,
            lastAccessDate:stats.atime,
            size:stats.size
        });
    }
};
var sys = require('sys');
var exec = require('child_process').exec;
function getCommandLine() {
   switch (process.platform) { 
      case 'darwin' : return 'open';
      case 'win32' : return 'start ""';
      case 'win64' : return 'start ""';
      default : return 'xdg-open';
   }
}
window.launch_file = function(fname){
    exec(getCommandLine() + ' "' + window.fpath + '/' + fname + '"');
};
})();