#ifndef WINDOW_CPP
#define WINDOW_CPP
#include "Window.h"
Window::Window(){ ready = false; };
int Window::width() { return _width; }
void Window::setwidth(int width)
{
    if (width < 60)
        width = 60;
    _width = width;
    glPopMatrix();
    glPushMatrix();
    glutReshapeWindow(_width, _height + titlebar_height);
    glOrtho(-0.5, _width - 0.5, _height + titlebar_height - 0.5, -0.5, 0, 1);
    resize_window_func();
}
int Window::height() { return _height; }
void Window::setheight(int height)
{
    if (height < 100)
        height = 100;
    _height = height;
    glPopMatrix();
    glPushMatrix();
    glutReshapeWindow(_width, _height + titlebar_height);
    glOrtho(-0.5, _width - 0.5, _height + titlebar_height - 0.5, -0.5, 0, 1);
    resize_window_func();
}
void Window::setsize(int width, int height)
{
    if (height < 100)
        height = 100;
    if (width < 100)
        width = 100;
    _width = width;
    _height = height;
    glPopMatrix();
    glPushMatrix();
    glutReshapeWindow(_width, _height + titlebar_height);
    glOrtho(-0.5, _width - 0.5, _height + titlebar_height - 0.5, -0.5, 0, 1);
    resize_window_func();
}
int Window::pos_x() { return glutGet((GLenum)GLUT_WINDOW_X); }
int Window::pos_y() { return glutGet((GLenum)GLUT_WINDOW_Y); }
int Window::x() { return _x; }
int Window::y() { return _y; }
void Window::setloc(int x, int y)
{
    _x = x;
    _y = y;
    glutPositionWindow(x, y);
}
void Window::center()
{
    setloc(screenwidth()/2-_width/2, screenheight()/2-(_height + titlebar_height)/2);
}
void Window::move(int x, int y)
{
    setloc(_x - x, _y - y);
}
void Window::iconify()
{
    glutIconifyWindow();
}
string Window::title() { return _title; }
void Window::operator ()(int width, int height, int x, int y, string title) 
{ 
    if (ready) return;
    ready = true;
    glutInitWindowSize(width, height + titlebar_height);
    glutInitWindowPosition(x, y);
    glutCreateWindow(title.c_str());
    _width = width;
    _height = height;
    _x = x;
    _y = y;
    _title = title;
    glPushMatrix();
    glOrtho(-0.5, _width - 0.5, _height + titlebar_height - 0.5, -0.5, 0, 1);
}
void Window::operator ()(int width, int height, string title) 
{ 
    if (ready) return;
    ready = true;
    glutInitWindowSize(width, height + titlebar_height);
    glutCreateWindow(title.c_str());
    _width = width;
    _height = height;
    _x = glutGet((GLenum)GLUT_WINDOW_X);
    _y = glutGet((GLenum)GLUT_WINDOW_Y);
    _title = title;
    glPushMatrix();
    glOrtho(-0.5, _width - 0.5, _height + titlebar_height - 0.5, -0.5, 0, 1);
}

#endif