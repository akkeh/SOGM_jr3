#include <cmath>
#include <iostream>

#define TWOPI (M_PI+M_PI)
#define FOURPI (TWOPI+TWOPI)

/*
    -   static buffers?
    -   mem leaks

*/

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
    std::cout << "newN: " << newN <<std::endl; 
    double* y = new double[newN];
<<<<<<< HEAD
    for (unsigned long n=0; n<N; ++n) {
        y[n] = x[n];
    }

    for (unsigned long n=N; n<newN; ++n) {
=======
    std::cout << "\n2\n";
    for(unsigned long n=0; n<N; n++)
        y[n] = x[n];
    for(unsigned long n=N; n<newN; n++)
>>>>>>> master
        y[n] = 0;
    }
    return y;
};

double** STFT(double* x, unsigned long N, unsigned long M, unsigned long bins, unsigned long H) {
    // N multiple of M?
<<<<<<< HEAD
    unsigned long frames = N/H;    

    double** X = new double*[frames];
    
    double* w = getBlackman(M);

    double* x_part = new double[bins*2];
=======
    /*
    float diff = (long double)N/(long double)M - (unsigned long)((long double)N/(long double)M);
    unsigned long newN = (unsigned long)(N+(M*diff));
    double* x_zp = zeropad(x, N, newN);    
    */
    unsigned long newN = N;
    unsigned long frames = newN/H;
    double** X = new double*[frames];
    for(unsigned long l=0; l<frames; l++)
        X[l] = new double[bins*2];      

    double* w = getBlackman(M);
 
    double* x_part = new double[M*2];
>>>>>>> master
    for(unsigned long l=0; l<frames; l++) {
        // copy input buffer:
        for(unsigned long n=0; n<M; n++) {
            x_part[2*n] = x[(2*H)*l + 2*n] * w[n];          // even
            x_part[2*n + 1] = x[(2*H)*l + 2*n + 1] * w[n];  // oneven
        };
        X[l] = zeropad(x_part, M*2, bins*2);
        FFT(X[l], bins);
<<<<<<< HEAD

=======
>>>>>>> master
    };
    return X;
};

double imabs(double re, double im) {
    return std::sqrt((re*re)+(im*im));
};

int main() {
    unsigned long N = 1024;
    double* x = new double[N*2];
    for(unsigned long n=0; n<N; ++n) {
        x[2*n] = std::sin(TWOPI * n * 2 / N);
        x[2*n + 1] = 0;
    };
    unsigned long M = 512;
    unsigned long H = 128;
    unsigned long bins = 512;
    double** X = STFT(x, N, M, bins, H);

    for (unsigned long m=0; m<N/H; ++m) {
        //std::cout << "frame: " << m << std::endl;
        for (unsigned long k=0; k<bins; ++k) {
            std::cout << "\t" << imabs(X[m][2*k], X[m][2*k + 1]) << std::endl;
        }
    }

    for (unsigned long m=0; m<N/H - 1; ++m) {
        delete[] X[m];
    }
};

