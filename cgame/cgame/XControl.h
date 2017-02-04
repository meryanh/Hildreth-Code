#ifndef XCONTROL_H
#define XCONTROL_H

class XControl
{
private:
    XControl()
    {
        idle_rate = 20;
    }
    XControl  operator =(XControl& x){}
    XControl* operator &(){}
    XControl& operator *(){}
public:
    static XControl _;
    unsigned int idle_rate;   // Millisecond delay for 'idle'
    Window window;
    
    void start(int* argc, char** argv);
    int operator ()(int* argc, char** argv);
} XControl::_, &X = XControl::_;

#endif