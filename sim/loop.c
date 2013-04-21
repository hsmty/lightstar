/* loop.c (c) 2013 R. Díaz de León P.
 * Lightstar simulator
 */
 
#include "loop.h"
#include "settings.h"

/* Needed for draw scene */
static struct Point** points;
int** rgb;
int the_flag=0;
int iwidth, iheight;

int** rgbArray ( char* filename );
XpmImage* loadimage ( char* filename );

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
    int i = 0, k;
    XpmImage* img = loadimage(filename);
    int num_pixels = (img->width) * (img->height);
    rgb = malloc(num_pixels*sizeof(int));
    for (i=0;i<num_pixels; i++) {
        rgb[i] = malloc(3*sizeof(int));
    }

    for(i = 0; i < num_pixels; i++) {
        XpmColor pixel = img->colorTable[img->data[i]];
        char* tmp = pixel.c_color;
        tmp++;
        sscanf(tmp, "%x", &k);
        rgb[i][0] = ((k & 0xFF0000) >> 16);
        rgb[i][1] = ((k & 0xFF00) >> 8);
        rgb[i][2] = (k & 0xFF);
    }

	iwidth = img->width;
	iheight = img->height;

	XpmFree(img);

    return rgb;
}


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
	gluPerspective(45.0f, ratio, 0.1f, 200.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/* Here goes our drawing code */
static struct Point**
createSemicircle(const int npoints_h, const int npoints_v, char* filename)
{
	int i, j, initial_x = 0, initial_y = 20;
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
	rgb = rgbArray(filename); 
	
	/* Set colors to RGB array */
	j=0;
	int newline=0, k=0;
	for (i = 0; i < npoints_h * npoints_v; i++) {
		if(newline==1  && (j<npoints_v)){j++; newline=0; k=0;}
		points[k][j].color.r = rgb[i][0] / 255.0f;
        points[k][j].color.g = rgb[i][1] / 255.0f;
        points[k][j].color.b = rgb[i][2] / 255.0f;
		k++;
 		if((i%npoints_h == 0)){newline=1;}
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

void
drawScene()
{
	int i = 0, j = 0;
	static int spin = 0;
	int npoints_h=iwidth, npoints_v=iheight;

	/* Clear The Screen And The Depth Buffer */
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	/* Move left 1.5 units and into the screen 20.0 */
	glLoadIdentity();

	if(the_flag == 0){
	glTranslatef(0.0f, 0.0f, -60.0f);

	glRotatef(spin, 0.0f, 1.0f, 0.0f);
	
    glPointSize(3.0f);
    for (i = 0; i < npoints_h; i++) {
        for (j = 0; j < npoints_v; j++) {
            struct Point p = points[i][j];
            glBegin( GL_POINTS );
            glColor3f(p.color.r, p.color.g, p.color.b);
            glVertex3f(p.x, p.y, p.z);
            glEnd();
        }
    }
	}else{
	
	glTranslatef(-1*npoints_h/2, npoints_v/2, -190.0f);

	glPointSize(3.0f);
    for (i = 0; i < npoints_h; i++) {
        for (j = 0; j < npoints_v; j++) {
            struct Point p = points[i][j];
            glBegin( GL_POINTS );
            glColor3f(p.color.r, p.color.g, p.color.b);
            glVertex3f(i, (-1)*j, 0);
            glEnd();
        }
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
		case 's':
			if(the_flag == 1){the_flag = 0;}
			else{the_flag = 1;}
			break;
	}
}

void
loop(char* filename)
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	XpmImage* img = loadimage(filename);	
	
	glutCreateWindow("Lightstar The Movie The Game X Turbo POV Simulation");
	glutReshapeFunc(resizeWindow);
	glutDisplayFunc(drawScene);	
	glutKeyboardFunc(keydown);
	glutIdleFunc(glutPostRedisplay);
	glEnable(GL_DEPTH_TEST);


	/* points is static to the file */
	points = createSemicircle(img->width, img->height, filename);
	glutMainLoop();
	destroySemicircle(points, img->height);
}

