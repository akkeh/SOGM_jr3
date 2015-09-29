#include "Analysis.h"

int main( ){
    unsigned long length = 1024;
    unsigned long windowLength = 512;
    double* data = new double[length];
    Analysis fft;

    //Test signal
    for (unsigned long i=0; i<length-1; i+=2) {
      data[i] = 0.5*sin(2.0*M_PI*i/windowLength)+ 0.5*sin(2.0*M_PI*i/windowLength*10); //Real part is signal
      data[i+1] = 0;  //Img part = 0
    }

    //Compute power spectrum
    double** powerSpectrum = fft.STFT(data,length,windowLength,1);
    for (unsigned long i=0; i<windowLength/2.0; i++) {
      cout << powerSpectrum[0][i] << endl;
    }
    
    //peak* findPeaks = fft.findPeaks(powerSpectrum[0], windowLength/4, 1, 10, 0.6);

    delete[] data;
    return 0;
}

/*

make
FFTAnalyzer -> plot.txt
gnuplot
plot "plot.txt" with lines

*/
