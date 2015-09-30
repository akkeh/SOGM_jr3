#ifndef __STFT__class__
#define __STFT__class__

#include <cmath>
#include <iostream>

#define TWOPI (M_PI+M_PI)
#define FOURPI (TWOPI+TWOPI)

//N
#define BUFFERSIZE = 1024;
//M
#define WINDOWSIZE = 512;
//H
#define HOPSIZE = 128;
//bins
#define FFTSIZE = 1024;

class STFT
{
public:
    
    STFT();
    STFT(unsigned long N, unsigned long M, unsigned long bins, unsigned long H);

    int FFT(double* data, unsigned long nn);

    double* getBlackman(unsigned long M);
    double* zeropad(double* x, unsigned long N, unsigned long newN);

    double** stft(double* x);
    double** stft(double* x, unsigned long N, unsigned long M, unsigned long bins, unsigned long H);
    double imabs(double re, double im);
    
private:
    unsigned long N;
    unsigned long M;
    unsigned long bins;
    unsigned long H;
};

#endif
