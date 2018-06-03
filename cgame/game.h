using namespace std;
#define MAP_WIDTH 50
#define MAP_HEIGHT 20

#define CV_BLOCKED 0x01

#define KEY_0 0x0000000000000001
#define KEY_1 0x0000000000000002
#define KEY_2 0x0000000000000004
#define KEY_3 0x0000000000000008
#define KEY_4 0x0000000000000010
#define KEY_5 0x0000000000000020
#define KEY_6 0x0000000000000040
#define KEY_7 0x0000000000000080
#define KEY_8 0x0000000000000100
#define KEY_9 0x0000000000000200
#define KEY_A 0x0000000000000400
#define KEY_B 0x0000000000000800
#define KEY_C 0x0000000000001000
#define KEY_D 0x0000000000002000
#define KEY_E 0x0000000000004000
#define KEY_F 0x0000000000008000
#define KEY_G 0x0000000000010000
#define KEY_H 0x0000000000020000
#define KEY_I 0x0000000000040000
#define KEY_J 0x0000000000080000
#define KEY_K 0x0000000000100000
#define KEY_L 0x0000000000200000
#define KEY_M 0x0000000000400000
#define KEY_N 0x0000000000800000
#define KEY_O 0x0000000001000000
#define KEY_P 0x0000000002000000
#define KEY_Q 0x0000000004000000
#define KEY_R 0x0000000008000000
#define KEY_S 0x0000000010000000
#define KEY_T 0x0000000020000000
#define KEY_U 0x0000000040000000
#define KEY_V 0x0000000080000000
#define KEY_W 0x0000000100000000
#define KEY_X 0x0000000200000000
#define KEY_Y 0x0000000400000000
#define KEY_Z 0x0000000800000000
#define KEY_SPACE 0x0000001000000000
#define KEY_UP    0x0000002000000000
#define KEY_RIGHT 0x0000004000000000
#define KEY_DOWN  0x0000006000000000
#define KEY_LEFT  0x0000010000000000

unsigned long long int GLFWKeyToFlag(unsigned int key){
	switch(key){
		case GLFW_KEY_0: return KEY_0;
		case GLFW_KEY_1: return KEY_1;
		case GLFW_KEY_2: return KEY_2;
		case GLFW_KEY_3: return KEY_3;
		case GLFW_KEY_4: return KEY_4;
		case GLFW_KEY_5: return KEY_5;
		case GLFW_KEY_6: return KEY_6;
		case GLFW_KEY_7: return KEY_7;
		case GLFW_KEY_8: return KEY_8;
		case GLFW_KEY_9: return KEY_9;
		case GLFW_KEY_A: return KEY_A;
		case GLFW_KEY_B: return KEY_B;
		case GLFW_KEY_C: return KEY_C;
		case GLFW_KEY_D: return KEY_D;
		case GLFW_KEY_E: return KEY_E;
		case GLFW_KEY_F: return KEY_F;
		case GLFW_KEY_G: return KEY_G;
		case GLFW_KEY_H: return KEY_H;
		case GLFW_KEY_I: return KEY_I;
		case GLFW_KEY_J: return KEY_J;
		case GLFW_KEY_K: return KEY_K;
		case GLFW_KEY_L: return KEY_L;
		case GLFW_KEY_M: return KEY_M;
		case GLFW_KEY_N: return KEY_N;
		case GLFW_KEY_O: return KEY_O;
		case GLFW_KEY_P: return KEY_P;
		case GLFW_KEY_Q: return KEY_Q;
		case GLFW_KEY_R: return KEY_R;
		case GLFW_KEY_S: return KEY_S;
		case GLFW_KEY_T: return KEY_T;
		case GLFW_KEY_U: return KEY_U;
		case GLFW_KEY_V: return KEY_V;
		case GLFW_KEY_W: return KEY_W;
		case GLFW_KEY_X: return KEY_X;
		case GLFW_KEY_Y: return KEY_Y;
		case GLFW_KEY_Z: return KEY_Z;
		case GLFW_KEY_SPACE: return KEY_SPACE;
		case GLFW_KEY_UP: return KEY_UP;
		case GLFW_KEY_RIGHT: return KEY_RIGHT;
		case GLFW_KEY_DOWN: return KEY_DOWN;
		case GLFW_KEY_LEFT: return KEY_LEFT;
		default: return 0;
	}
}


/*******************************************************/

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
    int height;
    Cell(unsigned int _texture_id = 0, unsigned long long int _attributes = 0, int _height = 0)
    {
        attributes = _attributes;
        texture_id = _texture_id;
		height = _height;
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