#include <cstdlib>

#include "nn1.h"

double Neuron::get_y() { return y; };

double Neuron::process() {
	/*
		process bottom up
	*/
	y = 0;
	for(unsigned n=0; n<N; ++n) {
		y += inputs[n]->get_y() * w[n];
	};
	return y;
};	// process


Neuron::Neuron(unsigned long t_N, Neuron* t_inputs) {
	N = t_N;
	inputs = t_inputs;

	w = new double[N];
	for(unsigned long n=0; n<N; ++n)
		w[n] = rand() % 200 / 100 - 1;	// initialise random weights

	y = 0;	//
	t = 0;	// reset time
};	// Neuron::Neuron()



Neuron::~Neuron() {
	delete[] w;
};

main() {

};
