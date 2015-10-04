#include <cmath>
#include <cstdlib>
#include <fstream>

#include "STFT.h"
#include "ODF.h"
#include "audio_io.h"
#include "wavio.h"

#define SAMPLERATE	44100
#define CHANNELS	1
#define FRAMES	    65536
#define ARGCOUNT 5

int findNotZeros(float* x, unsigned long N, unsigned long offset, float* buffer) {
    int count = 0;
    float val = 0;
    for(unsigned long n=0; n<N; ++n) {
        if(x[n] > 0) {
            count++;
            val += x[n];
        };
    };
    if(count > 0) 
        std::cout << "frame: " << offset << "\tonset: " << val << std::endl;
    return count;
};

int main(int argc, char** argv) {    
    if(argc < ARGCOUNT) {
        std::cout << "usage: ....\n";
        return -1;
    }
    
//    unsigned long N = atoi(argv[1]);
    unsigned long N;
    // open audiostream:
//    float* buffer;// = new float[100];
    static int input_device;

    /*
    Audio_IO audioStream(SAMPLERATE, 1);
    audioStream.set_mode(AUDIO_IO_READONLY);
    audioStream.set_framesperbuffer(N);
    audioStream.initialise();
    audioStream.list_devices();
    std::cout << "\nGive input device number: ";
    std::cin >> input_device;
    audioStream.set_input_device(input_device);
    audioStream.start_server();
    */

    // init ODF:
    float th = atof(argv[2]);
    float binTh = atof(argv[3]);
    int rechargeN = atoi(argv[4]);
    float* onsets;

    unsigned long frame = 0; 
    float* x =  readWav(argv[1], &N, 1);
    
    std::cout << "read file (" << N << " samples), detecting ODF...\n";
    float* buffer = new float[N*2];
    for(unsigned long n=0; n<N; ++n) {
        buffer[2*n] = x[n];  // real
        buffer[2*n + 1] = 0;      // imag.
    };
    ODF* odf = new ODF(N, 512, 1024, 128, th, binTh, rechargeN);
    onsets = odf->phaseFlux(buffer, N, th, binTh, rechargeN);
/*
    while (true) {
        audioStream.read(buffer);
        onsets = odf->phaseFlux(buffer, N, th, binTh, rechargeN);
        findNotZeros(onsets, N, frame, buffer);
        frame++;
    }
*/
    std::ofstream onsetFile;
    onsetFile.open("onsets.txt");
    int onsetCount = 0;
    for(unsigned long n=0; n<N; n++) {
        onsetFile << onsets[n] << std::endl;
        if(onsets[n] > 0)
            onsetCount++;
    }
    std::cout << "onsets found: " << onsetCount << std::endl;

    delete[] buffer;

    return 0;
}
