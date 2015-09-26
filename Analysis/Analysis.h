#ifndef __Analysis__
#define __Analysis__

#include <math.h>
#include <algorithm>
#include <iostream>
using std::cout; using std::endl;

typedef struct {	//Peak structure
	float freq;
	float amp;
} peak;

class Analysis {
public:

    //Data length should be two times nn and power of 2
    void FFT(double* data, unsigned long nn);

    //Length is horizontal resolution, windowLength is vertical resolution
    float** STFT(double* data, unsigned long dataLength, int windowLength, int sampleInterval, int length);
    float** STFT(double* data, unsigned long dataLength, int windowLength, int length);
    float** STFT(double* data, unsigned long dataLength, int length);

    float normalize(double* data, unsigned long dataLength, double gain);
    float normalize(double* data, unsigned long dataLength);
    peak** findPeaks(float** data, int windowLength, int length, int numPeak, float threshold);

    // Draw functions
    /*
    void draw(double* data, unsigned long nn, ofVec2f location, ofVec2f size, bool altMode);
    void drawMap(float** map, int length, int dataLength, ofVec2f location, ofVec2f size);
    */

};

#endif
