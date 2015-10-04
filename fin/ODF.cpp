#include <cmath>
#include <iostream>
#include <fstream>
#include <cstdlib>

#include "ODF.h"

// N
#define BUFFERSIZE 1024
// M
#define WINDOWSIZE 512
// bins
#define FFTSIZE 1024
// H
#define HOPSIZE 128

#define TWOPI (M_PI+M_PI)
#define FOURPI (TWOPI+TWOPI)

ODF::ODF(unsigned long N, unsigned long M, unsigned long bins, unsigned long H, float tth, float tbinTh, unsigned long tinhibRel) {
    stft = new STFT(N, M, bins, H);

    th = tth;
    binTh = tbinTh;
    inhibRel = tinhibRel;
    pX_mem = new float[M];
    derv_mem = new float[M];
    for(unsigned long m=0; m<M; m++) {
        pX_mem[m] = 0;
        derv_mem[m] = 0;
    };
};


float imabs(float re, float im) {
    if(re == 0 && im == 0)
        return 0;
    return std::sqrt((re*re)+(im*im));
};

float** stft_mag(float** X, unsigned long frames, unsigned long bins) {
    float** mX = new float*[frames];
    for(unsigned long l=0; l<frames; l++) {
        mX[l] = new float[bins];
        for(unsigned long k=0; k<bins; k++)
            mX[l][k] = imabs(X[l][2*k], X[l][2*k + 1]) / bins;
    };
    return mX;
};

float imphs(float re, float im) {
    if(re == 0 && im == 0)
        return 0;
    return std::atan2(im, re);
};

float** transpose(float** x, unsigned long M, unsigned long N) {
    float** y = new float*[N];
    for(unsigned long n=0; n<N; ++n) {
        y[n] = new float[M];
        for(unsigned long m=0; m<M; ++m)
            y[n][m] = x[m][n];
    };
    return y;
};

float* phase_unwrap(float* x, unsigned long N) {
    float* y = new float[N];
    unsigned long addPI = 0;

    y[0] = x[0];
    for(unsigned long n=1; n<N; n++) {
        if((x[n] < 0) && (x[n - 1] > x[n]))
            addPI++;
        y[n] = x[n] + (TWOPI * addPI);
    };
    return y;
};

float** nd_phase_unwrap(float** x, unsigned long M, unsigned long N) {
    float** y = new float*[M];
    for(unsigned long m=0; m<M; ++m)
        y[m] = phase_unwrap(x[m], N);

    return y;
        
};

float** stft_phs(float** X, unsigned long frames, unsigned long bins) {
    float** pX = new float*[frames];
    for(unsigned long l=0; l<frames; l++) {
        pX[l] = new float[bins];
        for(unsigned long k=0; k<bins; k++)
            pX[l][k] = imphs(X[l][2*k], X[l][2*k + 1]);
    };
    return pX;
};


float* derv(float* x, float mem, unsigned long N) {
    float* y = new float[N];
    y[0] = x[0] - mem;
    for(unsigned long n=1; n<N; ++n)
        y[n] = x[n] - x[n - 1];
    return y;
};

float** nd_derv(float** x, float* mem, unsigned long M, unsigned long N) {
    float** y = new float*[M];
    for(unsigned long m=0; m<M; ++m) {
        y[m] = derv(x[m], mem[m], N);
        mem[m] = y[m][N]; // save memory
//        for(unsigned long n=0; n<N; ++n)
            //std::cout << n << "\t" << m << "\t" << y[m][n] << std::endl;
    }
    
    return y;
};

float* normalise(float* x, unsigned long N) {
    float max = 0;
    for(unsigned long n=0; n<N; ++n)
        if(x[n] > max)
            max = x[n];
    
    float* y = new float[N];
    max = 1.0 / max;
    for(unsigned long n=0; n<N; ++n)
        y[n] = x[n] * max;
    return y;
};

float** nd_normalise(float** x, unsigned long M, unsigned long N) {
    float** y = new float*[M];
    for(unsigned long m=0; m<M; ++m) {
        y[m] = normalise(x[m], N);
    }
    
    return y;
};



float* ODF::phaseFlux(float* x, unsigned long N, float th, float binTh, unsigned long inhibRel) {
    float* onsets = new float[N];
    for(unsigned long n=0; n<N; ++n)
        onsets[n] = 0;
    float** X = stft->stft(x, N, WINDOWSIZE, FFTSIZE, HOPSIZE);

    unsigned long frames = N/HOPSIZE;
    float** mX = stft_mag(X, frames, FFTSIZE);
    float** pX = stft_phs(X, frames, FFTSIZE);
    /*
    // normalise?
    mX = transpose(mX, frames, FFTSIZE);
    mX = nd_normalise(mX, FFTSIZE, frames);
    mX = transpose(mX, FFTSIZE, frames);    
    
    pXT = nd_normalise(pXT, FFTSIZE, frames);
    */
    float** pXT = transpose(pX, frames, FFTSIZE);
    float** pX_unwrap = nd_phase_unwrap(pXT, FFTSIZE, frames);
    
    float** derv = nd_derv(pX_unwrap, pX_mem, FFTSIZE, frames);
    derv = nd_derv(derv, derv_mem, FFTSIZE, frames);

    
    std::ofstream dervPerFrame;
    dervPerFrame.open("perFrame.txt");

    std::cout << "mark!\n";

    for(unsigned long l=0; l<frames; ++l) {
        float val = 0;
        float ampl = 0;
        for(unsigned k=0; k<FFTSIZE; ++k) {
            val += derv[k][l] * (mX[l][k] > binTh);
        }
        val = val / FFTSIZE;
        if(val > th) {
            onsets[l * HOPSIZE] = val;
            for(unsigned long k=0; k<FFTSIZE; ++k) {
                if(derv[k][l] > binTh) 
                    for(unsigned long n=0; n<inhibRel; ++n) {
                        mX[l + n][k] *= n/(2*inhibRel);
                        if((l + n) <= frames)
                            break;
                    }
            };
        }
    }
    
    
    return onsets;
};


