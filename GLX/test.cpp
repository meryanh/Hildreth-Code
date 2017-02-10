#include "GLX/GLX.h"
#include <string>
#ifdef WINDOWS
    #include <Windows.h>
#else
    #include <dirent.h>
#endif

void toClipboard(const std::string &str){
    HWND hwnd = GetDesktopWindow();
	OpenClipboard(hwnd);
	EmptyClipboard();
	HGLOBAL hg=GlobalAlloc(GMEM_MOVEABLE,str.size()+1);
	if (!hg){
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg),str.c_str(),str.size()+1);
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT,hg);
	CloseClipboard();
	GlobalFree(hg);
}

function(quit)
{
    if (mousebutton == 0) 
        exit(0);
}

function(move_window)
{
    X.window.move(mousediff_x, mousediff_y);
}

function(iconify)
{
    if (mousebutton == 0) 
        X.window.iconify();
}

/* Run at startup */
void startup()
{
    // Initialize window:
    X.window                    (640, 480, "Test UI Window");
    X.window.color              (25, 25, 28);
    X.window.center();
    
    toClipboard("SOME STRING...");

    BEGIN
        
        // A editable text field:
        ADD(text)
            SET(display, text_field)
            SET(color["background"], 50, 50, 50)
            SET(color["text"], 255, 255, 255)
            SET(rect, 0, 0, 640, 480)
            SET(attribute, "text", (string)"")
        END
        
        // Title bar controls:
        ADD(title_bar)
            SET(display, opaque_text_field)
            SET(activemousemove, move_window)
            SET(color["background"], 50, 50, 50)
            SET(color["text"], 200, 200, 200)
            SET(rect, 0, -titlebar_height, X.window.width(), 0)
            SET(attribute, "text", X.window.title())
            
            ADD(minimize)
                SET(display, empty_button)
                SET(mouseup, iconify)
                SET(color["background"], 150, 150, 150)
                SET(color["hilite"], 0, 0, 0)
                SET(rect, X.window.width()-titlebar_height*2-2, -titlebar_height+1, X.window.width()-2-titlebar_height, -1)
            END
            
            ADD(quit)
                SET(display, empty_button)
                SET(mouseup, quit)
                SET(color["background"], 255, 0, 0)
                SET(color["hilite"], 0, 0, 0)
                SET(rect, X.window.width()-titlebar_height-1, -titlebar_height+1, X.window.width()-1, -1)
            END
        END
    END
}