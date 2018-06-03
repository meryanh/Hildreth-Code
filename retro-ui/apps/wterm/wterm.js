function wterm() {
    return{
        bound:false,
        locked:false,
        registered:new Map(),
        readCallback:null,
        mode:0,
        history:[],
        historyIndex:-1,
        fontColor:'FFF',
        command:'',
        prefix:'> ',
        window:document.createElement('div'),
        log:document.createElement('span'),
        input:document.createElement('span'),
            
        bind:function(parent){
            if (this.bound)
                return;
            this.window.className = 'wterm-window';
            this.log.className = 'wterm-log';
            this.window.appendChild(this.log);
            this.input.setAttribute('tabindex', "0");
            this.input.className = 'wterm-input';
            this.window.appendChild(this.input);
            this.input.addEventListener('keydown',
                function(e){
                    if (this.locked)
                        return;
                    if(e.keyCode == 13){
                        var text = this.input.innerText.trim();
                        this.writeLine(text);
                        this.input.innerText = '';
                        if (this.mode == 0){
                            if (text.length > 0)
                            {
                                this.history.unshift(text);
                                if (this.history.length > 50)
                                    this.history.pop();
                            }
                        this.historyIndex = -1;
                            this.call(text);
                        }
                         else{
                             if (text.length == 0)
                                return;
                            this.mode = 0;
                            try { this.readCallback(text); }
                            catch(e) { }
                        }
                        if (this.mode == 0)
                            this.write(this.prefix);
                    }
                    else if(e.keyCode == 8){
                        this.input.innerText = this.input.innerText.slice(0, -1);
                    }
                    else if (this.mode == 0 && e.keyCode == 38){
                        if (this.historyIndex < this.history.length-1)
                            this.historyIndex++;
                        if (this.historyIndex != -1)
                            this.input.innerText = this.history[this.historyIndex];
                    }
                    else if (this.mode == 0 && e.keyCode == 40){
                        if (this.historyIndex > -1)
                            this.historyIndex--;
                        if (this.historyIndex != -1)
                            this.input.innerText = this.history[this.historyIndex];
                        else
                            this.input.innerText = '';
                    }
                    else if (e.keyCode == 27){
                        this.input.innerText = '';
                        this.historyIndex = -1;
                    }
                    else if (e.key.length == 1)
                    {
                        this.input.innerText += e.key;
                    }
                    this.window.scrollTop = this.window.scrollHeight;
                }.bind(this));
            this.window.addEventListener('click',
                function(){
                    this.input.focus();
                }.bind(this));
            this.write(this.prefix);
            if (parent == null || typeof(parent) == 'undefined')
                document.body.appendChild(this.window);
            else
                parent.appendChild(this.window);
            return this;
        },
        register:function(name, fn){
            if (typeof(fn) != 'function')
                throw 'Expected function, found ' + typeof(fn);
            if (typeof(name) != 'string')
                throw 'Name parameter is required';
            var terminal = this;
            this.registered[name] = eval('('+fn+')');
        },
        clear:function(){
            this.log.innerHTML = '';
        },
        setColor:function(input){
            console.log(this.input);
            if (input.length > 1 && input[1].length == 6 && /[0-9a-fA-F]{6}/.test(input[1]))
            {
                this.fontColor = '#'+input[1].substr(0,3);
                this.input.style.color = this.fontColor;
                this.window.style.backgroundColor = '#'+input[1].substr(3,6);
            }
            else if (input.length > 1 && input[1].length == 3 && /[0-9a-fA-F]{3}/.test(input[1]))
            {
                this.fontColor = '#'+input[1].substr(0,3);
                this.input.style.color = this.fontColor;
            }
        },
        write:function(input){
            if (typeof(input) != 'undefined'){
                var n = document.createElement('span');
                n.innerHTML = input.toString()
                    .replace(/\/\/[cC]\[([0-9a-fA-F]{3})([0-9a-fA-F]{3})\](.+?)\/\//g, '<span class="wterm-color" style="color:#$1;background-color:#$2;">$3</span>')
                    .replace(/\/\/[cC]\[([0-9a-fA-F]{3})\](.+?)\/\//g, '<span class="wterm-color" style="color:#$1;">$2</span>');
                n.style.color = this.fontColor;
                this.log.appendChild(n);
                this.window.scrollTop = this.window.scrollHeight;
            }
        },
        writeLine:function(input){
            this.write(input+'<br/>');
        },
        lock:function(input){
            this.locked = true;
        },
        unlock:function(input){
            this.locked = false;
        },
        read:function(fn){
            if (typeof(fn) != 'function')
                throw 'Expected function, found ' + typeof(fn);
            this.mode = 1;
            this.readCallback = fn;
        },
        call:function(cmd){
            if (cmd == null || typeof(cmd) == 'undefined' || cmd.length == 0)
                return;
            cmd = cmd.replace(/&nbsp;/g, ' ');
            this.command = cmd;
            var params = cmd.match(/"[^"]*"|\S+/g) || [];
            for (var i = 0, len = params.length; i < len; i++){
                if (params[i][0] == '"' && params[i][params[i].length-1] == '"')
                    params[i] = params[i].substring(1, params[i].length-1);
            }
            try{
                if (this.registered.hasOwnProperty(params[0]) && typeof(this.registered[params[0]]) == 'function')
                    this.registered[params[0]](params);
                else
                    this.writeLine('Error: //C[F00]'+params[0]+'// is not recognized as an executable command.');
            }
            catch(e){}
        }
    };
}

(function(){
    var s = document.createElement('style');
    s.innerHTML = '\
body{user-select:none;-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;}\
.wterm-window{height:100%;width:100%;padding:0;margin:0;background-color:#000;color:#FFF;font-family:monospace;overflow-x:hidden;overflow-y:auto;white-space:pre-wrap;user-select:none;cursor:default;word-break:break-all;overflow:hidden;line-height:16px;}\
.wterm-log{display:inline;padding:0;padding-bottom:0;margin:0;}\
.wterm-input{display:inline;outline:none!important;}\
.wterm-color{border-radius:3px;}\
.wterm-input:focus:after{display:inline;content:"▌";animation: blink linear 0.75s infinite;}\
@keyframes blink{0%{opacity:1;}15%{opacity:0;}50%{opacity:0;}65%{opacity:1;}}';
    document.head.appendChild(s);
})();