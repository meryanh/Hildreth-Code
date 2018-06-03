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
int window_center_x = 320;
int window_center_y = 180;
bool KEY_LEFT_DOWN  = 0;
bool KEY_UP_DOWN    = 0;
bool KEY_RIGHT_DOWN = 0;
bool KEY_DOWN_DOWN  = 0;
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

bool read_texture()
{
    FILE *f = fopen(TEXTURE_FILE, "rb");
    if (f==NULL) return false;
    fseek(f, 0, SEEK_END);
    fseek(f, 0, SEEK_SET);
    fread(texture_data, TEXTURE_TOTAL_WIDTH * TEXTURE_TOTAL_HEIGHT * 4, 1, f);
    fclose(f);
	return true;
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
    glfwGetWindowSize(window, &window_width, &window_height);
	window_center_x = window_width / 2;
	window_center_y = window_height / 2;
}

#include "game.h"
// TEMP:
double vx = 1, vy = 1, vz = 0;
double mouse_x = window_center_x, mouse_y = window_center_y;
double velocity = 1.5;
unsigned long long int keys_down = 0;
const int tile_width = 30, tile_height = 30, map_height = 100, map_width = 100;
int jump_dist = 50;

#define FLOOR_NONE -512
int floor_map[map_width][map_height];

int keyIsDown(unsigned long long int key){
	return key & keys_down;
}

int heightAt(int x, int y){
	int posx = x / tile_width;
	int posy = y / tile_height;
	if (floor_map[posy][posx] > 10) 
		throw 1;
	return floor_map[posy][posx];
}

bool notIsBlocked(int x, int y, int z){
	if (x < 0 || y < 0)
		return true;
	int posx = x / tile_width;
	int posy = y / tile_height;
	int fv = floor_map[posy][posx];
	return posx >= 0 && posy >= 0 && fv <= vz + 1;
}

bool isValidPos(int x, int y, int z){
	if (x < 0 || y < 0)
		return false;
	int posx = x / tile_width;
	int posy = y / tile_height;
	int fv = floor_map[posy][posx];
	return posx >= 0 && posy >= 0 && fv != FLOOR_NONE && fv <= vz + 1;
}

