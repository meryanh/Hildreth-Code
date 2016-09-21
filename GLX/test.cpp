#include "GLX/GLX.h"
#include <string>
#ifdef WINDOWS
    #include "Windows.h"
#else
    #include <dirent.h>
#endif

/* Control Proto-Function Definitions */
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

function(write_mouseloc)
{
    control->set("text", SSTR("x= " << mouseloc_x << ", y= " << mouseloc_y));
}

// Encrypt/Decrypt strings using a key
void encryptDecrypt(string &toEncrypt, string key) 
{
    // Get size of string and code key
    int stringLength = toEncrypt.size();
    int keyLength    = key.size();
    int r = 1;
    
    for (int i = 0; i < keyLength; i++)
        key[i] += 3*i;
    
    // Perform encryption/decryption for this string
    for (int i = 0, j = 0; i < stringLength; i++)
    {
        toEncrypt[i] ^= (++key[j] + i);
        if (j >= keyLength || j < 0)
            r = -r;
        j += r;
    }
}

void cryptMe()
{
    string value = (string)control->get("text");
    encryptDecrypt(value, "here's a key...");
    control->set("text", value);
}

void addText()
{
    string str = (string)control->get("text");
    int length = str.length();
    
    if ((char)thekey == 8)
    {
        if (length > 0 && cursor_loc > 0)
        {
            str.erase(str.begin()+cursor_loc - 1);
            cursor_loc--;
            if (cursor_loc < 0)
                cursor_loc = 0;
        }
    }
    else if ((char)thekey == 127)
    {
        if (length > 0 && cursor_loc < str.length())
        {
            str.erase(str.begin()+cursor_loc);
        }
    }
    else
    {
        str.insert(cursor_loc, 1, thekey);
        cursor_loc++;
    }
    control->set("text", str);
}

void moveBlock()
{
    if (control->rect[0] < mouseloc_x)
    {
        control->move(2, 0);
    }
    else
    {
        control->move(-2, 0);
    }
    
    if (control->rect[1] < mouseloc_y)
    {
        control->move(0, 2);
    }
    else
    {
        control->move(0, -2);
    }
}

/* Returns a list of files in a directory (except the ones that begin with a dot) */

