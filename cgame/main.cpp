#include <cstdio>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <cmath>

#define TEXTURE_TOTAL_WIDTH  1024
#define TEXTURE_TOTAL_HEIGHT 1024
#define TEXTURE_FILE "texture.data"
char texture_data[TEXTURE_TOTAL_WIDTH * TEXTURE_TOTAL_HEIGHT * 4];
GLuint _TEXTURE_id;
GLFWwindow* window;
int window_width = 640;
int window_height = 360;
bool KEY_LEFT  = 0;
bool KEY_UP    = 0;
bool KEY_RIGHT = 0;
bool KEY_DOWN  = 0;
bool MOUSE_LEFT_DOWN   = 0;
bool MOUSE_RIGHT_DOWN  = 0;
bool MOUSE_MIDDLE_DOWN = 0;

void resize_callback(GLFWwindow* window, int width, int height)
{
    glPopMatrix();
    glPushMatrix();
    glViewport(0, 0, width, height);
    glOrtho(0, width, height, 0, 0, 1);
}

void draw_texture_segment(int segment, int x, int y, int size = 16)
{
    float segment_size = ((float)size)/((float)TEXTURE_TOTAL_WIDTH);
    float segment_start_x = (float)((segment * size) % TEXTURE_TOTAL_WIDTH)/((float)TEXTURE_TOTAL_WIDTH);
    float segment_start_y = (float)((segment * size) / TEXTURE_TOTAL_WIDTH)/((float)TEXTURE_TOTAL_WIDTH);

    glBegin(GL_POLYGON);
    glTexCoord2f(segment_start_x, segment_start_y);
    glVertex2i(x, y);
    glTexCoord2f(segment_start_x, segment_start_y+segment_size);
    glVertex2i(x, y+size);
    glTexCoord2f(segment_start_x+segment_size, segment_start_y+segment_size);
    glVertex2i(x+size, y+size);
    glTexCoord2f(segment_start_x+segment_size, segment_start_y);
    glVertex2i(x+size, y);
    glEnd();
}

void read_texture()
{
    FILE *f = fopen(TEXTURE_FILE, "rb");
    if (f==NULL) { exit (1); }
    fseek(f, 0, SEEK_END);
    fseek(f, 0, SEEK_SET);
    fread(texture_data, TEXTURE_TOTAL_WIDTH * TEXTURE_TOTAL_HEIGHT * 4, 1, f);
    fclose(f);
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
    glfwGetWindowSize(window, &window_width, &window_height);
}

#include "game.h"
// TEMP:
double vx = 50, vy = 50;
double mouse_x = 0, mouse_y = 0;
double velocity = 2.0;
int dash_cooldown = 60;
int dash_count = 99999;
int max_dash = 5;
Map m;

void dash()
{
    if (dash_count <= 0)
        return;
    
    double dx = (mouse_x - (window_width / 2));
    double dy = (mouse_y - (window_height / 2));
    double r = sqrt(dx * dx + dy * dy);
    for (int i = 0; i < 50; i++)
    {
        vx += velocity * (dx / r);
        if (m.cell[(int)(vy) / 16][(int)(vx - 5) / 16].attributes & CV_BLOCKED)
            vx = ((int)vx / 16) * 16 + 5;
        else if (m.cell[(int)(vy) / 16][(int)(vx + 5) / 16].attributes & CV_BLOCKED)
            vx = (((int)vx + 15) / 16) * 16 - 5;
        
        vy += velocity * (dy / r);
        if (m.cell[(int)(vy - 5) / 16][(int)(vx - 5) / 16].attributes & CV_BLOCKED)
            vy = ((int)vy / 16) * 16 + 5;
        else if (m.cell[(int)(vy + 5) / 16][(int)(vx) / 16].attributes & CV_BLOCKED)
            vy = (((int)vy + 15) / 16) * 16 - 5;
    }
    
    dash_count--;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
            case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
            case GLFW_KEY_UP:
            KEY_UP = true;
            KEY_DOWN = false;
            break;
            case GLFW_KEY_DOWN:
            KEY_DOWN = true;
            KEY_UP = false;
            break;
            case GLFW_KEY_LEFT:
            KEY_LEFT = true;
            KEY_RIGHT = false;
            break;
            case GLFW_KEY_RIGHT:
            KEY_RIGHT = true;
            KEY_LEFT = false;
            break;
        }
    }
    else if (action == GLFW_RELEASE)
    {
        switch (key)
        {
            case GLFW_KEY_UP:
            KEY_UP = false;
            break;
            case GLFW_KEY_DOWN:
            KEY_DOWN = false;
            break;
            case GLFW_KEY_LEFT:
            KEY_LEFT = false;
            break;
            case GLFW_KEY_RIGHT:
            KEY_RIGHT = false;
            break;
        }
    }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    mouse_x = xpos;
    mouse_y = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        switch (button)
        {
            case GLFW_MOUSE_BUTTON_LEFT:
            MOUSE_LEFT_DOWN = true;
            break;
            case GLFW_MOUSE_BUTTON_RIGHT:
            MOUSE_RIGHT_DOWN = true;
            dash();
            break;
        }
    }
    if (action == GLFW_RELEASE)
    {
        switch (button)
        {
            case GLFW_MOUSE_BUTTON_LEFT:
            MOUSE_LEFT_DOWN = false;
            break;
            case GLFW_MOUSE_BUTTON_RIGHT:
            MOUSE_RIGHT_DOWN = false;
            break;
        }
    }
}

