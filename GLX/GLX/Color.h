#ifndef COLOR_H
#define COLOR_H

class Color
{
public:
    float color[4];
    
    Color();
    Color(float r, float g, float b, float a);
    void operator ()(float r, float g, float b, float a) ;
    float  operator [](int i)const;
    float &operator [](int i);
};

class ColorMap
{
public:
    class _Color;
    
    ColorMap();
    void add(string name, Color color);
    Color& get(string name);
    
    Color& operator [](string i);
    void operator =(ColorMap  x);
    void operator =(ColorMap& x);
private:
    vector<_Color> _colors;
};

#endif