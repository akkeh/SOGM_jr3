CC = g++
CFLAGS = -Wall
OBJ = ODF.o
PROG = FFTAnalyzer

all: $(PROG)

DIR:
		mkdir -p ${DIR}

$(PROG): $(OBJ)
	$(CC) -o $@ $(CFLAGS) $(OBJ)

.cpp.o:
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o
