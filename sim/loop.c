/* loop.c (c) 2013 R. Díaz de León P.
 * Lightstar simulator
 */
 
#include "loop.h"
#include "settings.h"

/* Needed for draw scene */
static struct Point *points;

void 
resizeWindow(int width, int height)
{
	GLfloat ratio;

	/* Avoid division by zero */
	if (height == 0){
		height = 1;
	}

	ratio = (GLfloat) width / (GLfloat) height;
	glViewport(0, 0, (GLsizei) width, (GLsizei) height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/* Here goes our drawing code */
static struct Point*
createSemicircle(const int npoints, const int radius)
{
	int i;
	float arc = 0, rad = 0, pos = 0;
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

static void
drawScene()
{
	int i = 0;
	static float spin = 0;
	
	/* Clear The Screen And The Depth Buffer */
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	/* Move left 1.5 units and into the screen 20.0 */
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -20.0f);
	
	spin += 2.0f;
	glRotatef(spin, 0.0f, 1.0f, 0.0f);

	glPointSize(3.0f);
	for (i = 0; i < NPOINTS; i++) {	
		struct Point p = points[i];
		glBegin( GL_POINTS );
		glColor3f(p.color.r,  p.color.g,  p.color.b );
		glVertex3f(p.y, p.x,  0.0f); /* Top Of Triangle */
		/* Finished Drawing The Triangle */
		glEnd();
	}

	glFlush();
 	glutSwapBuffers();
}

static void
keydown(unsigned char key, int x, int y)
{
	switch (key) {
		case 'q':
			exit(0);
			break;
	}
}

void
loop()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	glutCreateWindow("Lightstar The Movie The Game X Turbo POV Simulation");
	glutReshapeFunc(resizeWindow);
	glutDisplayFunc(drawScene);	
	glutKeyboardFunc(keydown);
	glutIdleFunc(glutPostRedisplay);
	glEnable(GL_DEPTH_TEST);

	/* points is static to the file */
	points = createSemicircle(NPOINTS, 5);
	glutMainLoop();
	destroySemicircle(points);
}
