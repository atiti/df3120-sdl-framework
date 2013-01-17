TARGET=testproc
OBJECTS=picframe.o $(TARGET).o
CC=gcc
ALLCFLAGS=-ggdb -O0 -Wall
ALLLDFLAGS=
ALLLIBS=-lSDL -lSDL_image

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(ALLCFLAGS) $(ALLLDFLAGS) -o $@ $(OBJECTS) $(ALLLIBS)

%.o: %.c
	$(CC) $(ALLCFLAGS) -c $< 


clean:
	rm -rf $(OBJECTS) $(TARGET)
