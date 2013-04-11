/* simul.c (c) 2013 R. Díaz de León P.
 * Lightstar simulator
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

#include "loop.h"

/* screen width, height, and bit depth */
#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP     16

/* Define our booleans */
#define TRUE  1
#define FALSE 0

/* Number of points in the circumference */
#define NPOINTS 86
