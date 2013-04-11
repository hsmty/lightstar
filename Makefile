CC = gcc
CFLAGS = -Wall -Werror -g
GLLIBS = -lGL -lGLU -lglut
SDLLIB = -lSDL

all: simul

clean:
	rm simul

simul: sim/simul.c sim/loop.c
	$(CC) $(CFLAGS) -o simul sim/simul.c sim/loop.c $(GLLIBS) $(SDLLIB)
