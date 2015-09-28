#include "Analysis.h"

void Analysis::FFT(double* data, unsigned long nn) {
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
}

double** Analysis::STFT(double* data, unsigned long dataLength, int windowLength, int sampleInterval, int length) {
    double** spect = 0;
    spect = new double*[length];
    double* temp = new double[windowLength];

    for (int l = 0; l < length; l++) {
        spect[l] = new double[windowLength / 2];

        for (int i = 0; i < windowLength; i++) {
            temp[i] = data[(l * sampleInterval) + i];
        }

        FFT(temp, windowLength / 2);  //Compute FFT for one window
        for (int i = 0; i < windowLength-1 / 2; i+=2) { //Compute power spectrum of window
            spect[l][i] = sqrt(pow(temp[i],2)+pow(temp[i+1],2)) / (windowLength/2.0);
        }
    }
    delete[] temp;

    return spect;
}

double** Analysis::STFT(double* data, unsigned long dataLength, int windowLength, int length) {
    return STFT(data, dataLength, windowLength, dataLength / length, length);
}


double** Analysis::STFT(double* data, unsigned long dataLength, int length) {
    return STFT(data, dataLength, 256, dataLength / length, length);
}


float Analysis::normalize(double* data, unsigned long dataLength, double gain) {
    double max = 0;

    for (int i = 0; i < dataLength; i++) {
        if (fabs(data[i]) > max) {
            max = fabs(data[i]);
        }
    }

    for (int i = 0; i < dataLength; i++) {
        data[i] = data[i] * gain / max;
    }

    return max;
}


float Analysis::normalize(double* data, unsigned long dataLength) {
    return normalize(data, dataLength, 1.0);
}

peak* Analysis::findPeaks(double* data, int windowLength, int numPeak, double threshold) {
  vector<peak> peaks;
  peak peak;

  for (unsigned long i=0; i<windowLength/2.0; i++) {  //Find peaks
    if (data[i] > threshold) {
      peak.amp = data[i];
      peak.index = i;
      peaks.push_back(peak);
    }
  }
  for (unsigned long i=0; i<peaks.size(); i++) {
    cout << peaks[i].amp;
  }
  //std::sort(peaks.begin(),peaks.end());  //Sort peaks

  return 0;
}

/*
void Analysis::draw(double* data, unsigned long nn, ofVec2f location, ofVec2f size, bool altMode) {
    ofSetColor(255);
    float vert = size.x / nn;
    if (altMode) {
        for (int i = 0; i < nn; i++) {
            ofLine(location.x + (i * vert + vert / 2), location.y + size.y,
                   location.x + (i * vert + vert / 2), location.y + size.y + abs(data[i] / 8 * size.y) * -1);
        }
    } else {
        for (int i = 0; i < nn; i++) {
            ofLine(location.x + (i * vert + vert / 2), location.y + (size.y / 2),
                   location.x + (i * vert + vert / 2), location.y + (size.y / 2) + (size.y / 2) * data[i]);
        }
    }
}


void Analysis::drawMap(float** map, int length, int dataLength, ofVec2f location, ofVec2f size) {
    ofVec2f rect(size.x / length, size.y / dataLength);

    for (int l = 0; l < length; l++) {
        for (int i = 0; i < dataLength; i++) {

            ofSetColor(fmin(map[l][i] * 5, 255));
            ofRect(l * rect.x + location.x,
                   i * rect.y + location.y,
                   rect.x,
                   rect.y);
        }
    }
}
*/
