#ifndef XCONTROL_H
#define XCONTROL_H

class XControl
{
private:
    Control* _last;
    bool _toplevel;
    bool initial;
    XControl()
    {
        _toplevel = false;
        _last     = NULL;
        initial   = false;
        idle_rate = 20;
        root = new Control("root");
        root->rect(-32768, -32768, 32767, 32767);
        active  = 0;
        control = 0;
        clicked = 0;
    }
    XControl  operator =(XControl& x){}
    XControl* operator &(){}
    XControl& operator *(){}
    Control* root;
public:
    Control* active;    // Active control for idle/activemousemove messages
    Control* control;   // Currently executing control
    Control* clicked;   // Previously clicked control
    static XControl _;
    Control* get_root();
    unsigned int idle_rate;   // Millisecond delay for 'idle'
    Window window;
    
    void start(int* argc, char** argv);
    Control* find_control(string &name, Control* c = NULL);
    Control* control_at_point(int x, int y, Control* c = NULL);
    //void register_idle();
    
    void add(Control* c);
    int operator ()(int* argc, char** argv);
} XControl::_, &X = XControl::_;

#endif