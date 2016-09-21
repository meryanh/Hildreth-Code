#ifndef VAR_CPP
#define VAR_CPP
#include "var.h"
// Split one string into multiple strings
vector<string> split(string v, char d)
{
    vector<string> result;
    result.push_back("");
    int item = 0;
    int length = v.length();
    
    for (int i = 0; i < length; i++)
    {
        if (v[i] != d)
            result[item] += v[i];
        else
        {
            item++;
            result.push_back("");
        }
    }
    
    return result;
}

// Convert number to string for storage
string toString(double v)
{
    stringstream ss;
    ss << setprecision(32) << v;
    return ss.str();
}

// Convert string to number for calculations
double toNum(string v)
{
    stringstream ss;
    double result;
    ss << v;
    ss >> result;
    
    // Convert to 1 if not a number
    if ((result == 0) && (v != "0") && (v != ""))
        result = 1;
    
    return result;
}

// var class:
var::var()
{
    value = "";
    name = "";
}
var::var(double v, string n = "")
{
    value = toString(v);
    name = n;
}
var::var(string v, string n = "")
{
    value = v;
    name = n;
}

int var::round()
{
    int result = (int)(toNum(value)+0.5);
    value = toString(result);
    return result;
}

/* OPERATORS */
// Addition
var var::operator +(const double v)
{
    return var(toNum(value) + v);
}
var& var::operator +=(const double v)
{
    value = toString(toNum(value) + v);
    return *this;
}
var var::operator +(const int v)
{
    return var(toNum(value) + v);
}
var& var::operator +=(const int v)
{
    value = toString(toNum(value) + v);
    return *this;
}
    
// Subtraction
var var::operator -(const double v)
{
    return var(toNum(value) - v);
}
var& var::operator -=(const double v)
{
    value = toString(toNum(value) - v);
    return *this;
}
var var::operator -(const int v)
{
    return var(toNum(value) - v);
}
var& var::operator -=(const int v)
{
    value = toString(toNum(value) - v);
    return *this;
}

// Multiplication
var var::operator *(const double v)
{
    return var(toNum(value) * v);
}
var& var::operator *=(const double v)
{
    value = toString(toNum(value) * v);
    return *this;
}
var var::operator *(const int v)
{
    return var(toNum(value) * v);
}
var& var::operator *=(const int v)
{
    value = toString(toNum(value) * v);
    return *this;
}
    
// Division
var var::operator /(const double v)
{
    return var(toNum(value) / v);
}
var& var::operator /=(const double v)
{
    value = toString(toNum(value) / v);
    return *this;
}
var var::operator /(const int v)
{
    return var(toNum(value) / v);
}
var& var::operator /=(const int v)
{
    value = toString(toNum(value) / v);
    return *this;
}

// Modulus
var var::operator %(const double v)
{
    return var(((int)toNum(value)) % ((int)v));
}
var& var::operator %=(const double v)
{
    value = toString(((int)toNum(value)) % ((int)v));
    return *this;
}
var var::operator %(const int v)
{
    return var(((int)toNum(value)) % ((int)v));
}
var& var::operator %=(const int v)
{
    value = toString(((int)toNum(value)) % ((int)v));
    return *this;
}
    
// String combination
var& var::operator &=(const string v)
{
    value += v;
    return *this;
}
var var::operator &(const string v)
{
    return var(value + v);
}
    
// String comparison
bool var::operator ==(const double v)
{
    return (toNum(value) == v);
}
bool var::operator ==(const string v)
{
    return (value == v);
}
bool var::operator !=(const string v)
{
    return (value != v);
}
    
// Boolean comparison
bool var::operator and(const bool v)
{
    return ((value != "0") && v);
}
bool var::operator or(const bool v)
{
    return ((value != "0") || v);
}
bool var::operator not()
{
    return (value == "0");
}

// Assignment
bool var::operator =(const var v)
{
    value = v.value;
    name  = v.name;
    return true;
}
bool var::operator =(const string v)
{
    value = v;
    return true;
}
bool var::operator =(const int v)
{
    value = toString(v);
    return true;
}
bool var::operator =(const double v)
{
    value = toString(v);
    return true;
}

// Casting
var::operator double()
{
    return toNum(value);
}
var::operator string()
{
    return value;
}
var::operator bool()
{
    return value != "0";
}

// Get item by delimiter
var var::operator [](const int index)
{
    return var(split(value, delimiter)[index]);
}
void* var::operator new(unsigned int)
{
    return &V["$_" + toString(V.size())];
}

// VarMap class:
VarMap::VarMap(){}
void VarMap::operator=(void*x){}
void VarMap::operator&(){}
void VarMap::operator*(){}
var& VarMap::operator [](string index)
{
    for (int i = 0; i < vars.size(); i++)
        if (vars[i].name == index)
            return vars[i];
    vars.push_back(var("", index));
    return vars.back();
}
int VarMap::size()
{
    return vars.size();
}
#define $(...) V[#__VA_ARGS__]
#endif