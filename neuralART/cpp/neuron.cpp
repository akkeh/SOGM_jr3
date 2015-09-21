#include <iostream>

#include "neuron.h"

void Neuron::won() {
	/*
		notify neuron it has won
	*/
	update_w();
};

unsigned long Neuron::get_id() {
	return id;
};

double Neuron::eval(double* x) {
	y = 0;
	for(unsigned long n=0; n<N; ++n) 
		y += x[n] * w[n];	// inproduct
	last_x = x;
	return y;
};

void Neuron::update_w() {
	double error = 0;
	for(unsigned long n=0; n<N; ++n) {
		error = last_x[n] - w[n];
		w[n] = w[n] + (c * error);
	}
};

Neuron::Neuron(double* x, unsigned long t_N, double t_c, unsigned long t_id) {
	N = t_N;
	w = new double[N];
	for(unsigned long n=0; n<N; ++n)
		w[n] = x[n];

	id = t_id;
	c = t_c;
	y = 0;
};	// Neuron::Neuron()

Neuron::~Neuron() {
	delete[] w;
};	// Neuron::~Neuron()
