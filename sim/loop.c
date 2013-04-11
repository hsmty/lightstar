/* loop.c (c) 2013 R. Díaz de León P.
 * Lightstar simulator
 */
 
#include "loop.h"
#include "settings.h"

/* This is our SDL surface */
SDL_Surface *surface;

extern void
Quit(int ret)
{
    /* clean up the window */
    SDL_Quit();
    exit(ret);
}

static int
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
static void
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

static int
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
static struct Point*
createSemicircle(const int npoints, const int radius)
{
	int i, pos = 0;
	float arc = 0, rad = 0;
	struct Point *points;

	printf("Creating semicircle\n");

	points = (struct Point*) malloc(npoints * sizeof(struct Point));

	if (points == NULL) {
		/* Cannot allocate memory */
		return points;
	}

	/* Size of the arc between points */
	arc = 180.0f / npoints;

	for (i = 0; i < npoints; i++) {
		pos = i * arc;
		/* From pos in degrees to radians */
		rad = ((float) pos) * 3.14159/180;
		points[i].x = cos(rad) * radius;
		points[i].y = sin(rad) * radius;
		/* Set color to white */
		points[i].color.r = 1;
		points[i].color.g = 1;
		points[i].color.b = 1;
	}

	return points;
}

static void
destroySemicircle(struct Point* points)
{
	if (points != NULL) {
		free(points);
	}
}

static int
drawScene(struct Point* points)
{
	int i = 0;
	static float spin = 0;
	
	/* Clear The Screen And The Depth Buffer */
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	/* Move left 1.5 units and into the screen 20.0 */
	glLoadIdentity();
	glTranslatef(-1.5f, 0.0f, -20.0f);
	
	/* Starts to spin */ 
	spin += 2.0f;
	glRotatef(spin, 0.0f, 1.0f, 0.0f);

	glPointSize(3.0f);
	for (i = 0; i < NPOINTS; i++) {	
		/* Drawing Using Triangles */
		struct Point p = points[i];
		glBegin( GL_POINTS );
		glColor3f(p.color.r,  p.color.g,  p.color.b );
		glVertex3f(p.y, p.x,  0.0f); /* Top Of Triangle */
		/* Finished Drawing The Triangle */
		glEnd();
	}

	/* Draw it to the screen */
	SDL_GL_SwapBuffers();

	return TRUE;
}

extern int
loop()
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
	struct Point *points;

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

	points = createSemicircle(NPOINTS, 5); 

	if (points == NULL) {
		fprintf(stderr, "Can't allocate memory for points.\n");
		Quit(1);
	}

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
			drawScene(points);
		}
	}

	destroySemicircle(points);

	return 0;
}
