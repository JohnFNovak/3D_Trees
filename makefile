# the compiler to use.
CC=g++

all:
	$(CC) -o branch branch.cpp  -framework OpenGL -framework GLUT

clean:
	rm branch