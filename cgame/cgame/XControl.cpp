#ifndef XCONTROL_CPP
#define XCONTROL_CPP
#include "XControl.h"
#include "Windows.h"

#define GL_MULTISAMPLE_ARB 0x809D
void XControl::start(int* argc, char** argv)
{
    glutInit(argc, argv);
    glutInitDisplayMode(0x0C92); // 0x0C92 (undecorated), 0x0092 (title bar), 0x0492 (captionless)
    startup();
    glutMouseFunc(mouse_func);
    glutTimerFunc(idle_rate, idle_func, 0);
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

    
// TO DO:
//  Implement Entity, Cell, and Item classes
//  Create arrays of entities, cells, and items
//  Find a way to draw textures
//  

#include <cstdint>
#define char_map uint64_t
#define char_first 0x8000000000000000
#define shift_right(item) item = item >> 1
#define shift_left(item) item = item << 1
uint64_t _char_map[256] = 
{
    0x0000000000000000,// NUL
    0x0000000000000000,// SOH
    0x0000000000000000,// STX
    0x0000000000000000,// EOT
    0x0000000000000000,// ENQ
    0x0000000000000000,// ACK
    0x0000000000000000,// BEL
    0x0000000000000000,// BS
    0x0000000000000000,// TAB
    0x0000000000000000,// LF
    0x0000000000000000,// VT
    0x0000000000000000,// FF
    0x0000000000000000,// CR
    0x0000000000000000,// SO
    0x0000000000000000,// SI
    0x0000000000000000,// DLE
    0x0000000000000000,// DC1
    0x0000000000000000,// DC2
    0x0000000000000000,// DC3
    0x0000000000000000,// DC4
    0x0000000000000000,// NAK
    0x0000000000000000,// SYN
    0x0000000000000000,// ETB
    0x0000000000000000,// CAN
    0x0000000000000000,// EM
    0x0000000000000000,// SUB
    0x0000000000000000,// ESC
    0x0000000000000000,// FS
    0x0000000000000000,// GS
    0x0000000000000000,// RS
    0x0000000000000000,// US
    0x0000000000000000,// SPACE
    0x0808080800080000,// !
    0x1414000000000000,// "
    0x14143E143E141400,// #
    0x1C2A180C2A1C0000,// $
    0x6264081026460000,// %
    0x18201428241A0000,// &
    0x0808000000000000,// '
    0x0810101010080000,// (
    0x1008080808100000,// )
    0x081C080000000000,// *
    0x0008083E08080000,// +
    0x0000000018081000,// ,
    0x0000003C00000000,// -
    0x0000000018180000,// .
    0x0204081020400000,// /
    0x0000000000000000,// 0
    0x0000000000000000,// 1
    0x0000000000000000,// 2
    0x0000000000000000,// 3
    0x0000000000000000,// 4
    0x0000000000000000,// 5
    0x0000000000000000,// 6
    0x0000000000000000,// 7
    0x0000000000000000,// 8
    0x0000000000000000,// 9
    0x0000000000000000,// :
    0x0000000000000000,// ;
    0x0000000000000000,// <
    0x0000000000000000,// =
    0x0000000000000000,// >
    0x0000000000000000,// ?
    0x0000000000000000,// @
    0x0000000000000000,// A
    0x0000000000000000,// B
    0x0000000000000000,// C
    0x0000000000000000,// D
    0x0000000000000000,// E
    0x0000000000000000,// F
    0x0000000000000000,// G
    0x0000000000000000,// H
    0x0000000000000000,// I
    0x0000000000000000,// J
    0x0000000000000000,// K
    0x0000000000000000,// L
    0x0000000000000000,// M
    0x0000000000000000,// N
    0x0000000000000000,// N
    0x0000000000000000,// O
    0x0000000000000000,// P
    0x0000000000000000,// Q
    0x0000000000000000,// R
    0x0000000000000000,// S
    0x0000000000000000,// T
    0x0000000000000000,// U
    0x0000000000000000,// V
    0x0000000000000000,// W
    0x0000000000000000,// X
    0x0000000000000000,// Y
    0x0000000000000000,// Z
    0x0000000000000000,// [
    0x0000000000000000,// \ 
    0x0000000000000000,// ]
    0x0000000000000000,// ^
    0x0000000000000000,// _
    0x0000000000000000,// `
    0x0000000000000000,// a
    0x0000000000000000,// b
    0x0000000000000000,// c
    0x0000000000000000,// d
    0x0000000000000000,// e
    0x0000000000000000,// f
    0x0000000000000000,// g
    0x0000000000000000,// h
    0x0000000000000000,// i
    0x0000000000000000,// j
    0x0000000000000000,// k
    0x0000000000000000,// l
    0x0000000000000000,// m
    0x0000000000000000,// n
    0x0000000000000000,// o
    0x0000000000000000,// p
    0x0000000000000000,// q
    0x0000000000000000,// r
    0x0000000000000000,// s
    0x0000000000000000,// t
    0x0000000000000000,// u
    0x0000000000000000,// v
    0x0000000000000000,// w
    0x0000000000000000,// x
    0x0000000000000000,// y
    0x0000000000000000,// z
    0x0000000000000000,// {
    0x0000000000000000,// |
    0x0000000000000000,// }
    0x0000000000000000,// ~
    0x0000000000000000 // DEL
};

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
                return ;
            case '_':
                return 0x00000000007E0000;
            case '+':
                return ;
            case '=':
                return 0x00003C003C000000;
            case '\\':
                return 0x4020100804020000;
            case '/':
                return ;
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
                return ;
            case ',':
                return ;
            case '\'':
                return 0x0808000000000000;
            case '"':
                return 0x1414000000000000;
            case '@':
                return 0x18242C2024180000;
            case '#':
                return 0x14143E143E141400;
            case '$':
                return ;
            case '%':
                return ;
            case '^':
                return 0x0814220000000000;
            case '&':
                return ;//?
            case '*':
                return ;
            case '(':
                return ;
            case ')':
                return ;
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

int XControl::operator ()(int* argc, char** argv)
{
    start(argc, argv);
}
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

void right_align_text(string str, int start_x, int start_y, int max_width = 0xFFFFFFFF, int font_size = 0x01)
{
    // Draw the text
    str = str + ' ';
    int length = str.length();
    char_map c;
    int line_char = 0;
    int line = 0;
    char_map char_bit;
    
    if (font_size > 1)
    {
        glPointSize(font_size);
        glBegin(GL_POINTS);
        for (int i = 0; i < length; i++)
        {
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

void mouse_func(int button, int state, int x, int y)
{
    
}

void entry_func(int state)
{

}

void motion_func(int x, int y)
{
    
}

void idle_func(int i)
{
    
}

void passive_motion_func(int x, int y)
{
    
}

void resize_window_func()
{
    //glutPostRedisplay();
}

void display_func() 
{
    Color& clear = X.window.color;
    glClearColor(clear[0], clear[1], clear[2], clear[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // TEST:
    right_align_text("test", 300, 240);
    
    glutSwapBuffers();
}

void keyboard_func(unsigned char key, int x, int y)
{
    
}

void arrow_down_function(int key, int x, int y) 
{
    switch (key) 
    {
        case GLUT_KEY_LEFT:
        
            break;
        case GLUT_KEY_RIGHT:
        
            break;
        case GLUT_KEY_UP:
        
            break;
        case GLUT_KEY_DOWN:
        
            break;
    }
}

void arrow_up_function(int key, int x, int y) 
{
    switch (key) 
    {
        case GLUT_KEY_LEFT:
        
            break;
        case GLUT_KEY_RIGHT:
        
            break;
        case GLUT_KEY_UP:
        
            break;
        case GLUT_KEY_DOWN:
        
            break;
    }
}

#endif