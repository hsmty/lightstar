CC = gcc
CFLAGS = -Wall -Werror -g
GLLIBS = -lGL -lGLU -lglut
SDLLIB = -lSDL

all: simul

clean:
	rm simul

simul: simul.c
	$(CC) $(CFLAGS) -o simul simul.c $(GLLIBS) $(SDLLIB)
