#ifndef XCONTROL_CPP
#define XCONTROL_CPP
#include "XControl.h"
#include "Windows.h"

#define GL_MULTISAMPLE_ARB 0x809D
// XControl class:
void XControl::start(int* argc, char** argv)
{
    glutInit(argc, argv);
    glutInitDisplayMode(0x0C92); // 0x0C92 (undecorated), 0x0092 (title bar), 0x0492 (captionless)
    startup();
    glutMouseFunc(mouse_func);
    //glutTimerFunc(idle_rate, idle_func, 0);
    glutMotionFunc(motion_func);
    glutKeyboardFunc(keyboard_func);
    glutPassiveMotionFunc(passive_motion_func);
    glutEntryFunc(entry_func);
    glutDisplayFunc(display_func);
    glutSpecialFunc(arrow_down_function);
    glutSpecialUpFunc(arrow_up_function);
    
    glDisable(GL_POINT_SMOOTH);
    glDisable(GLUT_MULTISAMPLE);
    glDisable(GL_DITHER);
    glDisable(GL_POINT_SMOOTH);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POLYGON_SMOOTH);
    glHint(GL_POINT_SMOOTH, GL_DONT_CARE);
    glHint(GL_LINE_SMOOTH, GL_DONT_CARE);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE);
    glDisable(GL_MULTISAMPLE_ARB) ;
    glutMainLoop();    
}

// Get a control by name
Control* XControl::get_control(string name)
{
    Control* _check;
    if (_toplevel)
    {
        for (int i = 0; i < toplevel.size(); i++)
        {
            _check = &toplevel.at(i);
            if (_check->name() == name)
                return _check;
        }
        toplevel.push_back(Control(name));
        return &toplevel.back();
    }
    else
    {
        for (int i = 0; i < controls.size(); i++)
        {
            _check = &controls.at(i);
            if (_check->name() == name)
                return _check;
        }
        controls.push_back(Control(name));
        return &controls.back();
    }
}

void XControl::register_idle()
{
    idle_controls.push_back(_last);
}

XControl& XControl::operator [](string i) 
{
    _last = get_control(i);
    _toplevel = false;
    return *this;
}
Control* XControl::operator ->() 
{ 
    if (_last)
        return _last;
    else if (!_toplevel)
        return &controls[0];
    else 
        return &toplevel[0];
}
XControl& XControl::operator !()
{
    _toplevel = true;
    return *this;
}
int XControl::operator ()(int* argc, char** argv)
{
    start(argc, argv);
}
void XControl::operator <<(string i){ _last->set("text", i); }

// Show/Hide a group by ID
void group_vis(int group, bool vis)
{
    Control* _check;
    for (int i = 0; i < X.controls.size(); i++)
    {
        _check = &X.controls.at(i);
        if (_check->group == group)
        {
            if (_check->visible)
                _check->show();
            else
                _check->hide();
        }
    }
}

// Print text with no control binding
void draw_text(int x, int y, Color color, const char *string) 
{
    int v_offset = font_height;
    int h_offset = 0;
	glColor3f( color[0], color[1], color[2] );
    glRasterPos2d(x, y + v_offset);
    for( const char* p = string; *p; p++)
    {
        if (*p == '\n')
            glRasterPos2d(x, y + (v_offset+=font_height));
        else if (*p == '\t')
        {
            for (int i = 0; i < 4; i++)
                glutBitmapCharacter(font, ' ');
        }
        else if (*p >= ' ' && *p <= '~')
            glutBitmapCharacter(font, *p);
    }
}

// Print text aligned to the center of the active control
void center_text(int h_padding = 0, int v_padding = 0)
{
    // Get text width and height
    const char *string = control->get("text").value.c_str();
    int line_width = 0;
    int width  = font_width;
    int height = font_height;
    for (const char* p = string; *p; p++)
    {
        if (*p == '\n')
        {
            height += font_height;
            line_width = 0;
        }
        else if (*p == '\t')
            line_width += font_width * 4;
        else if (*p >= ' ' && *p <= '~')
            line_width += font_width;
        if (line_width > width)
            width = line_width;
    }
    
    // Calculate offsets
    int h_offset = control->rect[0] + (control->width()/2) - (width/2);
    if (h_offset < 0)
        h_offset = 0;
    int v_offset = control->rect[1] + (control->height()/2) - (height/2);
    if (v_offset < 0)
        v_offset = 0;
    
    // Draw the text
    draw_text(h_offset + h_padding, v_offset + v_padding, control->color["text"] , string);
}

