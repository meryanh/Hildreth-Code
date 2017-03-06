#include <iostream>
using namespace std;
#define MAP_WIDTH 100
#define MAP_HEIGHT 100

template <class T>
class ListItem {
public:
    T value;
    ListItem<T> *next;
    
    ListItem()
    {
        value = NULL;
        next = NULL;
    }
    ListItem(T v)
    {
        value = v;
        next = NULL;
    }
    T& operator* ()
    {
        return value;
    }
};
template <class T>
class List {
private:
    ListItem<T> *_first;
    ListItem<T> *_last;
    int _count;
public:
    List()
    {
        _first = NULL;
        _last = NULL;
        _count = 0;
    }
    ~List()
    {
        ListItem<T> *item = _first;
        ListItem<T> *next;
        while (item != NULL)
        {
            next = item->next;
            delete item;
            item = next;
        }
    }
    int count()
    {
        return _count;
    }
    
    void add(T value)
    {
        if (_first == NULL)
        {
            _first = new ListItem<T>(value);
            _last = _first;
        }
        else
        {
            ListItem<T> *next = new ListItem<T>(value);
            _last->next = next;
            _last = next;
        }
        _count++;
    }
    
    void remove(int index)
    {
        if (_count < 1)
            return;
        ListItem<T> *item = _first;
        ListItem<T> *before = NULL;
        
        for (int i = 0; i < index; i++)
        {
            before = item;
            item = item->next;
        }
        
        if (before == 0)
            _first = item->next;
        else
            before->next = item->next;
        delete item;
        _count--;
    }
    
    ListItem<T> *get(int index)
    {
        if (index > _count)
            return NULL;
        ListItem<T> *item = _first;
        for (int i = 0; i < index; i++)
        {
            item = item->next;
        }
        return item;
    }
    
    ListItem<T> *first()
    {
        return _first;
    }
    
    ListItem<T> *last()
    {
        return _last;
    }
};

class Entity
{
public:
    unsigned long long int attributes;
    unsigned int type;
    void (*on_frame)(Entity*);
    void (*on_collide)(Entity*);
};

class Cell
{
public:
    unsigned long long int attributes;
};

class Map
{
public:
    Cell cells[MAP_HEIGHT][MAP_WIDTH];
};

int main()
{
    List<int> list;
    list.add(1);
    list.add(2);
    list.add(3);
    list.add(4);
}