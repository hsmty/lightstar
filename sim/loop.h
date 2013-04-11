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

extern void Quit(int ret);
extern int loop();
