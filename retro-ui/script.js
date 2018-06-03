(function(){
    function makeID(){
        return 'X-'+('00000000'+(Math.random()*0x100000000>>>0).toString(16)).slice(-8)+
                '-'+('00000000'+(Math.random()*0x100000000>>>0).toString(16)).slice(-8)+
                '-'+('00000000'+(Math.random()*0x100000000>>>0).toString(16)).slice(-8)+
                '-'+('00000000'+(Math.random()*0x100000000>>>0).toString(16)).slice(-8);
    }
    var next_zIndex = 10;
    var setTop = function(el){
        if (!el.classList.contains('selected')){
            var l = document.querySelectorAll('.window.selected');
            for (var i = 0; i < l.length; i++){
                l[i].classList.remove('selected');
                document.getElementById('btn-'+l[i].id).classList.remove('selected');
            }
            el.style.zIndex = next_zIndex++;
        }
        document.getElementById('btn-'+el.id).classList.add('selected');
        el.classList.add('selected');
        el.style.display = 'block';
    }
    function strToElement(html){
        var template = document.createElement('template');
        template.innerHTML = html.trim().replace(/\${(.+?)}/g,function(a,b){return eval(b)});
        return template.content?template.content.firstChild:template.firstChild;
    }
    window.addEventListener('contextmenu', function(event){
        event.preventDefault();
    });
    var appData = null;
    function open(window_info, index, x, y, width, height, z){
        var t = document.getElementById(window_info.id);
        if (t != null){
            setTop(t);
            return;
        }
        var dragbackdrop = document.getElementById('drag-backdrop');
        var wnd_width = width?width:(window_info.width+8+'px');
        var wnd_height = height?height:(window_info.height+25+'px');
        var wnd_x = x?x:0;
        var wnd_y = y?y:0;
        var wnd_z = next_zIndex++;
        window_info.id = makeID();
        var wnd = strToElement(
    `<div wnd_index="${index}" id="${window_info.id}" class="window" style="display:none;top:${wnd_y};left:${wnd_x};width:${wnd_width};height:${wnd_height};z-index:${wnd_z}">
        <table class="header-bar" style="right:${window_info.resize?'57px':'40px'}">
            <tbody>
                <tr>
                    <td class="title-bar">${window_info.title}</td>
                    <td class="title-bar-background"></td>
                </tr>
            </tbody>
        </table>
        <div class="window-container"><iframe src="${window_info.url}"></iframe></div>
        <button class="max-btn" style="display:${window_info.resize?'inline-block':'none'}"></button>
        <button class="min-btn"></button>
        <button class="close-btn"></button>
        <div class="resize-btn"></div>
    </div>`);
        wnd.addEventListener('mousedown', function(event){setTop(this);});
        var frame = wnd.getElementsByTagName('iframe')[0];
        frame.name = window_info.id;
        frame.src = window_info.url;
        frame.addEventListener('load', function(){
            var title = wnd.getElementsByClassName('title-bar')[0];
            wnd.style.display = 'block';
            wnd.getElementsByClassName('max-btn')[0].addEventListener('click', function(){
                if (wnd.classList.contains('fullscreen'))
                    wnd.classList.remove('fullscreen');
                else
                    wnd.classList.add('fullscreen');
            });
            wnd.getElementsByClassName('min-btn')[0].addEventListener('click', function(){
                wnd.style.display = 'none';
                document.getElementById('btn-'+wnd.id).classList.remove('selected');
            });
            wnd.getElementsByClassName('close-btn')[0].addEventListener('click', function(){
                wnd.parentElement.removeChild(wnd);
                tbtn.parentElement.removeChild(tbtn);
            });
            wnd.getElementsByClassName('header-bar')[0].addEventListener('mousedown', function(event){
                dragStart(event, wnd);
            });
            wnd.getElementsByClassName('resize-btn')[0].addEventListener('mousedown', function(event){
                resizeStart(event, wnd)
            });
        });
        wnd.getElementsByClassName('resize-btn')[0].style.display = window_info.resize ? "block" : "none";
        var tbtn = strToElement(`<button id="${'btn-'+wnd.id}">${window_info.title}</button>`);
        tbtn.addEventListener('click', function(){
            wnd.style.display = 'block';
            setTop(wnd);
        });
        document.getElementById('toolbar').appendChild(tbtn);
        document.body.appendChild(wnd);
        setTop(wnd);
    }
    function resizeStart(ev, parent) {
        var p = parent;
        var sX = ev.clientX;
        var sY = ev.clientY;
        var sWi = parseInt(document.defaultView.getComputedStyle(p).width, 10);
        var sHe = parseInt(document.defaultView.getComputedStyle(p).height, 10);
        document.getElementById('drag-backdrop').style.display = 'block';
        var mousemove = function(ev){
            if (sWi + ev.clientX - sX > 100)
                p.style.width = (sWi + ev.clientX - sX) + 'px';
            if (sHe + ev.clientY - sY > 50)
                p.style.height = (sHe + ev.clientY - sY) + 'px';
        };
        var mouseup = function(ev){
            document.getElementById('drag-backdrop').style.display = 'none';
            window.removeEventListener('mousemove', mousemove, false);
            window.removeEventListener('mouseup', mouseup, false);
        };
        window.addEventListener('mousemove', mousemove);
        window.addEventListener('mouseup', mouseup, false);
    }
    function dragStart(ev, el){
        if (el.classList.contains('fullscreen'))
            return;
        var backdrop = document.getElementById('drag-backdrop');
        backdrop.style.display = 'block';
        ev = ev || window.event;
        var posX = ev.clientX,
            posY = ev.clientY,
        divTop = el.style.top,
        divLeft = el.style.left,
        eWi = parseInt(el.style.width),
        eHe = parseInt(el.style.height),
        cWi = parseInt(window.innerWidth),
        cHe = parseInt(window.innerHeight);
        divTop = divTop.replace('px','');
        divLeft = divLeft.replace('px','');
        var diffX = posX - divLeft,
            diffY = posY - divTop;
        var mousemove = function(ev){
            ev = ev || window.event;
            var posX = ev.clientX,
                posY = ev.clientY,
                aX = posX - diffX,
                aY = posY - diffY;
            if (aX + eWi > cWi) aX = cWi - eWi;
            // if (aY + eHe > cHe) aY = cHe -eHe;
            if (aY + 20 > cHe) aY = cHe - 20;
            if (aX < 0) aX = 0;
            if (aY < 24) aY = 24;
            el.style.left = aX + 'px';
            el.style.top = aY + 'px';
        };
        var mouseup = function(ev){
            window.removeEventListener('mousemove', mousemove);
            window.removeEventListener('mouseup', mouseup);
            backdrop.style.display = 'none';
        };
        window.addEventListener('mousemove', mousemove);
        window.addEventListener('mouseup', mouseup);
    }
    window.addEventListener('load', function(){
        var frame = document.getElementById('load-frame');
        frame.src = 'data.json';
        frame.onload = function(){
            var menu = document.getElementById('start-btn-dropdown');
            appData = JSON.parse(frame.contentDocument.body.innerText);
            var nextX = 0;
            var nextY = 24;
            for (var i = 0; i < appData.length; i++){
                var index = i;
                (function(a){
                    var index = i;
                    var item = document.createElement('button');
                    item.innerText = appData[i].name;
                    item.addEventListener('click', function(){
                        nextX+=24;
                        nextY+=24;
                        open(a, index, nextX+'px', nextY+'px');
                    });
                    menu.appendChild(item);
                })(appData[i]);
            }
            try {
                var w = localStorage.getItem("stored-windows");
                if (w)
                    w = JSON.parse(w);
                for (var i = 0; i < w.length; i++){
                    open(appData[w[i].index],w[i].index,w[i].left,w[i].top,w[i].width,w[i].height);
                }
            }catch(ex){}
            try{
                window.settings = JSON.parse(localStorage.getItem("stored-style-settings"));
                
                for (var property in window.settings) {
                    if (window.settings.hasOwnProperty(property)) {
                        document.body.style[property] = window.settings[property];
                    }
                }
            }catch(ex){}
        };
        var clock = document.getElementById('system-clock');
        var updateClock = function(){
            var date = new Date();
            clock.innerText = date.toLocaleTimeString();
            clock.title = date.toLocaleDateString();  
        };
        setInterval(updateClock, 1000);
        updateClock();
        setInterval(function(){
            if(document.activeElement.tagName == 'IFRAME'){
                if (document.getElementById(document.activeElement.name).classList.contains('window')){
                    setTop(document.getElementById(document.activeElement.name));
                }
            }
        }, 100);
        document.getElementById('peek-btn').addEventListener('click',function(){
            var w = document.getElementsByClassName('window');
            for (var i = 0; i < w.length; i++){
                w[i].style.display = 'none';
                document.getElementById('btn-'+w[i].id).classList.remove('selected');
            }
        });
    });
    window.setStyleSetting=function(name,value){
        window.hasStoredSettingChange = true;
        if (!window.settings)
            window.settings={};
        window.settings[name]=value;
        document.body.style[name] = value;
    };
    window.addEventListener('beforeunload', function(){
        var w = document.getElementsByClassName('window');
        var data = [];
        for (var i = 0; i < w.length; i++){
            data[i]={
                index:w[i].getAttribute('wnd_index'),
                height:w[i].style.height,
                width:w[i].style.width,
                top:w[i].style.top,
                left:w[i].style.left
            };
        }
        
        try {
            localStorage.setItem("stored-windows", JSON.stringify(data));
        }catch(ex){}
        try {
            if (window.hasStoredSettingChange)
                localStorage.setItem("stored-style-settings", JSON.stringify(window.settings));
        }catch(ex){}
    });
    window.utils = (function(){
        var self={};
        var scb=new Map();
        var shr=new Map();
        self.listen=function(n,b,w){
            if (!scb.has(n))
                scb.set(n,[]);
            scb.get(n).push({w:w,b:b});
            w['callback-'+n]=b;
        };
        self.post=function(n,v){
            var ts=scb.get(n);
            if (ts)
                for (var i=0;i<ts.length;i++){
                    ts[i].w['callback-'+n](v);
                }
        };
        self.get=function(n){
            return shr.get(n);
        };
        self.set=function(n,v){
            shr.set(n,v);
        };
        return self;
    })();
    window.HTMLAlert=function(v){
        var element = strToElement(`
        <div class="modal-popup">
            <div>${v}</div>
            <button>OK</button>
        </div>
        `);
        element.getElementsByTagName('button')[0].addEventListener('click',function(){
            document.body.removeChild(element);
        });
        document.body.appendChild(element);
    }
    window.HTMLConfirm=function(v,c){
        var element = strToElement(`
        <div class="modal-popup">
            <div>${v}</div>
            <button>Yes</button><button>No</button>
        </div>
        `);
        element.getElementsByTagName('button')[0].addEventListener('click',function(){
            c(true);
            document.body.removeChild(element);
        });
        element.getElementsByTagName('button')[1].addEventListener('click',function(){
            c(false);
            document.body.removeChild(element);
        });
        document.body.appendChild(element);
    }
    window.HTMLPrompt=function(v,c){
        var element = strToElement(`
        <div class="modal-popup">
            <div>${v}</div>
            <div><input type="text"></input></div>
            <button>OK</button>
        </div>
        `);
        element.getElementsByTagName('button')[0].addEventListener('click',function(){
            c(element.getElementsByTagName('input')[0].value);
            document.body.removeChild(element);
        });
        element.getElementsByTagName('input')[0].addEventListener('keypress',function(e){
            if (e.keyCode == 13) {
                    c(element.getElementsByTagName('input')[0].value);
                    document.body.removeChild(element);
                return false;
            }
        });
        document.body.appendChild(element);
    }
})();
