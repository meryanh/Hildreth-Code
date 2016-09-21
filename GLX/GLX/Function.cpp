#ifndef FUNCTION_CPP
#define FUNCTION_CPP
#include "Function.h"
void _null(){}
// Function class:
Function::Function()
{
    func = &_null;
}
Function::Function(void (*_func)(void))
{
    func = _func;
}
void Function::operator ()(void (*_func)(void))
{
    func = _func;
}
void Function::operator ()(Function _func)
{
    func = _func.func;
}
void Function::operator ()(){func();}

#endif