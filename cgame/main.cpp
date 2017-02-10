#include <math.h>
#include <limits.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <sound.h>

#define TEX_WIDTH  16
#define TEX_HEIGHT 16
#include "texture.ctxf"
GLuint _tex_id;
GLFWwindow* window;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

static void resize_callback(GLFWwindow* window, int width, int height)
{
    glPopMatrix();
    glPushMatrix();
    glViewport(0, 0, width, height);
    glOrtho(0, width, height, 0, 0, 1);
}

void draw_hp_bar(int bar_start_pos_x, int bar_start_pos_y, int bar_length, float bar_percent, unsigned char r = 0xFF, unsigned char g = 0xFF, unsigned char b = 0xFF)
{
    int bar_end = bar_percent*bar_length;

    glColor3ub(0x00, 0x00, 0x00);
    glBegin(GL_POLYGON);
    glVertex2i(bar_start_pos_x-3, bar_start_pos_y+3);
    glVertex2i(bar_start_pos_x+1, bar_start_pos_y);
    glVertex2i(bar_start_pos_x+bar_length, bar_start_pos_y);
    glVertex2i(bar_start_pos_x+bar_length+3, bar_start_pos_y+3);
    glVertex2i(bar_start_pos_x+bar_length, bar_start_pos_y+7);
    glVertex2i(bar_start_pos_x, bar_start_pos_y+7);
    glEnd();
    
    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);
    glVertex2i(bar_start_pos_x-2, bar_start_pos_y+3);
    glVertex2i(bar_start_pos_x+1, bar_start_pos_y+1);
    glVertex2i(bar_start_pos_x+bar_end-1, bar_start_pos_y+1);
    glVertex2i(bar_start_pos_x+bar_end+2, bar_start_pos_y+3);
    glVertex2i(bar_start_pos_x+bar_end, bar_start_pos_y+6);
    glVertex2i(bar_start_pos_x+1, bar_start_pos_y+6);
    glEnd();
}

void draw_texture_segment(int segment, int x, int y)
{
    float segment_size = ((float)TEX_HEIGHT)/((float)TEXTURE_TOTAL_HEIGHT);
    float segment_start = ((float)segment)*segment_size;
    glBegin(GL_POLYGON);
    glTexCoord2f(0, segment_start);
    glVertex2i(x, y);
    glTexCoord2f(0, segment_start+segment_size);
    glVertex2i(x, y+TEX_WIDTH);
    glTexCoord2f(1, segment_start+segment_size);
    glVertex2i(x+TEX_HEIGHT, y+TEX_WIDTH);
    glTexCoord2f(1, segment_start);
    glVertex2i(x+TEX_HEIGHT, y);
    glEnd();
}

int main()
{
    int _width = 640;
    int _height = 480;
    
    init_sounds();
    
    if (!glfwInit())
        exit(EXIT_FAILURE);
    
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
    
    glGenTextures(1, &_tex_id);
    glBindTexture(GL_TEXTURE_2D, _tex_id);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TEXTURE_TOTAL_WIDTH, TEXTURE_TOTAL_HEIGHT,0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, resize_callback);

    queue_sound(200, 1000, WV_HALF, WT_SQUARE);
    Sleep(1000);
    queue_sound(200, 1000, WV_HALF, WT_TRIANGLE);
    Sleep(1000);
    queue_sound(200, 1000, WV_HALF, WT_SAWTOOTH);
    Sleep(1000);
    queue_sound(200, 1000, WV_HALF, WT_SINE);
    Sleep(1000);
    queue_sound(200, 1000, WV_HALF, WT_ABS_SINE);
    Sleep(1000);
    queue_sound(400, 1000, WV_HALF, WT_SINE);
    queue_sound(200, 1000, WV_HALF, WT_SINE);

    float bar_percent = 0.5;
    bool up = true;// TEST!
    
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3ub(0xFF, 0xFF, 0xFF);
        
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _tex_id);
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 40; j++)
                for (int k = 0; k < 30; k++)
                {
                    if ((k+j)%2){
                        draw_texture_segment(0, j*15, k*15);
                    }
                    else
                    {
                        draw_texture_segment(1, j*15, k*15);
                    }
                }
        glDisable(GL_TEXTURE_2D);
        
        draw_hp_bar(5, 5, 300, bar_percent, 0x9A, 0x00, 0x00);
    
        if (bar_percent >= 1.0)
            up = !up;
        else if (bar_percent <= 0.0)
            up = !up;
        if (up)
            bar_percent+=0.01;
        else
            bar_percent-=0.01;
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    
    cleanup_sounds();
    exit(EXIT_SUCCESS);
}
