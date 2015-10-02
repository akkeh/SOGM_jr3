#include <cmath>
#include "audio_io.h"

#define SAMPLERATE	44100
#define CHANNELS	1
#define FRAMES	    65536

<<<<<<< HEAD

double abs(double a) {
    return std::sqrt(a*a);

};
double sum(float* buf) {
    double val = 0;
    for(unsigned long n=0; n<FRAMES*CHANNELS; ++n)
        val += abs(buf[n]);
    return val;

};

int main() {    
    // open audiostream:
=======
void startAudio(Audio_IO* audioStream) {
    static int input_device;
    audioStream->set_mode(AUDIO_IO_READONLY);
    audioStream->set_framesperbuffer(FRAMES);
    audioStream->initialise();
    audioStream->list_devices();
    std::cout << "\nGive input device number: ";
    std::cin >> input_device;
    audioStream->set_input_device(input_device);
    audioStream->start_server();
}

int main() {
>>>>>>> 8f4f0725ef6e33bf71dc142ca536d6a17cb95e71
    Audio_IO audioStream(SAMPLERATE,CHANNELS);
    float* buffer = new float[FRAMES*CHANNELS];

    startAudio(&audioStream);

    unsigned long frame = 0; 
    while (true) {
        audioStream.read(buffer);
        std::cout << (buffer) << std::endl;
    }

    delete[] buffer;

    return 0;
}
