#ifndef __Analysis__
#define __Analysis__

#include <math.h>
#include <algorithm>
#include <vector>
#include <iostream>
using std::cout; using std::endl;
using std::vector;

typedef struct {	//Peak structure
	unsigned int index;
	double freq;
	double amp;
} peak;


class Analysis {
public:

    //Data length should be two times nn and power of 2
    void FFT(double* data, unsigned long nn);

    //Length is horizontal resolution, windowLength is vertical resolution
    double** STFT(double* data, unsigned long dataLength, int windowLength, int sampleInterval, int length);
    double** STFT(double* data, unsigned long dataLength, int windowLength, int length);
    double** STFT(double* data, unsigned long dataLength, int length);

    float normalize(double* data, unsigned long dataLength, double gain);
    float normalize(double* data, unsigned long dataLength);
    peak* findPeaks(double* data, int windowLength, int numPeak, double threshold);
		//bool operator< (const peak &a, const peak &b) { return a.amp < b.amp; }
};

#endif

// Draw functions
/*
void draw(double* data, unsigned long nn, ofVec2f location, ofVec2f size, bool altMode);
void drawMap(float** map, int length, int dataLength, ofVec2f location, ofVec2f size);
*/
