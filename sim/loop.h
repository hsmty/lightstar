/* loop.h (c) 2013 R. Díaz de León P.
 * Lightstar simulator
 */
 
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
void handleKeyPress(SDL_keysym *key);
int initGL(GLvoid);
int drawGLScene(GLvoid);
int loop();
