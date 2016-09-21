#ifndef FUNCTION_H
#define FUNCTION_H

// Functor class definition:
class Function
{
public:
    void (*func)(void);
    Function(void (*_func)(void));
    Function();
    void operator ()(void (*_func)(void));
    void operator ()(Function _func);
    void operator ()();
};

// Functor macro:
#define function(FNAME) \
void _##FNAME##_(); \
Function FNAME(_##FNAME##_); \
void _##FNAME##_()

#endif