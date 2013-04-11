/* loop.c (c) 2013 R. Díaz de León P.
 * Lightstar simulator
 */
 
#include "loop.h"
#include "settings.h"

void 
resizeWindow(int width, int height)
{
    GLfloat ratio;

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

void 
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
			points[i].color->g = 1;
			points[i].color->b = 1;
		}
	}

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -20.0f);
	
	spin += 2.0f;
	glRotatef(spin, 0.0f, 1.0f, 0.0f);

	glPointSize(3.0f);
	for (i = 0; i < NPOINTS; i++) {	
		struct Point p = points[i];
		glBegin( GL_POINTS );
			glColor3f(p.color->r,  p.color->g,  p.color->b );
			glVertex3f(p.y, p.x,  0.0f);
		glEnd();
	}

	glFlush();
 	glutSwapBuffers();
}

void
loop()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow("Lightstar The Movie The Game X Turbo POV Simulation");
	glutReshapeFunc(resizeWindow);
	glutDisplayFunc(drawGLScene);	
	glutIdleFunc(glutPostRedisplay);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
}
