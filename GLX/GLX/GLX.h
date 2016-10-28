#ifndef INCLUDE_H
#define INCLUDE_H

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

#include "../control_types.h"

void parse(string input)
{
    // Parse 'style' as XML
    // Grab content between '<' and '>'
    // If content starts with '/' then go up one level
    // Else, create another control in the current context
    // Text between '>' and '<' is placed in the 'text' attribute of the current control
    // Exit on parse error
}

// HTML parsing macro:
//#define $X(...) void _startup_() { parse(#__VA_ARGS__);}

// Redirect main to the correct entry point:
int main(int argc, char** argv) { X(&argc, argv); }

#endif