void GetFilesInDirectory(std::vector<string> &out, const string &directory)
{
#ifdef _WIN32
    HANDLE dir;
    WIN32_FIND_DATA file_data;

    if ((dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
        return; /* No files found */

    do {
        string file_name = file_data.cFileName;
        string full_file_name = directory + "/" + file_name;
        bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
        
        if (file_name[0] == '.')
            continue;

        if (is_directory)
            full_file_name = ">" + full_file_name;
        else
            full_file_name = "<" + full_file_name;

        out.push_back(full_file_name);
    } while (FindNextFile(dir, &file_data));

    FindClose(dir);
#else
    /*
    DIR *dir;
    class dirent *ent;
    class stat st;

    dir = opendir(directory);
    while ((ent = readdir(dir)) != NULL) {
        const string file_name = ent->d_name;
        const string full_file_name = directory + "/" + file_name;

        if (file_name[0] == '.')
            continue;

        if (stat(full_file_name.c_str(), &st) == -1)
            continue;

        const bool is_directory = (st.st_mode & S_IFDIR) != 0;

        if (is_directory)
            continue;

        out.push_back(full_file_name);
    }
    closedir(dir);*/
#endif
} // GetFilesInDirectory

function(move_cursor)
{
    string str = ((string)control->get("text"));
    int length = str.length();
    
    if (arrowkey & ARROW_LEFT && cursor_loc > 0)
        cursor_loc--;
    else if (arrowkey & ARROW_RIGHT && cursor_loc < length)
        cursor_loc++;
    else if (arrowkey & ARROW_UP && cursor_loc > 0)
    {
        int found = 0;
        cursor_loc--;
        for (; found < 2 && cursor_loc > 0 && (str[cursor_loc] != '\n' && str[cursor_loc] != '\r'); cursor_loc--);
        if (cursor_loc > 0)
        {
            cursor_loc--;
            for (; cursor_loc > 0 && (str[cursor_loc] != '\n' && str[cursor_loc] != '\r'); cursor_loc--);
        }
        if (cursor_loc > 0 && cursor_loc < length - 2)
            cursor_loc++;
    }
    else if (arrowkey & ARROW_DOWN && cursor_loc < length)
    {
        if (cursor_loc < length && (str[cursor_loc] == '\n' || str[cursor_loc] == '\r'))
        {
            cursor_loc++;
        }
        else
        {
            cursor_loc++;
            for (; cursor_loc < length && (str[cursor_loc] != '\n' && str[cursor_loc] != '\r'); cursor_loc++);
            
            if (cursor_loc < length)
                cursor_loc++;
        }
    }
}

void draw_shape()
{
    glBegin(GL_LINE_LOOP);
        glVertex2i(200, 200);
        glVertex2i(205, 205);
        glVertex2i(200, 210);
        glVertex2i(195, 205);
    glEnd();
}

void draw_char()
{
	int size = 5;
	int weight = 1;
	int x = 200;
	int y = 300;
	vector<vector<char>> c = {{ 2, 4, 1, 3, 1, 2, 2, 1, 3, 1, 4, 2, 4, 3, 3, 4, 2, 4, 1, 5, 1, 6, 2, 7, 3, 7, 4, 6, 4, 5, 3, 4 }};
	char x_last = 255;
	char y_last = 255;
	
	glLineWidth(weight);
	glPointSize(weight);
	
	int parts = c.size();
	for (int i = 0; i < parts; i++)
	{
		int count = c[i].size();
		if (count == 2)
		{
			glBegin(GL_POINTS);
			glVertex2i(x + c[i][0] * size, y + c[i][1] * size);
			glEnd();
		}
		else
		{
			glBegin(GL_LINE_STRIP);
			for (int j = 0; j < count; j+=2)
			{
				
				if (j > 0)
				{
					glVertex2i(x + x_last * size, y + y_last * size);
				}
				glVertex2i(x + c[i][j] * size, y + c[i][j+1] * size);
				//glBegin(GL_POINTS);
				//glVertex2i(x + c[i][j] * size, y + c[i][j+1] * size);
				//glEnd();
				x_last = c[i][j];
				y_last = c[i][j+1];
			}
			glEnd();
		}
	}
	
	glLineWidth(1);
	glPointSize(1);
}

/* Run at startup */
void startup()
{
    // Initialize window:
    X.window                    (640, 480, "Test UI Window");
    X.window.color              (0,0,0);//(25, 25, 28);
    X.window.center();
    
    vector<string> files;
    string directory = "C:\\Users\\Ryan\\Desktop\\GLX";
    GetFilesInDirectory(files, directory);
    
    string file_content = "";
    string tmp;
    for (int i = 0; i < files.size(); i++)
    {
        tmp = files.at(i);
        for (int j = tmp.length()-1; j > 1; j--)
            if (tmp[j] == '\\' || tmp[j] == '/')
            {
                if (j < tmp.length()-1)
                    j++;
                tmp = tmp.substr(j);
                j = tmp.length();
            }
        if (i > 0)
            file_content += "\n";
        file_content += files.at(i)[0] + tmp;
    }

    // Initialize Controls:
    X["text"];
    X->display                  (text_field_editable);
    X->arrowkeydown             (move_cursor);
    X->arrowkeyup               (move_cursor);
    X->keydown                  (addText);
    //X->mouseup                  (cryptMe);
    X->color["background"]      (50, 50, 50);
    X->color["text"]            (255, 0, 0);
    X->rect                     (0, 0, 640, 480);
    X->set                      ("text", (string)"The quick brown fox jumps over the lazy dog.");

	// Basic UI Controls:
    (!X)["title_bar"];
    X->display                  (opaque_text_field);
    X->activemousemove          (move_window);
    X->color["background"]      (50, 50, 50);
    X->color["text"]            (200, 200, 200);
    X->rect                     (0, -titlebar_height, X.window.width(), 0);
    X->set                      ("text", (X.window.title()));
    
    (!X)["minimize"];
    X->display                  (empty_button);
    X->mouseup                  (iconify);
    X->color["background"]      (150, 150, 150);
    X->color["hilite"]          (0, 0, 0);
    X->rect                     (X.window.width()-titlebar_height*2-2, -titlebar_height+1, X.window.width()-2-titlebar_height, -1);

    (!X)["quit"];
    X->display                  (empty_button);
    X->mouseup                  (quit);
    X->color["background"]      (255, 0, 0);
    X->color["hilite"]          (0, 0, 0);
    X->rect                     (X.window.width()-titlebar_height-1, -titlebar_height+1, X.window.width()-1, -1);
}