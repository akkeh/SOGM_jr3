#include <cmath>
#include <iostream>

#define TWOPI (M_PI+M_PI)
#define FOURPI (TWOPI+TWOPI)

/*
    -   static buffers?
    -   mem leaks

*/

int STFT::FFT(double* data, unsigned long nn);


double* STFT::getBlackman(unsigned long M);
double* STFT::zeropad(double* x, unsigned long N, unsigned long newN);

double** STFT::stft(double* x, unsigned long N, unsigned long M, unsigned long bins, unsigned long H);
double imabs(double re, double im);

