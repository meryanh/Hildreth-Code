(function(){
    var l=null;
    var t = 0;
    var d = null;
    window.addEventListener('load', function(){
        var s=document.createElement('style');
        s.innerHTML=".load-panel{display:none;opacity:0.1;position:absolute;top:0; bottom:0;left:0; right:0;background-color:rgba(80,80,80,0.5);-webkit-transition: opacity .25s ease-in-out;-moz-transition: opacity .25s ease-in-out;transition: opacity .25s ease-in-out;}.square-grid {position:absolute;top:0; bottom:0;left:0; right:0;width:81px;height:81px;margin:auto;-webkit-transform:rotate(45deg);transform:rotate(45deg);}.square-grid div {width:27px;height:27px;background-color:#333;box-shadow:0 0 2px #333;box-sizing:border-box;margin:0;float:left;-webkit-animation:scale-fade 4s infinite ease-in-out;animation:scale-fade 4s infinite ease-in-out;}.square-grid .square1 {-webkit-animation-delay:0s;animation-delay:0s;}.square-grid .square2,.square-grid .square4 {-webkit-animation-delay:0.2s;animation-delay:0.2s;}.square-grid .square3,.square-grid .square5,.square-grid .square7 {-webkit-animation-delay:0.4s;animation-delay:0.4s;}.square-grid .square6,.square-grid .square8 {-webkit-animation-delay:0.6s;animation-delay:0.6s;}.square-grid .square9 {-webkit-animation-delay:0.8s;animation-delay:0.8s;}@-webkit-keyframes scale-fade {0%, 70%, 100% {opacity:1;-webkit-transform:scale3D(1, 1, 1);transform:scale3D(1, 1, 1);} 35% {opacity:0.1;-webkit-transform:scale3D(0, 0, 1);transform:scale3D(0, 0, 1);}}@keyframes scale-fade {0%, 70%, 100% {opacity:1;-webkit-transform:scale3D(1, 1, 1);transform:scale3D(1, 1, 1);} 35% {opacity:0.1;-webkit-transform:scale3D(0, 0, 1);transform:scale3D(0, 0, 1);}}";
        document.head.appendChild(s);
        l=document.createElement('div');
        l.className='load-panel';
        l.innerHTML='<div class="square-grid"><div class="square1"></div><div class="square2"></div><div class="square3"></div><div class="square4"></div><div class="square5"></div><div class="square6"></div><div class="square7"></div><div class="square8"></div><div class="square9"></div></div>';
        document.body.appendChild(l);
    });
  window.loadStart=function(){
    t++;
    if (!d){
        d = setTimeout(function(){
          if (t > 0)
          {
            l.style.display = 'block';
            setTimeout(function(){
                l.style.opacity = 1;
            }, 0);
          }
        },100);
    }
  };
  window.loadEnd=function(){
    if ((--t) <= 0)
    {
        if (d)
        {
            clearTimeout(d);
            d = null;
        }
        l.style.opacity = 0.1;
        setTimeout(function(){
            l.style.display = null;
        }, 250);
        t=(t<0)?0:t;
    }
  };
})();