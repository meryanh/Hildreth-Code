#ifndef CONTROL_H
#define CONTROL_H

class Control
{
private:
    string _name;
    VarMap _vars;
    Control* _parent;
    vector<Control*> _controls;
public:
    Rect rect;
    int group;
    bool visible;
    bool enabled;
    bool active;
    ColorMap color;
    
    Control();
    Control(string name, Control* parent = NULL);
    
    void show();
    void hide();
    void disable();
    void enable();
    
    int width();
    void setwidth(int width);
    int height();
    void setheight(int height);
    
    const char* chars();
    string name();
    
    void move(int x, int y);
    
    // Determine if a point is within the rect of this control
    bool within(int x, int y);

    // Proto-Function declarations
    Function display;        // Called when drawn (how to draw the control)
    Function activate;       // Called when this control is activated
    Function mouseenter;     // Called when the mouse enters the control
    Function mouseexit;      // Called when the mouse exits the control
    Function mousedown;      // Called when mouse button is pressed within the control
    Function mouseup;        // Called when mouse button is released within the control
    Function mouserelease;   // Called when mouse button is released outside of the control
    Function keydown;        // Called when a keyboard button is pressed
    Function keyup;          // Called when a keyboard button is released
    Function arrowkeydown;   // Called when a arrow key is pressed
    Function arrowkeyup;     // Called when a arrow key is released
    Function mousemove;      // Called when the mouse is moved within this control
    Function activemousemove;// Called when the mouse is moved while pressed
    Function windowresize;   // Called when the window size is changed
    Function destroy;        // Called when this control is destroyed
    Function idle;           // Called on every timer cycle while active
    
    Control* get_parent();
    int count();
    Control* add(string n);
    Control* add(Control* c);
    
    var& operator [](string i);
    Control* operator [](int i);
    
    var& get(string i);
    
    void attribute(string i, double v);
    void attribute(string i, string v);
    void attribute(string i, bool v);
};

#endif