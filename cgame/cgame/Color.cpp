#ifndef COLOR_CPP
#define COLOR_CPP
#include "Color.h"
// Color class:
Color::Color()
{
    color[0] = 150;
    color[1] = 150;
    color[2] = 150;
    color[3] = 0;
}
Color::Color(float r, float g, float b, float a = 0)
{
    color[0] = r/255.0;
    color[1] = g/255.0;
    color[2] = b/255.0;
    color[3] = a/255.0;
}
void Color::operator ()(float r, float g, float b, float a = 0) 
{
    color[0] = r/255.0;
    color[1] = g/255.0;
    color[2] = b/255.0;
    color[3] = a/255.0;
}
float  Color::operator [](int i) const    {return color[i];}
float& Color::operator [](int i)          {return color[i];}

// ColorMap class:
class ColorMap::_Color
{
public:
    string name;
    Color color;
    
    _Color(string _name, Color _color)
    {
        name = _name;
        color = _color;
    }
};
vector<ColorMap::_Color> _colors;

ColorMap::ColorMap()
{
    add("default", Color());
}
void ColorMap::add(string name, Color color)
{
    _colors.push_back(ColorMap::_Color(name, color));
}
Color& ColorMap::get(string name)
{
    for (int i = 0; i < _colors.size(); i++)
    {
        if (name == _colors[i].name)
            return _colors[i].color;
    }
    add(name, Color());
    return _colors.back().color;
}

Color& ColorMap::operator [](string i) {return get(i);}
void ColorMap::operator =(ColorMap  x){}
void ColorMap::operator =(ColorMap& x){}

#endif