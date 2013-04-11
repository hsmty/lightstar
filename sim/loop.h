/* loop.h (c) 2013 R. Díaz de León P.
 * Lightstar simulator
 */

#ifndef _LOOP_HEADER_
#define _LOOP_HEADER_

#include <stdio.h>
#include <stdlib.h>
#include <math.h> 

#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

#include <SDL.h>

struct Color {
	float r;
	float g;
	float b;
};

struct Point {
	float x;
	float y;
	struct Color color;
};

extern void Quit(int ret);
extern int loop();

#endif
