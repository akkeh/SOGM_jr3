#include <iostream>
#include <cmath>
#include <cstdlib>
/*
    Easy Cooley-Turkey FFT 
    by Akke Houben

*/

#define TWOPI (M_PI+M_PI)
#define FS 44100
#define ARGCOUNT 1

double imabs(double re, double im) {
    double mag = std::sqrt((re*re)+(im*im));
    return mag;
}

double* append(double* a, unsigned long Na, double* b, unsigned long Nb) {  // change to generic
    double* y = new double[Na + Nb];
    for(unsigned long n=0; n<Na; ++n)
        y[n] = a[n];
    
    for(unsigned long n=0; n<Nb; ++n)
        y[Na + n] = b[n];
    
    return y;
};

double* scramble(double* x, unsigned long N) {   // change to generic function!
    N = N >> 1;
    
    if(N > 1) {
        double* even = new double[N];
        double* oneven = new double[N];
        for(unsigned long n=0; n<N; ++n) {
            even[n] = x[n*2];       // get even samples
            oneven[n] = x[n*2 + 1]; // get uneven samples
        };
        even = scramble(even, N);
        oneven = scramble(oneven, N);
        return append(even, N, oneven, N);
    }
    return x;
};

double* zeropad(double* x, unsigned long N, unsigned long newN) {   // change to generic
    double* y = new double[newN];
    if(newN > N) {
        for(unsigned long n=0; n<N; ++n)
            y[n] = x[n];
        for(unsigned long n=0; n<newN-N; ++n)
            y[N + n] = 0;
        return y;
    } else {
        for(unsigned long n=0; n<newN; ++n)
            y[n] = x[n];
        return y;
    };
};

int dFFT(double* x_re, double* x_im, unsigned long N, double* X_re, double* X_im) {
    if(log2(N) != (int)log2(N)) {
        unsigned long newN = pow(2, (unsigned long)log2(N)+1);
        x_re = zeropad(x_re, N, newN);
        x_im = zeropad(x_im, N, newN);
        N = newN;
    };
    unsigned long depth = log2(N);
    unsigned long tempN;

    // rearrange input buffers:
    double* re = scramble(x_re, N);
    double* im = scramble(x_im, N);

    // declare arrays:
    double* w_re = new double[depth];
    double* w_im = new double[depth];
    double* copy_re = new double[N];
    double* copy_im = new double[N];

    // FFT:
    for(unsigned long k=0; k<N >> 1; ++k) {
        // calculate twiddle factors
         for(unsigned long d=0; d<depth; ++d) {
            w_re[d] = std::cos(TWOPI * k * pow(2, d) / (long double)N);
            w_im[d] = -std::sin(TWOPI * k * pow(2, d) / (long double)N);
        }        
        // copy inputbuffers
        for(unsigned long n=0; n<N; ++n) {
            copy_re[n] = re[n];
            copy_im[n] = im[n];
        };       

        tempN = N;
        // the FFT:

        for(unsigned long d=0; d<depth; ++d) {
            for(unsigned long n=0; n<tempN; ++n) {
                // a+bj * c+dj = a*c - b*d + adj * cbj
                copy_re[2*n] += copy_re[2*n + 1] * w_re[depth - d - 1] - copy_im[2*n + 1] * w_im[depth - d - 1];
                copy_im[2*n] += copy_re[2*n + 1] * w_im[depth - d - 1] + copy_im[2*n + 1] * w_re[depth - d - 1];
            };
            tempN = tempN >> 1;
        };
        
       X_re[k] = copy_re[0];
       X_re[N - k - 1] = copy_re[0];
       X_im[k] = copy_im[0];
       X_im[N - k - 1] = copy_im[0];
    };
   return 0;
};



int main(int argc, char** argv) {
    if(argc < ARGCOUNT) {
        return -1;
    }
    unsigned long N = pow(2, atoi(argv[1]));

    double* x_re = new double[N];
    double* x_im = new double[N];
    double* X_re = new double[N];
    double* X_im = new double[N];
    
    std::cout << "fill input buffers\n"; 
    for(unsigned long n=0; n<N; ++n) {
        x_re[n] = std::sin(TWOPI * 2 * n / N);
        x_im[n] = 0;
    };

    std::cout << "calc FFT\n";
    dFFT(x_re, x_im, N, X_re, X_im);


    for(unsigned long k=0; k<N >> 1; ++k) {
        std::cout << "bin: " << k << "\tmag: " << imabs(X_re[k], X_im[k]) << std::endl;
    };
};
