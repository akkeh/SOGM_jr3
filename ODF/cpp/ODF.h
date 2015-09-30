#include <cmath>
#include <iostream>

#define WINDOWSIZE 512
#define FFTSIZE 1044
#define HOPSIZE 128

/*
    -   static buffers?
    -   mem leaks

*/
class ODF {
public:
    int phaseFlux(double* x, unsigned long N, double th, double inhibTh, unsigned long inhibRel);
private:

};

