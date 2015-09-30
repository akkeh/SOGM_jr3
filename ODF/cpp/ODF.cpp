#include <cmath>
#include <iostream>

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
/*
    -   static buffers?
    -   mem leaks

*/

double imabs(double re, double im) {
    return std::sqrt((re*re)+(im*im));
};

double** stft_mag(double** X, unsigned long frames, unsigned long bins) {
    double** mX = new double*[frames];
    for(unsigned long l=0; l<frames; l++) {
        mX[l] = new double[bins];
        for(unsigned long k=0; k<bins; k++)
            mX[l][k] = imabs(X[l][2*k], X[l][2*k + 1]) / bins;
    };
    return mX;
};

double imphs(double re, double im) {
    return std::atan2(im, re);
};

double** transpose(double** x, unsigned long M, unsigned long N) {
    double** y = new double*[N];
    for(unsigned long n=0; n<N; ++n) {
        y[n] = new double[M];
        for(unsigned long m=0; m<M; ++m)
            y[n][m] = x[m][n];
    };
    return y;
};

double* phase_unwrap(double* x, unsigned long N) {
    double* y = new double[N];
    unsigned long addPI = 0;

    y[0] = x[0];
    for(unsigned long n=1; n<N; n++) {
        if((x[n] < 0) && (x[n - 1]))
            addPI++;
        y[n] = x[n] + (TWOPI * addPI);
    };
    return y;
};

double** nd_phase_unwrap(double** x, unsigned long M, unsigned long N) {
    double** y = new double*[M];
    for(unsigned long m=0; m<M; ++m)
        y[m] = phase_unwrap(x[m], N);

    return y;
        
};

double** stft_phs(double** X, unsigned long frames, unsigned long bins) {
    double** pX = new double*[frames];
    for(unsigned long l=0; l<frames; l++) {
        pX[l] = new double[bins];
        for(unsigned long k=0; k<bins; k++)
            pX[l][k] = imphs(X[l][2*k], X[l][2*k + 1]);
    };
    // transpose pX
    return pX;
};




int FFT(double* data, unsigned long nn) {
    unsigned long n, mmax, m, j, istep, i;
    double wtemp, wr, wpr, wpi, wi, theta;
    double tempr, tempi;

    // reverse-binary reindexing
    n = nn << 1;
    j = 1;
    for (i = 1; i < n; i += 2) {
        if (j > i) {
            std::swap(data[j-1], data[i-1]);
            std::swap(data[j], data[i]);
        }
        m = nn;
        while (m >= 2 && j > m) {
            j -= m;
            m >>= 1;
        }
        j += m;
    };

    // Danielson-Lanczos
    mmax = 2;
    while (n > mmax) {
        istep = mmax << 1;
        theta = -(2 * M_PI / mmax);
        wtemp = sin(0.5 * theta);
        wpr = -2.0 * wtemp*wtemp;
        wpi = sin(theta);
        wr = 1.0;
        wi = 0.0;
        for (m = 1; m < mmax; m += 2) {
            for (i = m; i <= n; i += istep) {
                j= i + mmax;
                tempr = wr * data[j-1] - wi * data[j];
                tempi = wr * data[j] + wi * data[j-1];

                data[j-1] = data[i-1] - tempr;
                data[j] = data[i] - tempi;
                data[i-1] += tempr;
                data[i] += tempi;
            }
            wtemp = wr;
            wr += wr*wpr - wi*wpi;
            wi += wi*wpr + wtemp*wpi;
        }
        mmax = istep;
    }
    return 0;
}

double* getBlackman(unsigned long M) {
    double a0, a1, a2;
    a0 = 7938.0/18608.0;
    a1 = 9240.0/18608.0;
    a2 = 1430.0/18608.0;

    double* w = new double[M];
    for(unsigned long n=0; n<M; ++n) {
        w[n] = a0 - a1 * std::cos((TWOPI * (long double)n) / ((long double)M - 1)) + a2 * std::cos((FOURPI * (long double)n) / ((long double)M - 1));
    };

    return w;
};

double* zeropad(double* x, unsigned long N, unsigned long newN) {
    double* y = new double[newN];
    for (unsigned long n=0; n<N; ++n) 
        y[n] = x[n];
    for(unsigned long n=N; n<newN; n++)
        y[n] = 0;
    return y;
};

double** STFT(double* x, unsigned long N, unsigned long M, unsigned long bins, unsigned long H) {
    // N multiple of M?
    unsigned long frames = N/H;    
    double** X = new double*[frames];
    
    double* w = getBlackman(M);

    double* x_part = new double[bins*2];
    
    for(unsigned long l=0; l<frames; l++) {
        // copy input buffer:
        for(unsigned long n=0; n<M; n++) {
            x_part[2*n] = x[(2*H)*l + 2*n] * w[n];          // even
            x_part[2*n + 1] = x[(2*H)*l + 2*n + 1] * w[n];  // oneven
        };
        X[l] = zeropad(x_part, M*2, bins*2);
        FFT(X[l], bins);

    };
    return X;
};

double* derv(double* x, unsigned long N) {
    double* y = new double[N];
    y[0] = 0;
    for(unsigned long n=1; n<N; ++n)
        y[n] = x[n] + x[n - 1];
    return y;
};

double** nd_derv(double** x, unsigned long M, unsigned long N) {
    double** y = new double*[M];
    for(unsigned long m=0; m<M; ++m)
        y[m] = derv(x[m], N);
    return y;
};

double* ODF::phaseFlux(double* x, unsigned long N, double th, double inhibTh, unsigned long inhibRel) {
    double* onsets = new double[N];

    // STFT* stft = new STFT(WINDOWSIZE, FFTSIZE, HOPSIZE);
    // double** X = STFT->stft(x, N, WINDOWSIZE, FFTSIZE, HOPSIZE);
    double** X = STFT(x, N, WINDOWSIZE, FFTSIZE, HOPSIZE);

    unsigned long frames = N/HOPSIZE;
    double** mX = stft_mag(X, frames, FFTSIZE);
    double** pX = stft_phs(X, frames, FFTSIZE);

    double** pXT = transpose(pX, frames, FFTSIZE);
    double** pX_unwrap = nd_phase_unwrap(pXT, FFTSIZE, frames);
    
    double** derv = nd_derv(pX_unwrap, FFTSIZE, frames);
    derv = nd_derv(derv, FFTSIZE, frames);
    
 
    for(unsigned long l=0; l<frames; ++l) {
        double val = 0;
        for(unsigned k=0; k<FFTSIZE; ++k) 
            val += derv[k][l] * mX[l][k];
        if(val > th) {
            onsets[l * HOPSIZE] = val;
            for(unsigned long k=0; k<FFTSIZE; ++k) {
                if(derv[k][l] > inhibTh) {
                    for(unsigned long n=0; n<inhibRel; ++n) {
                        mX[l + n][k] *= n/inhibRel;
                        if((l + n) <= frames)
                            break;
                    }
                }
            };
        }
    }
    
    
    return onsets;
};


int main() {
    unsigned long N = 1024;
    double* x = new double[N*2];
    for(unsigned long n=0; n<N; ++n) {
        x[2*n] = std::sin(TWOPI * n * 2 / N);
        x[2*n + 1] = 0;
    };
    ODF odf;
    double* onsets = odf.phaseFlux(x, N, 0.01, 0.01, 10);

    for(unsigned long n=0; n<N; ++n)
        std::cout << onsets[n] << std::endl;
};


