/* loop.h (c) 2013 R. Díaz de León P.
 * Lightstar simulator
 */

#ifndef _LOOP_HEADER_

#define _LOOP_HEADER_

#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

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
