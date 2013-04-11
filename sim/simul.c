/* simul.c (c) 2013 R. Díaz de León P.
 * Lightstar simulator
 */
 
#include "loop.h"
#include "settings.h"

int
main( int argc, char **argv )
{
	int ret = loop();
	/* Clean the buffers */
	Quit(ret);
	return ret;
}
