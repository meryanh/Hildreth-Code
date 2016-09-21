#ifndef VAR_H
#define VAR_H

char delimiter = ','; // Item delimiter

vector<string> split(string v, char d);
string toString(double v);
double toNum(string v);

class var
{
public:
    string value;
    string name;
    
    var();
    var(double v, string n);
    var(string v, string n);
    int round();
    char* c_str();
    
    /* OPERATORS */
    // Addition
    var operator +(const double v);
    var& operator +=(const double v);
    var operator +(const int v);
    var& operator +=(const int v);
    
    // Subtraction
    var operator -(const double v);
    var& operator -=(const double v);
    var operator -(const int v);
    var& operator -=(const int v);

    // Multiplication
    var operator *(const double v);
    var& operator *=(const double v);
    var operator *(const int v);
    var& operator *=(const int v);
        
    // Division
    var operator /(const double v);
    var& operator /=(const double v);
    var operator /(const int v);
    var& operator /=(const int v);

    // Modulus
    var operator %(const double v);
    var& operator %=(const double v);
    var operator %(const int v);
    var& operator %=(const int v);
        
    // String combination
    var& operator &=(const string v);
    var operator &(const string v);
        
    // String comparison
    bool operator ==(const double v);
    bool operator ==(const string v);
    bool operator !=(const string v);
        
    // Boolean comparison
    bool operator and(const bool v);
    bool operator or(const bool v);
    bool operator not();
    
    // Assignment
    bool operator =(const var v);
    bool operator =(const string v);
    bool operator =(const int v);
    bool operator =(const double v);
    
    // Casting
    operator double();
    operator string();
    operator bool();
    
    // New operator
    void* operator new(unsigned int);
    
    // Stream operator
    friend ostream& operator<<(ostream& os, const var& v)
    {
        os << v.value;
        return os;
    }
    
    // Get item by delimiter
    var operator [](const int index);
};

class VarMap
{
private:
    void operator=(void*x);
    void operator&();
    void operator*();
protected:
    vector<var> vars;
public:
    VarMap();
    static VarMap V;
    
    var& operator [](string index);
    int size();
} VarMap::V, &V=VarMap::V;

#endif