#include <stdio.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>

#define TEXTURE_TOTAL_WIDTH  1024
#define TEXTURE_TOTAL_HEIGHT 1024
#define TEXTURE_FILE "texture.data"
#define START_TEXTURE glEnable(GL_TEXTURE_2D);
#define END_TEXTURE glDisable(GL_TEXTURE_2D);
char texture_data[TEXTURE_TOTAL_WIDTH * TEXTURE_TOTAL_HEIGHT * 4];
GLuint _TEXTURE_id;
GLFWwindow* window;
bool KEY_LEFT   = 0;
bool KEY_UP     = 0;
bool KEY_RIGHT  = 0;
bool KEY_DOWN   = 0;

void read_texture()
{
    FILE *f = fopen(TEXTURE_FILE, "rb");
    if (f==NULL) { exit (1); }
    fseek(f, 0, SEEK_END);
    fseek(f, 0, SEEK_SET);
    fread(texture_data, TEXTURE_TOTAL_WIDTH * TEXTURE_TOTAL_HEIGHT * 4, 1, f);
    fclose(f);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
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
            break;
            case GLFW_KEY_DOWN:
            KEY_DOWN = true;
            break;
            case GLFW_KEY_LEFT:
            KEY_LEFT = true;
            break;
            case GLFW_KEY_RIGHT:
            KEY_RIGHT = true;
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

static void resize_callback(GLFWwindow* window, int width, int height)
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
#include "game.h"

int main()
{
    int _width = 640;
    int _height = 360;
    
    if (!glfwInit())
        exit(EXIT_FAILURE);
    
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    //window = glfwCreateWindow(_width, _height, "", glfwGetPrimaryMonitor(), NULL);
    window = glfwCreateWindow(_width, _height, "", NULL, NULL);
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
    glfwSetFramebufferSizeCallback(window, resize_callback);
    
    float bar_percent = 0.5;
    float bar_reverse_percent = 0.0;
    bool up = false;
    bool up2 = false;
    double vx = 0, vy = 0;
    double velocity = 2.7;
    Map m;
    
    for (int j = 0; j < MAP_WIDTH; j++)
        for (int k = 0; k < MAP_HEIGHT; k++)
        {
            if (j == 0 || j == MAP_WIDTH - 1 || k == 0 || k == MAP_HEIGHT - 1)
                m.cells[k][j].texture_id = 1;
            else
                m.cells[k][j].texture_id = 2;
        }
    
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        
        if (KEY_UP)
        {
            vy -= velocity;
        }
        if (KEY_DOWN)
        {
            vy += velocity;
        }
        if (KEY_LEFT)
        {
            vx -= velocity;
        }
        if (KEY_RIGHT)
        {
            vx += velocity;
        }
        
        m.draw(vx, vy);
        
        draw_hp_bar(5, 5, 300, bar_percent, bar_reverse_percent);
    
        if (bar_percent >= 1.0)
            up = !up;
        else if (bar_percent <= 0.0)
            up = !up;
        if (up)
            bar_percent+=0.01;
        else
            bar_percent-=0.01;
    
        if (bar_reverse_percent >= 1.0)
            up2 = !up2;
        else if (bar_reverse_percent <= 0.0)
            up2 = !up2;
        if (up2)
            bar_reverse_percent+=0.01;
        else
            bar_reverse_percent-=0.01;
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    
    exit(EXIT_SUCCESS);
}