#include <cstdint>
#define char_map uint64_t
#define char_first 0x8000000000000000
#define shift_right(item) item = item >> 1
#define shift_left(item) item = item << 1
class Characters
{
public:
    char_map get(char c)
    {
        switch (c)
        {
            case '\t':
                return 0x0000000000000000;
            case ' ':
                return 0x0000000000000000;
            case '0':
                return 0x1824242424180000;
            case '1':
                return 0x08180808081C0000;
            case '2':
                return 0x18240408103C0000;
            case '3':
                return 0x1824080424180000;
            case '4':
                return 0x0818283C08080000;
            case '5':
                return 0x3C20380424180000;
            case '6':
                return 0x1820382424180000;
            case '7':
                return 0x3C04080810100000;
            case '8':
                return 0x1824182424180000;
            case '9':
                return 0x1824241C04180000;
            case '-':
                return 0x0000003C00000000;
            case '_':
                return 0x00000000007E0000;
            case '+':
                return 0x0008083E08080000;
            case '=':
                return 0x00003C003C000000;
            case '\\':
                return 0x4020100804020000;
            case '/':
                return 0x0204081020400000;
            case '|':
                return 0x0808080808080000;
            case '[':
                return 0x1810101010180000;
            case ']':
                return 0x1808080808180000;
            case '{':
                return 0x0810303010080000;
            case '}':
                return 0x10080C0C08100000;
            case '!':
                return 0x0808080800080000;
            case '?':
                return 0x1824040800080000;//?
            case '.':
                return 0x0000000018180000;
            case ',':
                return 0x0000000018081000;
            case '\'':
                return 0x0808000000000000;
            case '"':
                return 0x1414000000000000;
            case '@':
                return 0x18242C2024180000;
            case '#':
                return 0x14143E143E141400;
            case '$':
                return 0x1C2A180C2A1C0000;
            case '%':
                return 0x6264081026460000;
            case '^':
                return 0x0814220000000000;
            case '&':
                return 0x18201428241A0000;//?
            case '*':
                return 0x081C080000000000;
            case '(':
                return 0x0810101010080000;
            case ')':
                return 0x1008080808100000;
            case '<':
                return 0x0008102010080000;
            case '>':
                return 0x0010080408100000;
            case ';':
                return 0x0000080000081000;
            case ':':
                return 0x0000080000080000;
            case 'a':
                return 0x0000182424140000;
            case 'b':
                return 0x2020382424380000;
            case 'c':
                return 0x0000182020180000;
            case 'd':
                return 0x04041C24241C0000;
            case 'e':
                return 0x0000183C20180000;//
            case 'f':
                return 0x0408081C08080000;
            case 'g':
                return 0x00001824241C0418;
            case 'h':
                return 0x2020382424240000;
            case 'i':
                return 0x08001808081C0000;// serif
            case 'j':
                return 0x0800180808080830;// serif
            case 'k':
                return 0x2020283028240000;//?
            case 'l':
                return 0x18080808081C0000;// serif
            case 'm':
                return 0x0000342A2A2A0000;
            case 'n':
                return 0x0000382424240000;
            case 'o':
                return 0x0000182424180000;
            case 'p':
                return 0x0000382424382020;
            case 'q':
                return 0x00001C24241C0404;
            case 'r':
                return 0x0000182420200000;
            case 's':
                return 0x000C100804180000;/////////////////
            case 't':
                return 0x08081C0808040000;
            case 'u':
                return 0x00002424241C0000;
            case 'v':
                return 0x0000242414080000;
            case 'w':
                return 0x00002A2A2A340000;
            case 'x':
                return 0x0000241818240000;
            case 'y':
                return 0x00002424241C0418;
            case 'z':
                return 0x00003C08103C0000;
            case 'A':
                return 0x18243C2424240000;
            case 'B':
                return 0x3824382424380000;
            case 'C':
                return 0x1824202024180000;
            case 'D':
                return 0x3824242424380000;
            case 'E':
                return 0x3C203C20203C0000;
            case 'F':
                return 0x3C203C2020200000;
            case 'G':
                return 0x1824202C24180000;
            case 'H':
                return 0x24243C2424240000;
            case 'I':
                return 0x1C080808081C0000;
            case 'J':
                return 0x1C04040424180000;
            case 'K':
                return 0x2428302824240000;
            case 'L':
                return 0x20202020203C0000;
            case 'M':
                return 0x342A2A2A2A2A0000;
            case 'N':
                return 0x3824242424240000;//
            case 'O':
                return 0x1824242424180000;
            case 'P':
                return 0x3824243820200000;
            case 'Q':
                return 0x1824242424180400;
            case 'R':
                return 0x3824243824240000;
            case 'S':
                return 0x1820180424180000;/////////////////
            case 'T':
                return 0x3E08080808080000;//?
            case 'U':
                return 0x24242424241C0000;
            case 'V':
                return 0x2424242414080000;//?
            case 'W':
                return 0x2A2A2A2A2A340000;
            case 'X':
                return 0x2424181824240000;//?
            case 'Y':
                return 0x24241C0404080000;//?
            case 'Z':
                return 0x3C040810203C0000;
            default:
                return 0x2A542A542A542A54;
        }
    }
} Chars;

