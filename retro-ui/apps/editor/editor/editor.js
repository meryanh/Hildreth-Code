var editorPath = document.getElementsByTagName("script");
editorPath = editorPath[editorPath.length-1].src.replace(/editor\.js.*/,'');
var editor = (function(){
	var self = {};
	self.attach = function(p,t){
		var editObject = {};
		var container = document.createElement('div')
		container.className = 'html-editor';
		container.innerHTML = `
<div class="toolbar">
	<div class="dropdown-container" title="Font" style="width:71px;">
	<select>
		<option value="Arial, Helvetica, sans-serif">Arial</option>
		<option value="'Comic Sans MS', cursive, sans-serif">Comic Sans</option>
		<option value="'Courier New', Courier, monospace">Courier New</option>
		<option value="Georgia, serif;">Georgia</option>
		<option value="Impact, Charcoal, sans-serif">Impact</option>
		<option value="'Lucida Sans Unicode', 'Lucida Grande', sans-serif">Lucida</option>
		<option value="'Lucida Console', Monaco, monospace">Lucida Console</option>
		<option value="'Palatino Linotype', 'Book Antiqua', Palatino, serif">Palatino</option>
		<option value="Tahoma, Geneva, sans-serif">Tahoma</option>
		<option value="'Times New Roman', Times, serif">Times New Roman</option>
		<option value="'Trebuchet MS', Helvetica, sans-serif">Trebuchet</option>
		<option value="Verdana, Geneva, sans-serif">Verdana</option>
	</select>
	</div>
	<div class="dropdown-container" title="Size" style="width:71px;">
	<select>
		<option value="1">10px</option>
		<option value="2">12px</option>
		<option value="3">16px</option>
		<option value="4">18px</option>
		<option value="5">22px</option>
		<option value="6">32px</option>
		<option value="7">48px</option>
	</select>
	</div>
</div>
<div class="frame-container"><iframe><html><body contenteditable="true"></body></html></iframe></div>
`;
		p.appendChild(container);
		var frame = container.getElementsByTagName('iframe')[0];
		var frameWindow = frame.contentWindow;
		setTimeout(function(){
			frameWindow.document.body.contentEditable="true"
			var style = document.createElement('style');
			style.innerHTML = 'body{font-size:12px;white-space:pre-wrap;word-wrap:break-word;font-family:sans-serif;}';
			frameWindow.document.head.appendChild(style);
			if (t) frameWindow.document.body.innerHTML = t;
			frameWindow.document.addEventListener('mouseup', function(){
				updateCommandState();
			});
			frameWindow.document.addEventListener('keyup', function(){
				updateCommandState();
			});
			frameWindow.document.addEventListener('keypress', function(e){
				if(e.keyCode == 9){
					frameWindow.document.execCommand('inserthtml', false, '&#009');
					e.preventDefault()   
				}
			});
		}, 100);
		var toolbar = container.getElementsByClassName('toolbar')[0];
		var sendCommand = function(cmd, param){
			frameWindow.document.execCommand(cmd, false, param);
			frameWindow.focus();
		};
		var commandBtnList = [];
		var updateCommandState = function(){
			for (var i = 0; i < commandBtnList.length; i++){
				if (frameWindow.document.queryCommandState(commandBtnList[i].cmd))
					commandBtnList[i].btn.classList.add('selected');
				else
					commandBtnList[i].btn.classList.remove('selected');
			}
		};
		var addCmdBtn = function(parent, cmd, title, img){
			var btn = document.createElement('button');
			btn.innerHTML = '<img src="'+img+'"></img>';
			btn.title = title;
			btn.addEventListener('click', function(){
				frameWindow.document.execCommand(cmd);
				updateCommandState();
				frameWindow.focus();
			});
			commandBtnList.push({btn:btn,cmd:cmd});
			parent.appendChild(btn);
		};
		addCmdBtn(toolbar, 'bold', 'Bold', editorPath + 'images/bold.png');
		addCmdBtn(toolbar, 'italic', 'Italic', editorPath + 'images/italic.png');
		addCmdBtn(toolbar, 'underline', 'Underline', editorPath + 'images/underline.png');
		addCmdBtn(toolbar, 'strikethrough', 'Strikethrough', editorPath + 'images/strikethrough.png');
		addCmdBtn(toolbar, 'superscript', 'Superscript', editorPath + 'images/superscript.png');
		addCmdBtn(toolbar, 'subscript', 'Subscript', editorPath + 'images/subscript.png');
		addCmdBtn(toolbar, 'removeformat', 'Clear Formatting', editorPath + 'images/removeformat.png');
		toolbar.appendChild(document.createElement('br'));
		addCmdBtn(toolbar, 'undo', 'Undo', editorPath + 'images/undo.png');
		addCmdBtn(toolbar, 'redo', 'Redo', editorPath + 'images/redo.png');
		addCmdBtn(toolbar, 'decreasefontsize', 'Decrease Font Size', editorPath + 'images/decreasefontsize.png');
		addCmdBtn(toolbar, 'increasefontsize', 'Increase Font Size', editorPath + 'images/increasefontsize.png');
		addCmdBtn(toolbar, 'insertunorderedlist', 'Bulleted List', editorPath + 'images/unorderedlist.png');
		addCmdBtn(toolbar, 'insertorderedlist', 'Numbered List', editorPath + 'images/orderedlist.png');
		addCmdBtn(toolbar, 'inserthorizontalrule', 'Insert Horizontal Rule', editorPath + 'images/horizontalrule.png');
		addCmdBtn(toolbar, 'indent', 'Indent', editorPath + 'images/indent.png');
		addCmdBtn(toolbar, 'outdent', 'Outdent', editorPath + 'images/outdent.png');
		addCmdBtn(toolbar, 'justifyleft', 'Justify Left', editorPath + 'images/justifyleft.png');
		addCmdBtn(toolbar, 'justifycenter', 'Justify Center', editorPath + 'images/justifycenter.png');
		addCmdBtn(toolbar, 'justifyright', 'Justify Right', editorPath + 'images/justifyright.png');
		addCmdBtn(toolbar, 'justifyfull', 'Justify Full', editorPath + 'images/justifyfull.png');
		
		container.getElementsByTagName('select')[0].addEventListener('change', function(event){
			sendCommand('fontname', this.value);
			this.value = null;
		});
		container.getElementsByTagName('select')[1].addEventListener('change', function(event){
			sendCommand('fontsize', this.value);
			this.value = null;
		});
		editObject.getHTML = function(){
			return frameWindow.document.body.innerHTML;
		}
		editObject.setHTML = function(t){
			frameWindow.document.body.innerHTML = t;
		}
		editObject.container = container;
		return editObject;
	};
	
	window.addEventListener('load', function(){
		if (document.getElementById('html-editor-style'))
			return;
		var s = document.createElement('style');
		s.type = 'text/css';
		s.innerHTML = `
.html-editor{height:100%;width:100%;font-family:sans-serif;overflow:visible;overflow:hidden;position:relative;}
.html-editor div.toolbar{display:block;overflow:visible;background-color:#CCC;padding:2px;font-size:0;height:52px;cursor:default!important;left:0px;}
.html-editor .frame-container{box-sizing:border-box;position:absolute;left:0px;right:0px;top:52px;bottom:0;}
.html-editor iframe{box-sizing:border-box;border:2px inset #BBB;width:100%;height:100%;background-color:#FFF;}
.html-editor .dropdown-container,
.html-editor div.toolbar button{box-sizing:border-box;position:relative;vertical-align:top;display:inline-block;background-color:transparent;border:2px solid transparent;height:21px;width:21px;padding:0;margin:2px;text-align:center;}
.html-editor div.toolbar button{line-height:0;}
.html-editor .dropdown-container:before{position:absolute;content:attr(title);color:#000;pointer-events:none;font-size:14px;left:4px;top:0px;}
.html-editor .dropdown-container:after{position:absolute;content:'⏷';color:#000;pointer-events:none;font-size:12px;right:3px;top:0px;}
.html-editor div.toolbar button.selected,
.html-editor .dropdown-container:hover,
.html-editor div.toolbar button:hover{background-color:#CCC;border:2px outset #BBB;}
.html-editor .dropdown-container:active,
.html-editor div.toolbar button:active{border:2px inset #BBB;}
.html-editor div.toolbar select{opacity:0;width:100%;border:0;}
`;
		window.document.head.appendChild(s);
	});
	return self;
})();