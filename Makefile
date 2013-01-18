TARGET=testproc
OBJECTS=datatypes.o picframe.o $(TARGET).o
#CC=arm-v4t-linux-uclibcgnueabi-gcc
CC=gcc
ALLCFLAGS=-ggdb -O0 -Wall #-I/home/atiti/dev/Firmwares/DF3120/minifs/build-df3120/staging/usr/include
ALLLDFLAGS= #--sysroot=/home/atiti/dev/Firmwares/DF3120/minifs/build-df3120/rootfs/
ALLLIBS=-lSDL -lSDL_image -lSDL_ttf

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(ALLCFLAGS) $(ALLLDFLAGS) -o $@ $(OBJECTS) $(ALLLIBS)

%.o: %.c
	$(CC) $(ALLCFLAGS) -c $< 


clean:
	rm -rf $(OBJECTS) $(TARGET)
