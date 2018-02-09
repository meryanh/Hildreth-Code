// This file is required by the index.html file and will
// be executed in the renderer process for that window.
// All of the Node.js APIs are available in this process.
const remote = require('electron').remote;

document.getElementById("min-btn").addEventListener("click", function (e) {
   var window = remote.getCurrentWindow();
   window.minimize(); 
});

document.getElementById("max-btn").addEventListener("click", function (e) {
   var window = remote.getCurrentWindow();
   if (!window.isMaximized()) {
       window.maximize();          
   } else {
       window.unmaximize();
   }
});

document.getElementById("close-btn").addEventListener("click", function (e) {
   var window = remote.getCurrentWindow();
   window.close();
}); 