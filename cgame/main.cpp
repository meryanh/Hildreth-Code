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
    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);
    glVertex2i(bar_start_pos_x+4, bar_start_pos_y+1);
    glVertex2i(bar_start_pos_x+bar_end+3, bar_start_pos_y+1);
    glVertex2i(bar_start_pos_x+bar_end-1, bar_start_pos_y+4);
    glVertex2i(bar_start_pos_x+2, bar_start_pos_y+4);
    glEnd();
}

void draw_hp_reverse_overlay(int bar_start_pos_x, int bar_start_pos_y, int bar_length, float bar_percent, float bar_reverse_percent, unsigned char r = 0xFF, unsigned char g = 0xFF, unsigned char b = 0xFF)
{
    int bar_end = bar_percent*bar_length;
    if (bar_end <= 2)
        return;
    else if (bar_end > bar_length)
        bar_end = bar_length;
    
    int bar_reverse_start = bar_end - (bar_reverse_percent*bar_length);
    if (bar_reverse_start <= bar_start_pos_x)
        bar_reverse_start = bar_start_pos_x;
    glColor3ub(r, g, b);
    glBegin(GL_POLYGON);
    glVertex2i(bar_start_pos_x+bar_reverse_start+4, bar_start_pos_y+1);
    glVertex2i(bar_start_pos_x+bar_end+3, bar_start_pos_y+1);
    glVertex2i(bar_start_pos_x+bar_end-1, bar_start_pos_y+4);
    glVertex2i(bar_start_pos_x+bar_reverse_start+2, bar_start_pos_y+4);
    glEnd();
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

int main()
{
    int _width = 640;
    int _height = 480;
    
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
    bool up = false;
    
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glColor3ub(0xFF, 0xFF, 0xFF);
       
        START_TEXTURE
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 40; j++)
                for (int k = 0; k < 30; k++)
                {
                    if ((k+j)%2){
                        draw_texture_segment(1, j*16, k*16);
                    }
                    else
                    {
                        draw_texture_segment(2, j*16, k*16);
                    }
                }
        END_TEXTURE
        
        draw_hp_bar(5, 5, 300, bar_percent, 0x9A, 0x00, 0x00);
        draw_hp_reverse_overlay(5, 5, 300, bar_percent, 0.2, 0x00, 0x00, 0x00);
    
        if (bar_percent >= 1.0)
            up = !up;
        else if (bar_percent <= 0.0)
            up = !up;
        if (up)
            bar_percent+=0.001;
        else
            bar_percent-=0.001;
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    
    exit(EXIT_SUCCESS);
}