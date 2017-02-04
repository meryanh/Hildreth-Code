#ifndef RECT_CPP
#define RECT_CPP
#include "Rect.h"
// Rect class:
Rect::Rect()
{
    rect[0] = 0;
    rect[1] = 0;
    rect[2] = 0;
    rect[3] = 0;
}
Rect::Rect(int x, int y, int dx, int dy)
{
    rect[0] = x;
    rect[1] = y  + titlebar_height;
    rect[2] = dx;
    rect[3] = dy + titlebar_height;
}
void Rect::operator ()(int x, int y, int dx, int dy)
{
    rect[0] = x;
    rect[1] = y  + titlebar_height;
    rect[2] = dx;
    rect[3] = dy + titlebar_height;
}
int Rect::operator [](int i) const    {return rect[i];}
int& Rect::operator [](int i) {return rect[i];}

#endif