#ifndef CONTROL_CPP
#define CONTROL_CPP
#include "Control.h"
// Control class:
Control::Control(){}
Control::Control(string name, int group_id = -1)
{
    _name = name;
    group = group_id;
    visible = true;
    enabled = true;
    active = false;
}

void Control::show()     { visible = true;  }
void Control::hide()     { visible = false; }
void Control::disable()  { enabled = false; }
void Control::enable()   { enabled = true;  }

int Control::width()                 { return abs(rect[2] - rect[0]); }
void Control::setwidth(int width)    { rect.rect[2] = rect[0]+width;  }
int Control::height()                { return abs(rect[3] - rect[1]); }
void Control::setheight(int height)  { rect.rect[3] = rect[1]+height; }

string Control::name()  { return _name; }

void Control::move(int x, int y)
{
    rect[0] += x;
    rect[2] += x;
    rect[1] += y;
    rect[3] += y;
}

// Determine if a point is within the rect of this control
bool Control::within(int x, int y)
{
    return (x > rect[0] && x < rect[2] && y > rect[1] && y < rect[3]);
}

var& Control::operator [](string i)
{
    return _vars[i];
}

var& Control::get(string i)
{
    return _vars[i];
}

void Control::set(string i, double v)
{
    _vars[i] = v;
}

void Control::set(string i, string v)
{
    _vars[i] = v;
}

void Control::set(string i, bool v)
{
    _vars[i] = v;
}
#endif