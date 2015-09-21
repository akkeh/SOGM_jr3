#ifndef __NN1_H_
#define __NN1_H_



class Neuron;

class Neuron {
public:
	
	Neuron(unsigned long t_N, Neuron* t_inputs);
	~Neuron();
	double process();
	double get_y();


private:
	double* w;		// weights
	Neuron* inputs;		// 
	unsigned long N;	// input count

	double y;		// output
	unsigned long t;	// internal time
	


};	// class Neuron


#endif
