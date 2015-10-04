#include <iostream>
#include <cstdlib>
#include <cmath>
#include <time.h>

#include "art.h"
#include "neuron.h"

#define FIELD2SIZE 2048
#define FS 44100.0

float abs(float x) {
	return std::sqrt(std::pow(x, 2));
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




// Container
ARTContainer::ARTContainer(unsigned long t_M, unsigned long t_N, float vigilance) {
    M = t_M;
	N = t_N;
	art = new ART(M, N, vigilance);
};

ARTContainer::~ARTContainer() {
	
};

unsigned long ARTContainer::trainAM(unsigned long cycles, unsigned long groups) {
	// create input arrays:
	std::cout << "creating input arrays\n";
	int* group = new int[cycles];
	float*** x = new float**[cycles];
	for(unsigned long i=0; i<cycles; i++) {
		x[i] = new float*[M];
        for(unsigned long m=0; m<M; ++m)
            x[i][m] = new float[N];
	};
	
	// set params of group:
	std::cout << "setting parameters\n";
	int** group_Params = new int*[groups];
	bool* group_has_neuron = new bool[groups];
	int* group_false_neg = new int[groups];
	int* group_false_pos = new int[groups];
	int false_neg = 0;
	int false_pos = 0;
	int f1, f2;
	srand(time(0));
	for(unsigned long i=0; i<groups; i++) {
		group_Params[i] = new int[2];
		group_Params[i][0] = rand() % 10000; 
		group_Params[i][1] = rand() % 10000;
		bool duplicate = false;
		unsigned long j=1;
		while((!duplicate) && (j<i)) {
			if(
				(group_Params[i][0] == group_Params[i-j][0]) || 
				(group_Params[i][1] == group_Params[i-j][0])
			) {
					duplicate = true;
					i = i-1;
			};
			j++;
		}
		group_false_neg[i] = 0;
		group_false_pos[i] = 0;
		group_has_neuron[i] = false;
	};

	// create cycles:
	std::cout << "creating cycles\n";
	for(unsigned long i=0; i<cycles; i++) {
		group[i] = rand() % groups;
		float f1 = group_Params[group[i]][0] + (rand() % 20 - 10);
		float f2 = group_Params[group[i]][1] + (rand() % 20 - 10);
		for(unsigned long n=0; n<N; n++) {
			x[i][n] = std::sin((M_PI+M_PI) * n * f1 / FS) * std::sin((M_PI+M_PI) * n * f2 / FS);
		};
	};
	
	// start training:	
	for(unsigned long i=0; i<cycles; i++) {
		int groupCount = art->get_neuronCount();
		int match_id = art->eval(x[i], group[i]);
		if(art->get_neuronCount() > groupCount) {
			if(group_has_neuron[group[i]]) {
				group_false_neg[group[i]]++;
				false_neg++;
			} else
				group_has_neuron[group[i]] = true;
		}
		if(art->get_id_of(match_id) != group[i]) {
			group_false_pos[group[i]]++;
			false_pos++;
		};
	}	
	

	// 
	std::cout << "amount of groups: " << art->get_neuronCount() << std::endl;
	for(unsigned long i=0; i<groups; i++) {
		int members = 0;
		for(unsigned long j=0; j<cycles; j++)
			if(group[j] == i)
				members++;
		std::cout << " group[" << i << "]: f1 = " << group_Params[i][0] << "\tf2 = " << group_Params[i][1] << "\t members: " << members << "\tfalse negatives: " << group_false_neg[i] << "\tfalse positives: " << group_false_pos[i] << std::endl;
	};
	std::cout << "total false negatives: " << false_neg << " ratio: " << (float)false_neg / (float)cycles << std::endl;
	std::cout << "total false positives: " << false_pos << " ratio: " << (float)false_pos / (float)cycles << std::endl;
	std::cout << "total misses: " << false_pos + false_neg << " ratio: " << (float)(false_pos+false_neg) / (float)cycles << std::endl;
};
