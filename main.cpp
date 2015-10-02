#include <cmath>
#include <cstdlib>

#include "ODF/cpp/ODF.h"
#include "portaudio/audio_io.h"

#define SAMPLERATE	44100
#define CHANNELS	1
#define FRAMES	    65536
#define ARGCOUNT 3 

int findNotZeros(float* x, unsigned long N, unsigned long offset) {
    int count = 0;
    for(unsigned long n=0; n<N; ++n) {
        if(x[n] > 0) {
            std::cout << "onset: " << N+(offset*N) << std::endl;
            count++;
        };
    };
    return count;
};

int main(int argc, char** argv) {    
    if(argc < ARGCOUNT) {
        std::cout << "usage: ....\n";
        return -1;
    }
    
    unsigned long N = atoi(argv[1]);
    // open audiostream:
    Audio_IO audioStream(SAMPLERATE, 1);
    float* buffer = new float[N];
    static int input_device;

    audioStream.set_mode(AUDIO_IO_READONLY);
    audioStream.set_framesperbuffer(N);
    audioStream.initialise();
    audioStream.list_devices();
    std::cout << "\nGive input device number: ";
    std::cin >> input_device;
    audioStream.set_input_device(input_device);
    audioStream.start_server();


    // init ODF:
    float th = atof(argv[2]);
    ODF* odf = new ODF(N, 512, 1024, 128, th, 0.01, 10);
    float* onsets;

    unsigned long frame = 0; 
    while (true) {
        audioStream.read(buffer);
        onsets = odf->phaseFlux(buffer, N, th, 0.01, 10);
        findNotZeros(onsets, N, frame);
        frame++;
    }

    delete[] buffer;

    return 0;
}
