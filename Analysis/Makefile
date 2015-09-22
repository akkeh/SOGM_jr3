CC = g++
CFLAGS = -Wall
OBJ = Analysis.o main.o
DIR = exec
PROG = FFTAnalyzer

all: $(DIR) $(PROG)

DIR:
		mkdir -p ${DIR}

$(PROG): $(OBJ)
	$(CC) -o $(DIR)/$@ $(CFLAGS) $(OBJ)

.cpp.o:
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o
