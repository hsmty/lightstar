/* simul.c (c) 2013 R. Díaz de León P.
 * Lightstar simulator
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <GL/gl.h>
#include <GL/glu.h>
#include <SDL/SDL.h>

/* screen width, height, and bit depth */
#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP     16

/* Define our booleans */
#define TRUE  1
#define FALSE 0

/* Number of points in the circumference */
#define NPOINTS 86

/* This is our SDL surface */
SDL_Surface *surface;

struct Color {
	float r;
	float g;
	float b;
};

struct Point {
	float x;
	float y;
	struct Color *color;
};

void Quit(int ret);
int resizeWin(int width, int height);

void
Quit(int ret)
{
    /* clean up the window */
    SDL_Quit( );
    exit(ret);
}

int
resizeWin(int width, int height)
{
    /* Height / width ratio */
    GLfloat ratio;

    /* Protect against a divide by zero */
    if (height == 0){
	height = 1;
    }

    ratio = (GLfloat) width / (GLfloat) height;

    glViewport(0, 0, (GLsizei) width, (GLsizei) height);

    /* change to the projection matrix and set our viewing volume. */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity( );

    /* Set our perspective */
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);

    /* Reset The View */
    glLoadIdentity();

    return TRUE;
}

/* function to handle key press events */
void
handleKeyPress(SDL_keysym *key)
{
	switch (key->sym){
		case SDLK_ESCAPE:
			Quit(0);
			break;
		case SDLK_F1:
			SDL_WM_ToggleFullScreen(surface);
			break;
		default:
			break;
	}
}

int
initGL(GLvoid)
{

    /* Enable smooth shading */
    glShadeModel( GL_SMOOTH );

    /* Set the background black */
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    /* Depth buffer setup */
    glClearDepth(1.0f);

    /* Enables Depth Testing */
    glEnable(GL_DEPTH_TEST);

    /* The Type Of Depth Test To Do */
    glDepthFunc(GL_LEQUAL);

    /* Really Nice Perspective Calculations */
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    return( TRUE );
}

/* Here goes our drawing code */
int
drawGLScene( GLvoid )
{
	float pos = 0, radians = 0;
	int radius = 5, i = 0;
	static int init = 0;
	static struct Point *points;
	static float spin = 0, arc = 0;

	/* Get coords semicircle */
	if (init == 0) {
		init = 1;
		printf("Creating semicircle\n");
		arc = 180.0f / NPOINTS; /* Size of the arc between points */
		points = (struct Point*) malloc(NPOINTS * sizeof(struct Point));
		for (i = 0; i < NPOINTS; i++) {
			pos = i * arc;
			radians = ((float) pos) * 3.14159/180;
			points[i].x = cos(radians) * radius;
			points[i].y = sin(radians) * radius;
			points[i].color = (struct Color*) malloc(sizeof(struct Color));
			points[i].color->r = 1;
			points[i].color->g = 0;
			points[i].color->b = 0;
		}
	
	}

	
	/* Clear The Screen And The Depth Buffer */
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	/* Move Left 1.5 Units And Into The Screen 6.0 */
	glLoadIdentity();
	glTranslatef(-1.5f, 0.0f, -20.0f);
	
	/* Starts to spin */ 
	spin += 15.0f;
	glRotatef(spin, 0.0f, 1.0f, 0.0f);

	glPointSize(3.0f);
	for (i = 0; i < NPOINTS; i++) {	
		/* Drawing Using Triangles */
		struct Point p = points[i];
		glBegin( GL_POINTS );
		glColor3f(p.color->r,  p.color->g,  p.color->b );
		glVertex3f(p.y, p.x,  0.0f); /* Top Of Triangle */
		/* Finished Drawing The Triangle */
		glEnd();
	}

	/* Draw it to the screen */
	SDL_GL_SwapBuffers();

	return TRUE;
}

int
main( int argc, char **argv )
{
	/* Flags to pass to SDL_SetVideoMode */
	int videoFlags;
	/* main loop variable */
	int done = FALSE;
	/* used to collect events */
	SDL_Event event;
	/* this holds some info about our display */
	const SDL_VideoInfo *videoInfo;
	/* whether or not the window is active */
	int isActive = TRUE;

	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		fprintf( stderr, "Video initialization failed: %s\n",
				SDL_GetError( ) );
		Quit( 1 );
	}

	/* Fetch the video info */
	videoInfo = SDL_GetVideoInfo( );

	if (!videoInfo) {
		fprintf( stderr, "Video query failed: %s\n",
				SDL_GetError( ) );
		Quit( 1 );
	}

	/* the flags to pass to SDL_SetVideoMode */
	videoFlags  = SDL_OPENGL;          /* Enable OpenGL in SDL */
	videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
	videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */
	videoFlags |= SDL_RESIZABLE;       /* Enable window resizing */

	/* This checks to see if surfaces can be stored in memory */
	if (videoInfo->hw_available) {
		videoFlags |= SDL_HWSURFACE;
	} else {
		videoFlags |= SDL_SWSURFACE;
	}

	/* This checks if hardware blits can be done */
	if ( videoInfo->blit_hw )
		videoFlags |= SDL_HWACCEL;

	/* Sets up OpenGL double buffering */
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	/* get a SDL surface */
	surface = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
			videoFlags );

	/* Verify there is a surface */
	if (!surface) {
		fprintf(stderr,  "Video mode set failed: %s\n", SDL_GetError());
		Quit(1);
	}

	/* initialize OpenGL */
	initGL();

	/* resize the initial window */
	resizeWin(SCREEN_WIDTH, SCREEN_HEIGHT);

	/* wait for events */
	while (!done) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_ACTIVEEVENT:
				/* Something's happend with our focus
				 * If we lost focus or we are iconified,
				 * we shouldn't draw the screen
				 */
				if (event.active.gain == 0)
					isActive = FALSE;
				else
					isActive = TRUE;
				break;			    
			case SDL_VIDEORESIZE:
				/* handle resize event */
				surface = SDL_SetVideoMode(
						event.resize.w,
						event.resize.h,
						16, videoFlags );
				if (!surface) {
					fprintf(stderr, 
							"Could not get a "
							"surface after resize: %s\n", 
							SDL_GetError( ) );
					Quit(1);
				}
				resizeWin( 
						event.resize.w, 
						event.resize.h );
				break;
			case SDL_KEYDOWN:
				/* handle key presses */
				handleKeyPress( &event.key.keysym );
				break;
			case SDL_QUIT:
				/* handle quit requests */
				done = TRUE;
				break;
			default:
				break;
			}
		}

		if (isActive) {
			drawGLScene();
		}
	}

	/* clean ourselves up and exit */
	Quit(0);

	/* Should never get here */
	return 0;
}
