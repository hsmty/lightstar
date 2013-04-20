/* loop.c (c) 2013 R. Díaz de León P.
 * Lightstar simulator
 */
 
#include "loop.h"
#include "settings.h"

/* Needed for draw scene */
static struct Point** points;
int** rgb;

int** rgbArray ( char* filename );
XpmImage* loadimage ( char* filename );


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
static struct Point**
createSemicircle(const int npoints_h, const int npoints_v)
{
	int i, j, initial_x = 0, initial_y = 5;
	float arc = 0, arc2 = 0, rad = 0, deg = 0, tetha = 0;
	float temp_x = 0, temp_y = 0;	

	printf("Creating semicircle\n");

	points = (struct Point**) malloc(npoints_h * sizeof(struct Point*));
	for (i = 0; i < npoints_h; i++){
		 points[i] = (struct Point*) malloc(npoints_v * sizeof(struct Point));
	} 
		
	if (points == NULL) {
		/* Cannot allocate memory */
		return points;
	}
	

	/* Size of the arc between points */
	arc = 180.0f / npoints_v;
	arc2 = 360.0f /  npoints_h;

	for (i = 0; i < npoints_h; i++) {
		tetha = (arc2 * i) * 3.14159/180;
		for (j = 0; j < npoints_v; j++) {
	 		deg = j * arc;
			/* From pos in degrees to radians */
			rad = deg * 3.14159/180;
			/* 2D rotation un plane xy */
			temp_x = (initial_x * cos(rad)) - (initial_y * sin(rad));
			temp_y = (initial_x * sin(rad)) + (initial_y * cos(rad));
			/* Rotate about y axis with z=0 */
			points[i][j].x = temp_x * cos(tetha); 
			points[i][j].y = temp_y; 
			points[i][j].z = temp_x * sin(tetha);		
		/*	
			points[i][j].color.r = 255;
        	points[i][j].color.g = 255; 
       	 	points[i][j].color.b = 255;
	*/
		}
		
	}

	/*get image RGB values*/
	rgb = rgbArray("world.xpm"); 
	
	/* Set colors to RGB array */
	for (i = 0; i < npoints_h; i++) {
    	for (j = 0; j < npoints_v; j++){
 			points[i][j].color.r = rgb[i+j][0];
        	points[i][j].color.g = rgb[i+j][1]; 
        	points[i][j].color.b = rgb[i+j][2];
		}
	}
	
	return points;
}

static void
destroySemicircle(struct Point** points, const int npoints_h)
{
	int i = 0;
	if (points != NULL) {
		for (i = 0; i < npoints_h; i++){
			free(points[i]);         
    	}	
		free(points);
	}
		
	for (i = 0; i < 3; i++){
		free(rgb[i]);
    }
    free(rgb);
	
}

static void
drawScene()
{
	int i = 0, j = 0;
	static int spin = 0;

	/* Clear The Screen And The Depth Buffer */
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	/* Move left 1.5 units and into the screen 20.0 */
	glLoadIdentity();

	glTranslatef(0.0f, 0.0f, -20.0f);

	glRotatef(spin, 0.0f, 1.0f, 0.0f);
	
    glPointSize(3.0f);
    for (i = 0; i < NPOINTS_H; i++) {
        for (j = 0; j < NPOINTS_V; j++) {
            struct Point p = points[i][j];
            glBegin( GL_POINTS );
            glColor3f(p.color.r, p.color.g, p.color.b);
            glVertex3f(p.x, p.y, p.z);
            glEnd();
        }
    }

    spin++;
	
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
	points = createSemicircle(NPOINTS_H, NPOINTS_V);
	glutMainLoop();
	destroySemicircle(points, NPOINTS_H);
}

XpmImage*
loadimage(char* filename)
{
    int size;
    XpmImage* img = NULL;
    FILE* file = fopen(filename, "r");

    if (file == NULL) {
        /* Couldn't open the image, send and empty XpmImage */
        return img;
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fclose(file);

    img = malloc(sizeof(XpmImage) + size);

    if (XpmReadFileToXpmImage(filename, img, NULL) == XpmSuccess) {
        return img;
    }

    return (XpmImage*) NULL;
}

int**
rgbArray( char* filename )
{
    int i = 0, j = 0;
    XpmImage* img = loadimage(filename);
    int num_pixels = (img->width) * (img->height);
    rgb = malloc(num_pixels*sizeof(int));
    for (i=0;i<num_pixels; i++){
        rgb[i] = malloc(3*sizeof(int));
    }

    for(i = 0; i < num_pixels; i++){
        XpmColor pixel = img->colorTable[img->data[i]];
        char* tmp = pixel.c_color;
        tmp++;

        /*Hex color code to decimal RGB*/
        for(j = 0; j < 3; j++){
            char hex[4] = "0x";
            char dec[4] = "";
            int t = 0;

            strncat(hex, tmp, 2);
            t = strtol(hex, NULL, 16);
            sprintf(dec, "%d", t);
            rgb[i][j]= atoi(dec);

            if(j != 2) tmp+=2;
        }
    }

    return rgb;
}
