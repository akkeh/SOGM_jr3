//
//  Analysis.h
//
//  Created by Stefan Hendriks on 15/09/15.
//
//

#ifndef __Analysis__
#define __Analysis__

//#include "ofMain.h"
//#include <stdio.h>

#include <math.h>
#include <algorithm>

class Analysis
{
public:
    
    // data moet 2 keer de lengte hebben van nn
    void FFT(double* data, unsigned long nn);
    
    // length is de horizontale resolutie, windowLength bepaalt de fft(verticale) resolutie
    float** STFT(double* data, unsigned long dataLength, int windowLength, int sampleInterval, int length);
    float** STFT(double* data, unsigned long dataLength, int windowLength, int length);
    float** STFT(double* data, unsigned long dataLength, int length);
    
    // normalized de buffer data met lenghte dataLength, geeft het hartste sample terug voor de weging
    float normalize(double* data, unsigned long dataLength, double gain);
    float normalize(double* data, unsigned long dataLength);
    
    // teken functies
    /*
    void draw(double* data, unsigned long nn, ofVec2f location, ofVec2f size, bool altMode);
    void drawMap(float** map, int length, int dataLength, ofVec2f location, ofVec2f size);
    */
};

#endif /* defined(__Analysis__) */
