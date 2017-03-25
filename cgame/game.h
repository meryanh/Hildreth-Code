#include <iostream>
using namespace std;
#define MAP_WIDTH 50
#define MAP_HEIGHT 20

#define CV_BLOCKED 0x01

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
    unsigned int texture_id;
    Cell(unsigned int _texture_id = 0, unsigned long long int _attributes = 0)
    {
        attributes = _attributes;
        texture_id = _texture_id;
    }
    void draw(int x, int y)
    {
        draw_texture_segment(texture_id, x, y, 16);
    }
};

class Map
{
public:
    Cell **cell;
    Map()
    {
        cell = new Cell*[MAP_HEIGHT];
        for(int i = 0; i < MAP_HEIGHT; ++i) {
            cell[i] = new Cell[MAP_WIDTH];
        }
    }
    ~Map()
    {
        for(int i = 0; i < MAP_HEIGHT; ++i) {
            delete [] cell[i];
        }
        delete [] cell;
    }
    void draw(long long int offset_x, long long int offset_y)
    {
        glColor3ub(0xFF, 0xFF, 0xFF);
        offset_x -= window_width / 2;
        offset_y -= window_height / 2;
        glEnable(GL_TEXTURE_2D);
        int start_x = offset_x / 16;
        int start_y = offset_y / 16;
        int end_x;
        int end_y;
        glfwGetWindowSize(window, &end_x, &end_y);
        end_x = start_x + (end_x / 16) + 2;
        if (end_x > MAP_WIDTH)
            end_x = MAP_WIDTH;
        end_y = start_y + (end_y / 16) + 2;
        if (end_y > MAP_HEIGHT)
            end_y = MAP_HEIGHT;
        if (start_x < 0)
            start_x = 0;
        if (start_y < 0)
            start_y = 0;
        for (int j = start_x; j < end_x; j++)
            for (int k = start_y; k < end_y; k++)
            {
                cell[k][j].draw(j*16 - offset_x, k*16 - offset_y);
            }
        glDisable(GL_TEXTURE_2D);
    }
};

void draw_hp_bar(int bar_start_pos_x, int bar_start_pos_y, int bar_length, float bar_percent, float bar_reverse_percent)
{
    glColor3ub(0xFF, 0xFF, 0xFF);
    glBegin(GL_POLYGON);
    glVertex2i(bar_start_pos_x+4, bar_start_pos_y);
    glVertex2i(bar_start_pos_x+bar_length+4, bar_start_pos_y);
    glVertex2i(bar_start_pos_x+bar_length, bar_start_pos_y+5);
    glVertex2i(bar_start_pos_x, bar_start_pos_y+5);
    glEnd();
    
    int bar_end = bar_percent*bar_length;
    if (bar_end <= 2)
        return;
    else if (bar_end > bar_length)
        bar_end = bar_length;
    if (bar_percent > bar_reverse_percent)
    {
        glColor3ub(0x9A, 0x00, 0x00);
        glBegin(GL_POLYGON);
        glVertex2i(bar_start_pos_x+4, bar_start_pos_y+1);
        glVertex2i(bar_start_pos_x+bar_end+3, bar_start_pos_y+1);
        glVertex2i(bar_start_pos_x+bar_end-1, bar_start_pos_y+4);
        glVertex2i(bar_start_pos_x+2, bar_start_pos_y+4);
        glEnd();
    }
    else
        bar_reverse_percent = bar_percent;
    
    int bar_begin = bar_end - (bar_reverse_percent * bar_length);
    if (bar_begin <= 2)
        bar_begin = bar_start_pos_x;
    glColor3ub(0x00, 0x00, 0x00);
    glBegin(GL_POLYGON);
    glVertex2i(bar_begin+4, bar_start_pos_y+1);
    glVertex2i(bar_start_pos_x+bar_end+3, bar_start_pos_y+1);
    glVertex2i(bar_start_pos_x+bar_end-1, bar_start_pos_y+4);
    glVertex2i(bar_begin+2, bar_start_pos_y+4);
    glEnd();
}