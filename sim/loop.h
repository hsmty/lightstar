/* loop.h (c) 2013 R. Díaz de León P.
 * Lightstar simulator
 */

#ifndef _LOOP_HEADER_
#define _LOOP_HEADER_

#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <string.h>
#include <X11/xpm.h>

#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else
	#include <GL/gl.h>
	#include <GL/glu.h>
	#include <GL/glut.h>
#endif

struct Color {
	float r;
	float g;
	float b;
};

struct Point {
	float x;
	float y;
	float z;
	struct Color color;
};

void loop(char*);

#endif
