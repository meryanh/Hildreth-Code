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

var spawn = require('child_process').spawn;
window.proc = spawn(__dirname+'\\Sample.exe');
window.proc.stdout.on('data', function(data) {
    console.log(data.toString());
});
window.proc.stderr.on('data', function(data) {
    console.log(data.toString());
});
window.proc.on('close', function(code, signal) {
    console.log('process closed');
});