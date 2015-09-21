#ifndef __ART_H_
#define __ART_H_

#include <ctime>

#include "neuron.h"

#define VECSIZE 255

class ART {
public:
	ART(unsigned long t_N, double t_vigilance);
	~ART();
	long eval(double* x, unsigned long id);
	unsigned long get_neuronCount();		
	unsigned long get_id_of(unsigned long i);
	
private:
	unsigned long N;
	double vigilance;

	Neuron** field2;
	unsigned long neuronCount;
	double c;

	void add_Neuron(double* x, unsigned long id);
		
	void save_State();
		
};


class ARTContainer {
public:
	ARTContainer(unsigned long t_N, double vigilance);
	~ARTContainer();
	unsigned long trainAM(unsigned long cycles, unsigned long groups);
private:
	ART* art;
	unsigned long N;

};
#endif
