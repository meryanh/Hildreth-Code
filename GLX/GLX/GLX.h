#ifndef GLX_INCLUDE_H
#define GLX_INCLUDE_H

// Include OpenGL
#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/glut.h>
#endif

// Required libraries:
#include <cstdlib>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

// Include the source files:
using namespace std;
#include "global.h"
#include "Rect.cpp"
#include "Color.cpp"
#include "Function.cpp"
#include "Window.cpp"
#include "var.cpp"
#include "Control.cpp"
#include "XControl.cpp"

#define SSTR(x) dynamic_cast<ostringstream&>((ostringstream() << dec << x)).str()

// UI definition macros:
#define BEGIN Control* root = X.get_root(); Control* current = root;
#define ADD(name) current = current->add(#name);
#define SET(attr, ...) current->attr (__VA_ARGS__);
#define END if (current != NULL && current == root) { current = NULL; } else if (current != NULL) current = current->get_parent();

#include "../control_types.h"

// Redirect main to the correct entry point:
int main(int argc, char** argv) { X(&argc, argv); }

#endif