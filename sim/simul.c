/* simul.c (c) 2013 R. Díaz de León P.
 * Lightstar simulator
 */
 
#include "loop.h"
#include "settings.h"

int
main( int argc, char **argv )
{
	glutInit(&argc, argv);
	loop(argv[1]);
	return 0;
}
