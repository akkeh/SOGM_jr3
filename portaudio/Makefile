CC = g++
CFLAGS = -Wall
LDFLAGS= -L/usr/local/lib -lportaudio

OBJ= audio_io.o main.o

all: audioIn

audioIn: $(OBJ)
	$(CC) -o $@ $(CFLAGS) $(OBJ) $(LDFLAGS)

.cpp.o:
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o
