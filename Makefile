CC = gcc
CFLAGS = -Wall -Werror -g

OS := $(shell uname)
ifeq ($(OS), Darwin)
	CFLAGS += -framework OpenGL -framework GLUT -I/opt/local/include/SDL/
	SDLLIB = `sdl-config --libs`
else
	GLLIBS = -lGL -lGLU -lglut
endif

all: simul

clean:
	rm simul

simul: sim/simul.c sim/loop.c
	$(CC) $(CFLAGS) -o simul sim/simul.c sim/loop.c $(GLLIBS)
