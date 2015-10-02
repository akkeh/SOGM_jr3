CC = g++
CFLAGS = -Wall
LDFLAGS= -L/usr/local/lib -lportaudio

OBJ= main.o portaudio/audio_io.o ODF/cpp/ODF.o ODF/cpp/STFT.o

all: audioInput

audioInput: $(OBJ)
	$(CC) -o $@ $(CFLAGS) $(OBJ) $(LDFLAGS)

.cpp.o:
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o
