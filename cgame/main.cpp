#include "cgame/cgame.h"

template <typename T, typename I>
T volatile_cast(I value)
{
    return (*(T*)(&value));
}

template <typename T>
void object_to_binary(T &object, char *out)
{
    char *binary = volatile_cast<char*>(&object);
    for (int i = 0; i < sizeof(T); i++)
        out[i] = binary[i];
}

class Cell
{
public:
    int attributes;
    int texture_id;
    Cell()
    {
        attributes = 0;
        texture_id = -1;
    }
};

class Effect
{
public:
    unsigned int id;
    signed int modifier;
};

class Item
{
public:
    Effect effects[5];
    
};

class Entity
{
public:
    
};

/* Run at startup */
void startup()
{
    // Initialize window:
    X.window(640, 480, "Test");
    X.window.color(25, 25, 28);
    X.window.center();
}