#ifndef RECT_H
#define RECT_H

class Rect
{
public:
    int rect[4];
    
    Rect();
    Rect(int x, int y, int dx, int dy);
    void operator ()(int x, int y, int dx, int dy);
    int operator [](int i)const;
    int & operator [](int i);
};

#endif