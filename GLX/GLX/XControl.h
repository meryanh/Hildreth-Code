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
    }
    XControl  operator =(XControl& x){}
    XControl* operator &(){}
    XControl& operator *(){}
public:
    static XControl _;
    vector<Control*> idle_controls;
    vector<Control> controls; // Standard controls
    vector<Control> toplevel; // TopLevel (UI) controls
    unsigned int idle_rate;   // Millisecond delay for 'idle'
    Window window;
    
    void start(int* argc, char** argv);
    Control* get_control(string name);
    void register_idle();
    
    XControl& operator [](string i);
    Control* operator ->();
    XControl& operator !();
    int operator ()(int* argc, char** argv);
    void operator <<(string i);
} XControl::_, &X = XControl::_;

#endif