void tryMove(float x, float y){
	if (vx + x < 0 || vy + y < 0 || vx + x > map_width * tile_width || vy + y > map_height * tile_height)
		return;
	int posx = (vx + x) / tile_width;
	int posy = (vy + y) / tile_height;
	int fv = floor_map[posy][posx];
	if (posx >= 0 && posy >= 0 && fv != FLOOR_NONE && fv <= vz + 1){
		vx += x;
		vy += y;
		vz = fv;
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	unsigned long long int keyAt = GLFWKeyToFlag(key);
    if (action == GLFW_PRESS)
    {
		keys_down |= keyAt;
        if(key == GLFW_KEY_ESCAPE){
            glfwSetWindowShouldClose(window, GL_TRUE);
		}
		else if (keyAt == KEY_SPACE){
            double dx = (mouse_x - window_center_x);
            double dy = (mouse_y - window_center_y);
			double r = sqrt(dx * dx + dy * dy);
			double jc = r > jump_dist ? jump_dist : r;
				
			if (isValidPos(vx + jc * (dx / r), vy + jc * (dy / r), vz)){
				tryMove(jc * (dx / r), jc * (dy / r));
			} else {
				for (int i = 0; i < jc; i++){
					tryMove(dx / r, 0);
					tryMove(0, dy / r);
				}
			}
		}
    }
	
    else if (action == GLFW_RELEASE)
    {
		keys_down &= ~keyAt;
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

int abs(int v){
	return v < 0 ? -v : v;
}

int min(int a, int b){
	return a < b ? a : b;
}

int max(int a, int b){
	return a < b ? b : a;
}

int map_posx(){
	return vx / tile_width;
}

int map_posy(){
	return vy / tile_height;
}

void color_value(float r, float g, float b, float v){
	glColor3ub(max(0, r * v), max(0, g * v), max(0, b * v));
}

bool drawFloorSegment(int x, int y, int l){
	int f_y = y * tile_height - vy + window_center_y - l * 5  + vz * 5;
	int f_x = x * tile_width - vx + window_center_x;
	if (f_y < window_height && f_x < window_width){
		float dw = 1 / (((abs(map_posx() - x) + abs(map_posy() - y)) / 10.0) + 1);
		float dp = 1 / (((abs(map_posx() - x) + abs(map_posy() - y) - (vz - floor_map[y][x] < 0 ? vz - floor_map[y][x] : 0)) / 10.0) + 1);

		color_value(0x10, 0x15, 0x25, dw);
		glBegin(GL_POLYGON);
		glVertex2i(f_x, window_height);
		glVertex2i(f_x, f_y + tile_height);
		glVertex2i(f_x + tile_width, f_y + tile_height);
		glVertex2i(f_x + tile_width, window_height);
		glEnd();
		
		color_value(0xFF, 0xFF, 0xFF, dp);
		glEnable(GL_TEXTURE_2D);
		draw_texture_segment(0, f_x, f_y, 30);
		glDisable(GL_TEXTURE_2D);

		// color_value(0x00, 0x95, 0x75, dp);
		// glBegin(GL_POLYGON);
		// glVertex2i(f_x, f_y + tile_height);
		// glVertex2i(f_x, f_y);
		// glVertex2i(f_x + tile_width, f_y);
		// glVertex2i(f_x + tile_width, f_y + tile_height);
		// glEnd();
		
		// glColor3ub(0x00, 0x00, 0x00);
		// glBegin(GL_LINE_LOOP);
		// glVertex2i(f_x, f_y + tile_height);
		// glVertex2i(f_x, f_y);
		// glVertex2i(f_x + tile_width, f_y);
		// glVertex2i(f_x + tile_width, f_y + tile_height);
		// glEnd();
		return true;
	} else return false;
}

void rayCastLine(int r, int ox, int oy, int dx, int dy, int *outx, int *outy){
	if (ox == dx && oy == dy){
		(*outx) = ox;
		(*outy) = oy;
		return;
	}
	double vdx = (dx - ox);
	double vdy = (dy - oy);
	double vr = sqrt(vdx * vdx + vdy * vdy);
	
	if (r > vr) r = vr;
	for (int i = 0; i < r; i++){
		if (!notIsBlocked(vx + i * (vdx / vr),vy + i * (vdy / vr),vz)){
			(*outx) = i * (vdx / vr) + ox;
			(*outy) = i * (vdy / vr) + oy;
			return;
		}
	}
	(*outx) = r * (vdx / vr) + ox;
	(*outy) = r * (vdy / vr) + oy;
}

int main()
{
	srand(0xDEADBEEF);
    if (!glfwInit())
        return 1;
    
    //glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    //window = glfwCreateWindow(window_width, window_height, "", glfwGetPrimaryMonitor(), NULL);
    window = glfwCreateWindow(window_width, window_height, "", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return 1;
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
    
    if (!read_texture()) return 1;
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

	// glClearColor(1, 1, 1, 1);
	for (int i = 0; i < map_height; i++){
		for (int j = 0; j < map_width; j++){
			floor_map[i][j] = 0;
		}
	}
	
	floor_map[0][10] = 120;
	floor_map[0][9] = 120;
	floor_map[0][8] = 120;
	floor_map[0][7] = 120;
	floor_map[0][6] = 120;
	floor_map[0][5] = 120;
	floor_map[0][4] = 120;
	floor_map[0][3] = 120;
	floor_map[0][2] = 120;
	floor_map[0][1] = 120;
	floor_map[0][0] = 120;
	
	floor_map[1][5] = 4;
	floor_map[1][4] = 3;
	floor_map[1][3] = 2;
	floor_map[1][2] = 1;
	floor_map[1][0] = 120;
	
	floor_map[2][6] = 4;
	floor_map[2][5] = 4;
	floor_map[2][4] = 4;
	floor_map[2][3] = 0;
	floor_map[2][2] = 0;
	floor_map[2][0] = 120;
	
	floor_map[3][4] = 0;
	floor_map[3][3] = -1;
	floor_map[3][2] = 0;
	floor_map[3][0] = 120;
	
	floor_map[4][0] = 120;
	
	floor_map[5][3] = 0;
	floor_map[5][2] = 0;
	floor_map[5][0] = 120;
	
	floor_map[6][3] = 0;
	floor_map[6][2] = 0;
	
	floor_map[6][0] = 120;
	
	floor_map[7][0] = 120;
	floor_map[6][6] = 0;
	
	floor_map[8][0] = 10;
	
	floor_map[4][4] = 0;
	
	// int vertx[10] = {0};
	// int verty[10] = {0};
	// int vertn = 0;
	vz = floor_map[0][0];
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
		
		// drawpoly(4, vertx, verty);

		
		if (keys_down & KEY_W){
			tryMove(0, -velocity);
		} else if (keys_down & KEY_S){
			tryMove(0, velocity);
		}
		
		if (keys_down & KEY_A){
			tryMove(-velocity, 0);
		} else if (keys_down & KEY_D){
			tryMove(velocity, 0);
		}
		
		int mposy = map_posy();
		
		bool entity_drawn = 0;
		for (int i = 0; i < map_height; i++){
			for (int j = 0; j < map_width; j++){
				if (floor_map[i][j] != FLOOR_NONE)
					if (!drawFloorSegment(j, i, floor_map[i][j]))
						break;
			}
			if (!entity_drawn && i == mposy){
				
				// glColor4ub(0xFF, 0xFF, 0xFF, 0x50);
				// vertx[vertn] = vx;
				// verty[vertn] = vy;
				// vertn = (vertn + 1) % 10;
				// glBegin(GL_LINE_STRIP);
				// for (int i = vertn + 1; i != vertn; i = (i + 1) % 10){
					// if (i >= 10) continue;
					// vertx[i] += (rand() % 7 - 3);
					// verty[i] += (rand() % 7 - 5);
					// glVertex2i(vertx[i] - vx + window_center_x, verty[i] - vy + window_center_y);
				// }
				// glEnd();
				
				glColor3ub(0xFF, 0xFF, 0xFF);
				glBegin(GL_POLYGON);
				glVertex2i(window_center_x - 2, window_center_y - 4);
				glVertex2i(window_center_x - 2, window_center_y);
				glVertex2i(window_center_x + 2, window_center_y);
				glVertex2i(window_center_x + 2, window_center_y - 4);
				glEnd();
				entity_drawn = true;
			}
		}
		
			double dx = (mouse_x - window_center_x);
			double dy = (mouse_y - window_center_y);
			double r = sqrt(dx * dx + dy * dy);
		
		if (!(MOUSE_LEFT_DOWN)){
			
			glColor4ub(0xFF, 0xFF, 0xFF, 0x20);
			
			// int rcposx = vx, rcposy = vy;
			// rayCastLine(jump_dist, window_center_x, window_center_y, mouse_x, mouse_y, &rcposx, &rcposy);
			
			// glBegin(GL_LINES);
			// glVertex2i(window_center_x, window_center_y);
			// glVertex2i(rcposx, rcposy);
			// glEnd();
			
			
			// glBegin(GL_LINE_LOOP);
			// glVertex2i(rcposx - 3, rcposy);
			// glVertex2i(rcposx, rcposy + 3);
			// glVertex2i(rcposx + 3, rcposy);
			// glVertex2i(rcposx, rcposy - 3);
			// glEnd();
			
			if (r > jump_dist){
				int cx = window_center_x + jump_dist * (dx / r);
				int cy = window_center_y + jump_dist * (dy / r);
				
				glBegin(GL_LINES);
				glVertex2i(window_center_x, window_center_y);
				glVertex2i(cx, cy);
				glEnd();
			
				glColor4ub(0xFF, 0xFF, 0xFF, 0x70);
				glBegin(GL_LINE_LOOP);
				glVertex2i(cx - 3, cy);
				glVertex2i(cx, cy + 3);
				glVertex2i(cx + 3, cy);
				glVertex2i(cx, cy - 3);
				glEnd();
			} else {
				glBegin(GL_LINES);
				glVertex2i(window_center_x, window_center_y);
				glVertex2i(mouse_x, mouse_y);
				glEnd();
			}
		}
		
		// CURSOR
		glColor3ub(0xFF, 0xFF, 0xFF);
		glBegin(GL_LINES);
		glVertex2i(mouse_x - 6, mouse_y);
		glVertex2i(mouse_x - 3, mouse_y);
		glVertex2i(mouse_x + 3, mouse_y);
		glVertex2i(mouse_x + 6, mouse_y);
		glVertex2i(mouse_x, mouse_y - 6);
		glVertex2i(mouse_x, mouse_y - 3);
		glVertex2i(mouse_x, mouse_y + 3);
		glVertex2i(mouse_x, mouse_y + 6);
		glEnd();
        
		
		if (MOUSE_LEFT_DOWN){
		
			glColor4ub(0xFF, 0xFF, 0xFF, 0x70);
			int hasCollision = false;
			glBegin(GL_LINE_STRIP);
			for (int i = 2; i < 50; i++){
				int tx = i * 4 * (dx / r) + window_center_x;
				int ty = i * 4 * (dy / r) + window_center_y;
				if (!notIsBlocked(vx + i * 4 * (dx / r),vy + i * 4 * (dy / r),vz)){
					glVertex2i(tx, ty);
					glEnd();
					glBegin(GL_LINE_LOOP);
					glVertex2i(tx - 4 + rand() % 13 - 7, ty + rand() % 13 - 7);
					glVertex2i(tx + rand() % 13 - 7, ty + 4 + rand() % 13 - 7);
					glVertex2i(tx + 4 + rand() % 13 - 7, ty + rand() % 13 - 7);
					glVertex2i(tx + rand() % 13 - 7, ty - 4 + rand() % 13 - 7);
					glEnd();
					hasCollision = true;
					break;
				}
				glVertex2i(tx + rand() % 9 - 4, ty + rand() % 9 - 4);
			}
			if (!hasCollision)
				glEnd();
		}
		
        // draw_hp_bar(5, 5, 300, 0.5, 0.25);
        
		// CURSOR:
		// glBegin(GL_POLYGON);
		// glVertex2i(mouse_x, mouse_y);
		// glVertex2i(mouse_x + 6, mouse_y + 6);
		// glVertex2i(mouse_x, mouse_y + 12);
		// glEnd();
		
        // texture_data[rand()%(TEXTURE_TOTAL_WIDTH * TEXTURE_TOTAL_HEIGHT * 4)] = 0;
        // glDeleteTextures(1, &_TEXTURE_id);
        // glGenTextures(1, &_TEXTURE_id);
        // glBindTexture(GL_TEXTURE_2D, _TEXTURE_id);
        // glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, TEXTURE_TOTAL_WIDTH, TEXTURE_TOTAL_HEIGHT,0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    
    return 0;
}