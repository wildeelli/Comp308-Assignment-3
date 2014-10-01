CC = g++
CFLAGS = -Wall -Wextra -pedantic -std=c++11 -pthread
IPATH = -I/usr/X11/include -I/usr/pkg/include
LPATH = -L/usr/X11/lib -L/usr/pkg/lib
LDPATH = -Wl,-R/usr/pkg/lib 
LIBS = -lm -lopengl32 -lGLU32 -lfreeglut -ljpeg -lpng16 -lglew32
LIBS-LINUX = -lm -lGL -lGLU -lglut -ljpeg -lpng16 -lGLEW
RM = rm

BUILD = ./build/
SRC = ./src/

all: $(BUILD)As3

$(BUILD)As3: $(BUILD)G308_ImageLoader.o $(BUILD)main.o $(BUILD)G308_Geometry.o $(BUILD)loadShader.o
	$(CC) -g -o $@ $^ $(LIBS-LINUX) $(LPATH) $(LDPATH)
	
$(BUILD)%.o:  $(SRC)%.cpp
	$(CC) $(CFLAGS) -g -c -O3 -o $@ $^ $(IPATH)

clean:
	$(RM) -f $(BUILD)*.o $(BUILD)*.gch $(BUILD)MidTerm*
