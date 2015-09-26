#include "Analysis.h"

int main( ){
    unsigned long length = 1024;
    unsigned long windowLength = 512;
    double* data = new double[length];
    Analysis fft;

    for (unsigned long i=0; i<length; i++) {
      data[i] = 0.5*sin(2.0*M_PI*i/windowLength)+ 0.5*sin(2.0*M_PI*i/windowLength*10);
    }

    float** powerSpectrum = fft.STFT(data,length,windowLength,1);
    for (unsigned long i=0; i<windowLength/4.0; i++) {
      cout << powerSpectrum[0][i] << endl;
    }

    delete[] data;
    return 0;
}

/*

make
FFTAnalyzer -> plot.txt
gnuplot
plot "plot.txt" with lines

*/
