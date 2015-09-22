CC = g++
CFLAGS = -Wall
OBJ = Analysis.o
PROG = FFTAnalyzer
DIR = exec

all: $(PROG)

DIR:
		mkdir -p ${DIR}

$(PROG): $(OBJ)
	$(CC) -o $(DIR)/$@  $(CFLAGS) $(OBJ)

.cpp.o:
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o
