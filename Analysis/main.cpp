#include "Analysis.h"

int main( ){
    unsigned long length = 2048;
    double* data = new double[length];
    Analysis fft;

    for (unsigned long i=0; i<length; i++) {
      data[i] = sin(2.0*M_PI*i/length)+ sin(2.0*M_PI*i/length*10);
    }

    /*float** powerSpectrum = */fft.FFT(data,length/2);

    for (unsigned long i=0; i<length/2-1; i+=2) {
      cout << sqrt(pow(data[i],2) + pow(data[i+1],2)) / (length/2)  << endl;
    }

    // for (unsigned long i=0; i<length/2; i++) {
    //   cout << powerSpectrum[0][i] << endl;
    // }

    return 0;
}

/*

make
FFTAnalyzer -> plot.txt
gnuplot
plot "plot.txt" with lines

*/