void draw_char(char_map c, int x, int y, int font_size)
{
    if (c)
    {
        char_map char_bit = char_first;
        if (font_size > 1)
        {
            for (int j = 0; j < 8; j++)
            {
                for (int k = 0; k < 8; k++)
                {
                    if (c & char_bit)
                    {
                        
                        glVertex2d(x + k * font_size, y + j * font_size);
                    }
                    shift_right(char_bit);
                }
            }
        }
        else
        {
            for (int j = 0; j < 8; j++)
            {
                for (int k = 0; k < 8; k++)
                {
                    if (c & char_bit)
                    { 
                        glVertex2d(x + k, y + j);
                    }
                    shift_right(char_bit);
                }
            }
        }
    }
}

// Print text aligned to the top-left of the active control
void right_align_text(bool editable = false, int h_padding = 2, int v_padding = 2, int font_size = 1)
{
    // Draw the text
    string str = (string)control->get("text") + ' ';
    int length = str.length();
    int max_width = control->rect[2] - control->rect[0];
    int start_x = control->rect[0] + h_padding;
    int start_y = control->rect[1] + v_padding;
    char_map c;
    int line_char = 0;
    int line = 0;
    char_map char_bit;
    bool drawCursor = (editable && (control == active));
    
    if (font_size > 1)
    {
        glPointSize(font_size);
        glBegin(GL_POINTS);
        for (int i = 0; i < length; i++)
        {
            if (drawCursor && i == cursor_loc)
            {
                for (int j = 0; j < 4; j++)
                    glVertex2d(start_x + line_char * font_size + j * font_size, start_y + 8 * font_size + line);
            }
            if (str[i] == '\n' || str[i] == '\r')
            {
                line_char = 0;
                line += (font_size * 10 + 2);
                continue;
            }
            else if (str[i] <= ' ')
            {
                line_char += (font_size * 6 + 1);
                continue;
            }
            else
            {
                draw_char(Chars.get(str[i]), start_x + line_char, start_y + line, font_size);
                line_char += (font_size * 6 + 1);
            }
        }
        glEnd();
    }
    else
    {
        glPointSize(1);
        glBegin(GL_POINTS);
        for (int i = 0; i < length; i++)
        {
            if (drawCursor && i == cursor_loc)
            {
                for (int j = 0; j < 4; j++)
                    glVertex2d(start_x + line_char + j, start_y + 8 + line);
            }
            if (str[i] == '\n' || str[i] == '\r')
            {
                line_char = 0;
                line += 12;
                continue;
            }
            else if (str[i] <= ' ')
            {
                line_char += 7;
                continue;
            }
            else
            {
                draw_char(Chars.get(str[i]), start_x + line_char, start_y + line, font_size);
                line_char += 7;
            }
        }
        glEnd();
    }
}

// Get the topmost control at the mouseloc
bool set_control(int x, int y)
{
    Control* _check;
    bool set = false;
    control = 0;
    
    // Find in UI controls
    for (int i = 0; i < X.toplevel.size(); i++)
    {
        _check = &X.toplevel[i];
        if (_check->visible && _check->enabled && _check->within(x, y))
        {
            control = _check;
            set = true;
        }
    }
    
    // Find in standard controls
    if (!set)
    {
        // Get the mouse control
        for (int i = 0; i < X.controls.size(); i++)
        {
            _check = &X.controls[i];
            if (_check->visible && _check->enabled && _check->within(x, y))
            {
                control = _check;
                set = true;
            }
        }
        
        // Return false no control was found
        if (!set)
            return false;
    }
    return true;
}

// Send mousedown, mouseup, and mouserelease to the clicked control
void mouse_func(int button, int state, int x, int y)
{
    // Set global button data
    mousebutton = button;
    
    // Exit if no controls are at the mouseloc
    if (!set_control(x, y))
        return;
    
    if (control != active)
        cursor_loc = 0;
    
    // Set active for keyboard capture
    active = control;
    
    // Send the message to the new active control
    if (state == GLUT_DOWN && control) 
    {
        clickloc_x = x;
        clickloc_y = y;
        control->active = true;
        control->mousedown();
        clicked = control;
    }
    else if (state == GLUT_UP)
    {
        // Send mouseup
        if (control && active == control)
        {
            control->mouseup();
            control->active = false;
            control = 0;
        }
        
        // Send mouserelease if mouseup on another control
        if (clicked && !clicked->within(x, y))
        {
            control = clicked;
            control->mouserelease();
            control->active = false;
            clicked = 0;
        }
    }
    
    // Reload the display
    glutPostRedisplay();
}

