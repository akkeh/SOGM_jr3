CC = g++
CFLAGS = -Wall
OBJ = ODF.o STFT.o
PROG = onset

all: $(PROG)

DIR:
		mkdir -p ${DIR}

$(PROG): $(OBJ)
	$(CC) -o $@ $(CFLAGS) $(OBJ)

.cpp.o:
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o
