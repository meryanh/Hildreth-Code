#ifndef WINDOW_H
#define WINDOW_H

class Window
{
private:
    int _width;
    int _height;
    int _x;
    int _y;
    string _title;
    bool ready;
public:
    Window();
    Color color;
    int width();
    void setwidth(int width);
    int height();
    void setheight(int height);
    void setsize(int width, int height);
    int pos_x();
    int pos_y();
    int x();
    int y();
    void setloc(int x, int y);
    void center();
    void move(int x, int y);
    void iconify();
    string title();
    void operator ()(int width, int height, int x, int y, string title);
    void operator ()(int width, int height, string title);
};

#endif