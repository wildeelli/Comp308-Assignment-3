CC = g++
CFLAGS = -Wall -Wextra -pedantic
IPATH = -I/usr/X11/include -I/usr/pkg/include
LPATH = -L/usr/X11/lib -L/usr/pkg/lib
LDPATH = -Wl,-R/usr/pkg/lib 
LIBS = -lm -lopengl32 -lGLU32 -lfreeglut -ljpeg -lpng16 
RM = rm

BUILD = ./build/
SRC = ./src/

all: $(BUILD)As3

$(BUILD)As3: $(BUILD)G308_ImageLoader.o $(BUILD)main.o $(BUILD)G308_Geometry.o
	$(CC) -g -o $@ $^ $(LIBS) $(LPATH) $(LDPATH)
	
$(BUILD)%.o:  $(SRC)%.cpp
	$(CC) $(CFLAGS) -g -c -O3 -o $@ $^ $(IPATH)

clean:
	$(RM) -f $(BUILD)*.o $(BUILD)*.gch $(BUILD)MidTerm*
