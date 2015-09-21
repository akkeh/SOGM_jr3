#CC-compiler:
CC=g++
CFLAGS = -Wall

DIR=outp

OBJ = art.o neuron.o
 
all: ART

ART: ${OBJ}
	${CC} -o $@ ${CFLAGS} ${OBJ}

.cpp.p:
	${CC} -c $< ${CFlAGS}

clean:
	rm -rf *.o
