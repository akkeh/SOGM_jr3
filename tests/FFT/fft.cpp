#include <iostream>
#include <cmath>
#include <cstdlib>
/*
    Easy Cooley-Turkey FFT 
    by Akke Houben

*/


#define ARGCOUNT 1

double* append(double* a, unsigned long Na, double* b, unsigned long Nb) {  // change to generic
    double* y = new double[Na + Nb];
    for(unsigned long n=0; n<Na; ++n)
        y[n] = a[n];
    
    for(unsigned long n=0; n<Nb; ++n)
        y[Na + n] = b[n];
    
    return y;
};

double* scramble(double* x, unsigned long N) {   // change to generic function!
    N = N >> 2;
    
    if(N > 0) {
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
 
    // rearrange input buffers:
    double* re = scramble(x_re, N);
    double* im = scramble(x_im, N);


    // FFT:
    double a, b, c, d, oneven_re, oneven_im;
    double** swap_re = new double*[2];
    double** swap_im = new double*[2];
    for(int i=0; i<2; ++i) {
        swap_re[i] = new double[N];
        swap_im[i] = new double[N];   
    }
    int swap_w_id = 1;
    unsigned long tempN = N >> 2;
    for(unsigned long k=0; k<N >> 2; ++k) {
        // copy buffers:
        for(unsigned long n=0; n<N; ++n) {  
            swap_re[0][n] = x_re[n];
            swap_im[0][n] = x_im[n];
        }
        for(unsigned long d=0; d<depth; ++d) {
            
            for(unsigned long n=0; n<tempN; ++n) {
                // a + bj * c + dj = ac - db + adj + cbj
                a = swap_re[swap_r_id][2*n + 1];    // get data points
                b = swap_im[swap_r_id][2*n + 1];
                c = twiddle_re[depth - d - 1];      // load twiddle factor
                d = twiddle_im[depth - d - 1];
                oneven_re = a*c - d*b;
                oneven_im = a*d + c*b;
                swap_re[swap_w_id] = swap_re[swap_r_id][2*n] + oneven_re;
                swap_im[swap_w_id] = swap_im[swap_r_id][2*n] + oneven_im;

            };
            swap_w_id = (swap_w_id + 1) % 2;
            swap_r_id = (swap_w_id + 1) % 2;
            tempN = N >> 2;
        }; 


    };
    // calculate twiddle factors:
    unsigned int depth = log2(N);   // get depth   
    double* twiddle = new double[depth];
    for(unsigned long d=0; d<depth; ++d) 
        twiddle     
    

    return 0;
};



int main(int argc, char** argv) {
    if(argc < ARGCOUNT) {
        return -1;
    }
    unsigned long N = atoi(argv[1]);

    double* x_re = new double[N];
    double* x_im = new double[N];
    double* X_re = new double[N];
    double* X_im = new double[N];
    

    dFFT(x_re, x_im, N, X_re, X_im);

};
