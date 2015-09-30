#include "audio_io.h"

#define SAMPLERATE	44100
#define CHANNELS	1
#define FRAMES	    1024

int main() {
    Audio_IO audioStream(SAMPLERATE,CHANNELS);
    float* buffer = new float[FRAMES*CHANNELS];
    static int input_device;

    audioStream.set_mode(AUDIO_IO_READONLY);
    audioStream.set_framesperbuffer(FRAMES);
    audioStream.initialise();
    audioStream.list_devices();
    std::cout << "\nGive input device number: ";
    std::cin >> input_device;
    audioStream.set_input_device(input_device);
    audioStream.start_server();

    while (true) {
        audioStream.read(buffer);
    }

    delete[] buffer;

    return 0;
}