int main()
{
    if (!glfwInit())
        exit(EXIT_FAILURE);
    
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    //window = glfwCreateWindow(window_width, window_height, "", glfwGetPrimaryMonitor(), NULL);
    window = glfwCreateWindow(window_width, window_height, "", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glPushMatrix();
    glfwSwapInterval(1);
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glOrtho(0, width, height, 0, 0, 1);
    glDisable(GL_POINT_SMOOTH);
    glDisable(GL_DITHER);
    glDisable(GL_POINT_SMOOTH);
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POLYGON_SMOOTH);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_POINT_SMOOTH, GL_DONT_CARE);
    glHint(GLFW_SAMPLES, 0);
    glHint(GL_LINE_SMOOTH, GL_DONT_CARE);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE);
    
    read_texture();
    glGenTextures(1, &_TEXTURE_id);
    glBindTexture(GL_TEXTURE_2D, _TEXTURE_id);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, TEXTURE_TOTAL_WIDTH, TEXTURE_TOTAL_HEIGHT,0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
   
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetFramebufferSizeCallback(window, resize_callback);
    
    float bar_percent = 0.8;
    float bar_reverse_percent = 0.5;
    
    for (int j = 0; j < MAP_WIDTH; j++)
        for (int k = 0; k < MAP_HEIGHT; k++)
        {
            if (j == 0 || j == MAP_WIDTH - 1 || k == 0 || k == MAP_HEIGHT - 1)
            {
                m.cell[k][j].texture_id = 1;
                m.cell[k][j].attributes |= CV_BLOCKED;
            }
            else
            {
                m.cell[k][j].texture_id = 2;
                m.cell[k][j].attributes &= ~CV_BLOCKED;
            }
        }
    
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        
        if (dash_count != max_dash)
        {
            if (dash_count > max_dash)
                dash_count = max_dash;
            else
            {
                if (dash_cooldown > 0)
                    dash_cooldown--;
                else
                {
                    dash_count++;
                    dash_cooldown = 60;
                }
            }
        }
        
        if (MOUSE_LEFT_DOWN)
        {
            double dx = (mouse_x - (window_width / 2));
            double dy = (mouse_y - (window_height / 2));
            double r = sqrt(dx * dx + dy * dy);
            
            vx += velocity * (dx / r);
            if (m.cell[(int)(vy) / 16][(int)(vx - 5) / 16].attributes & CV_BLOCKED)
                vx = ((int)vx / 16) * 16 + 5;
            else if (m.cell[(int)(vy) / 16][(int)(vx + 5) / 16].attributes & CV_BLOCKED)
                vx = (((int)vx + 15) / 16) * 16 - 5;
            
            vy += velocity * (dy / r);
            if (m.cell[(int)(vy - 5) / 16][(int)(vx - 5) / 16].attributes & CV_BLOCKED)
                vy = ((int)vy / 16) * 16 + 5;
            else if (m.cell[(int)(vy + 5) / 16][(int)(vx) / 16].attributes & CV_BLOCKED)
                vy = (((int)vy + 15) / 16) * 16 - 5;
        }
        
        m.draw(vx, vy);
        
        glBegin(GL_POLYGON);
        glVertex2i(window_width/2 - 5, window_height / 2 - 5);
        glVertex2i(window_width/2 - 5, window_height / 2 + 5);
        glVertex2i(window_width/2 + 5, window_height / 2 + 5);
        glVertex2i(window_width/2 + 5, window_height / 2 - 5);
        glEnd();
        
        draw_hp_bar(5, 5, 300, bar_percent, bar_reverse_percent);
        
        glColor3ub(0xFF, 0xFF, 0xFF);
        for (int i = 0; i < dash_count; i++)
        {
            glBegin(GL_POLYGON);
            glVertex2i(315 + 2 * i * 5, 5);
            glVertex2i(319 + 2 * i * 5, 5);
            glVertex2i(315 + 2 * i * 5, 10);
            glVertex2i(311 + 2 * i * 5, 10);
            glEnd();
        }
        
        //if (!MOUSE_LEFT_DOWN)
        {
            glBegin(GL_POLYGON);
            glVertex2i(mouse_x, mouse_y);
            glVertex2i(mouse_x + 6, mouse_y + 6);
            glVertex2i(mouse_x, mouse_y + 12);
            glEnd();
        }
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    
    exit(EXIT_SUCCESS);
}