CC = g++
LPATH = -L/usr/pkg/lib
LDPATH = -Wl,-R/usr/pkg/lib 
CFLAGS=-g -Wall
LIBS=-lopengl32 -lfreeglut -lGLU32 -ljpeg -lpng16 -lm
IPATH= -I/usr/pkg/include

SRC = ./src/

all: TextureDemo
TextureDemo : TextureDemo.o G308_ImageLoader.o
	$(CC) -o TextureDemo TextureDemo.o G308_ImageLoader.o $(LIBS) $(LPATH) $(LDPATH)
TextureDemo.o :
	$(CC) -c $(CFLAGS) $(SRC)TextureDemo.cpp $(IPATH)  
G308_ImageLoader.o :
	$(CC) -c $(CFLAGS) $(SRC)G308_ImageLoader.cpp $(IPATH)
clean :
	rm -rf *.o
	rm TextureDemo
