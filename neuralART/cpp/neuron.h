#ifndef __NEURON_H_
#define __NEURON_H_

class Neuron;

class Neuron {
public:
	double eval(double* x);
	void won();
	unsigned long get_id();
	Neuron(double* x, unsigned long N, double t_c, unsigned long t_id);
	~Neuron();
private:
	double* w;	// weighting
	unsigned long N;
	unsigned long id;


	double c;	// weigthing change factor	
	double y;
	
	
	double* last_x;
	void update_w();
};

#endif
