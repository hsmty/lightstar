CC = gcc
CFLAGS = -Wall -Werror -g
GLLIBS = -lGL -lGLU -lglut
SDLLIB = -lSDL

all: pov simul

clean:
	rm pov simul

pov: pov.c
	$(CC) pov.c -o pov -lGL -lGLU -lSDL -lglut

simul: simul.c
	$(CC) $(CFLAGS) -o simul simul.c $(GLLIBS) $(SDLLIB)
