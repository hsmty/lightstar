CC = gcc
CFLAGS = -Wall -Werror -g
Objects = simul

OS := $(shell uname)
ifeq ($(OS), Darwin)
	CFLAGS += -framework OpenGL -framework GLUT
	Objects += simul.dSYM
else
	GLLIBS = -lGL -lGLU -lglut
endif

all: simul

clean:
	rm -r $(Objects)

simul: sim/simul.c sim/loop.c
	$(CC) $(CFLAGS) -o simul sim/simul.c sim/loop.c $(GLLIBS)
