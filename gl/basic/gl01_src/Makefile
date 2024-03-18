CC = gcc
CFLAGS = -pedantic -Wall -g
LDLIBS = -lGLEW -lGL -lGLU -lglut

.PHONY: all
all: gl1 gl2 gl3
gl1:	gl1.o
gl1.o:	gl1.c
gl2:	gl2.o
gl2.o:	gl2.c
gl3:	gl3.o
gl3.o:	gl3.c

.PHONY: clean
clean:
	rm -f gl1 gl1.o gl2 gl2.o gl3 gl3.o *~
