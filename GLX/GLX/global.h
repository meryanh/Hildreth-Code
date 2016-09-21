#ifndef GLOBAL_H
#define GLOBAL_H

class Color;
class Function;
class var;
class Control;
class XControl;

Control* active  = 0;   // Active control for idle/activemousemove messages
Control* control = 0;   // Currently executing control
Control* clicked = 0;   // Previously clicked control

// Screen data
int screenwidth() { return glutGet(GLUT_SCREEN_WIDTH); }
int screenheight() { return glutGet(GLUT_SCREEN_HEIGHT); }

// Window info
void resize_window_func();
const int titlebar_height = 12;
int clickloc_x = 0;
int clickloc_y = 0;

// Mouse info
bool mouse_lock = false;
int mousebutton = 0;
int mouseloc_x  = 0;
int mouseloc_y  = 0;
int mousediff_x = 0;
int mousediff_y = 0;

// Keyboard info
unsigned char thekey = '?';
unsigned char arrowkey = 0;
#define ARROW_UP 1
#define ARROW_DOWN 2
#define ARROW_LEFT 4
#define ARROW_RIGHT 8
int cursor_loc  = 0;

// Font info
void* font = GLUT_BITMAP_8_BY_13;
const int font_width  = 8;
const int font_height = 13;

// Function prototypes
void startup();
void mouse_func(int, int, int, int);
void display_func();
void idle_func(int);
void motion_func(int, int);
void passive_motion_func(int, int);
void keyboard_func(unsigned char, int, int);
void resize_window_func(int, int);
void entry_func(int);
void arrow_down_function(int key, int x, int y);
void arrow_up_function(int key, int x, int y);

#endif