#include <iostream>
#include <cstdlib>
#include <cmath>
#include <time.h>

#include "art.h"
#include "neuron.h"

#define FIELD2SIZE 2048
#define FS 44100.0

float abs(float x) {
	return std::sqrt(x*x);
};

unsigned long ART::get_neuronCount() {
	return neuronCount;
};

unsigned long ART::get_id_of(unsigned long i) {
	return field2[i]->get_id();
};

long ART::eval(float** x, unsigned long id) {
	if(neuronCount <= 0) {
		add_Neuron(x, id);
		return eval(x, id);
	} else {
		float* y = new float[neuronCount];
		for(unsigned long i=0; i<neuronCount; ++i) {
			y[i] = field2[i]->eval(x);		// need abs?
		};
	
		// find winner:
		unsigned long max_i = 0;
		for(unsigned long i=0; i<neuronCount; ++i) {
			if(y[i] > y[max_i])
				max_i = i;
		};
		
		// winner > vigilance?
		if(y[max_i] > vigilance) {
			field2[max_i]->won();
			
			return max_i;
		} else {
			add_Neuron(x, id);
			return neuronCount - 1;
		}
		return -1;
	};
	
};

void save_State() {


};

void ART::add_Neuron(float** x, unsigned long id) {
	field2[neuronCount] = new Neuron(x, M, N, c, id);
	neuronCount++;
}; 

ART::ART(unsigned long t_N, float t_vigilance) {
	N = t_N;
	vigilance = t_vigilance;

	field2 = new Neuron*[FIELD2SIZE];	// possible group size
	neuronCount = 0;
	
	c = 0.5;	// learning constant
};

ART::~ART() {
	delete[] field2;
};

/*
	example input:
		$ ./ART 2048 10000 10 0.016

*/
int main(int argc, char** argv) {
	
	if(argc < 4) {
		std::cout << "usage: [input vector m][input vector N][cycles][groups][vigilance]\n";
		return -1;
	} 
	unsigned long N = atoi(argv[1]);
	unsigned long M = atoi(argv[2]);
    
	int cycles = atoi(argv[3]);
	int groups = atoi(argv[4]);
	float vigilance = atof(argv[5]);

	ARTContainer* trainer = new ARTContainer(M, N, vigilance);
	trainer->trainAM(cycles, groups);
};