// Send messages when the mouse leaves/enters the window
void entry_func(int state)
{
    // Send mouserelease if mouse leaves/enters the window
    if (clicked)
    {
        control = clicked;
        control->mouserelease();
        control->active = false;
        clicked = 0;
    }    
}

// Send activemousemove to the active control
void motion_func(int x, int y)
{
    // Exit if no controls are active
    if (control == 0)
        return;
    
    // Set global mouse data
    mousediff_x = clickloc_x - x;
    mousediff_y = clickloc_y - y;
    mouseloc_x = x;
    mouseloc_y = y;
    
    // Send the mousemove message to the control
    if (control->visible && control->enabled)
        active->activemousemove();
}

// Send mouseenter, mouseexit, and mousemove to the mouse control
void passive_motion_func(int x, int y)
{
    // Exit if no controls are at the mouseloc
    if (!set_control(x, y))
        return;
    
    // Set global mouse data
    mousediff_x = mouseloc_x - x;
    mousediff_y = mouseloc_y - y;
    mouseloc_x = x;
    mouseloc_y = y;
    
    // Send the mousemove message to the control
    control->mousemove();
    
    // Reload the display
    glutPostRedisplay();
}

// Handle resizing the window
void resize_window_func()
{
    // Handle the controls
    for (int i = 0; i < X.controls.size(); i++)
    {
        control = &X.controls[i];
        control->windowresize();
    }
    
    // Handle toplevel controls
    for (int i = 0; i < X.toplevel.size(); i++)
    {
        control = &X.toplevel[i];
        control->windowresize();
    }
    
    // Reload the display
    glutPostRedisplay();
}

// Called on each idle cycle (default 20 ms)
void idle_func(int data)
{
    // Handle the registered controls
    for (int i = 0; i < X.idle_controls.size(); i++)
    {
        control = X.idle_controls[i];
        control->idle();
    }
    
    // Reload the display
    glutPostRedisplay();
    
    // Restart the timer
    glutTimerFunc(X.idle_rate, idle_func, 0);
}

// Display all visible controls.
void display_func() 
{
    Color& clear = X.window.color;
    glClearColor(clear[0], clear[1], clear[2], clear[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Display the controls
    for (int i = 0; i < X.controls.size(); i++)
    {
        control = &X.controls[i];
        if (control->visible)
            control->display();
    }
    
    // Display toplevel controls
    for (int i = 0; i < X.toplevel.size(); i++)
    {
        control = &X.toplevel[i];
        if (control->visible)
            control->display();
    }

    glutSwapBuffers();
}

void keyboard_func(unsigned char key, int x, int y)
{
    // Send the keystroke to the active control
    if (active)
    {
        thekey = key;
        control = active;
        active->keydown();
        
        // Reload the display
        glutPostRedisplay();
    }
}

void send_arrow_down()
{
    if (active)
    {
        control = active;
        control->arrowkeydown();
    }
    else if (control)
        control->arrowkeydown();
    
    // Reload the display
    glutPostRedisplay();
}

void arrow_down_function(int key, int x, int y) 
{
    switch (key) 
    {
        case GLUT_KEY_LEFT:
            if (!(arrowkey & ARROW_LEFT))
                arrowkey += ARROW_LEFT;
            send_arrow_down();
            break;
        case GLUT_KEY_RIGHT:
            if (!(arrowkey & ARROW_RIGHT))
                arrowkey += ARROW_RIGHT;
            send_arrow_down();
            break;
        case GLUT_KEY_UP:
            if (!(arrowkey & ARROW_UP))
                arrowkey += ARROW_UP;
            send_arrow_down();
            break;
        case GLUT_KEY_DOWN:
            if (!(arrowkey & ARROW_DOWN))
                arrowkey += ARROW_DOWN;
            send_arrow_down();
            break;
    }
}

void send_arrow_up()
{
    if (active)
    {
        control = active;
        control->arrowkeyup();
    }
    else if (control)
        control->arrowkeyup();
        
    // Reload the display
    glutPostRedisplay();
}

void arrow_up_function(int key, int x, int y) 
{
    switch (key) 
    {
        case GLUT_KEY_LEFT:
            if (arrowkey & ARROW_LEFT)
                arrowkey -= ARROW_LEFT;
            send_arrow_up();
            break;
        case GLUT_KEY_RIGHT:
            if (arrowkey & ARROW_RIGHT)
                arrowkey -= ARROW_RIGHT;
            send_arrow_up();
            break;
        case GLUT_KEY_UP:
            if (arrowkey & ARROW_UP)
                arrowkey -= ARROW_UP;
            send_arrow_up();
            break;
        case GLUT_KEY_DOWN:
            if (arrowkey & ARROW_DOWN)
                arrowkey -= ARROW_DOWN;
            send_arrow_up();
            break;
    }
}

#endif