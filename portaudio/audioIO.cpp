#include "audioIO.h"

audioIO::audioIO() {
    buffer = new float[numFrames * numChannels];
    bufIndex = 0;
}

audioIO::~audioIO() {
  stopAudio();
  delete[] buffer;
}

void audioIO::startAudio() {
  audioStream.set_mode(AUDIO_IO_READWRITE);
  audioStream.set_samplerate(sampleRate);
  audioStream.set_nrofchannels(numChannels);
  audioStream.set_framesperbuffer(numFrames);
  audioStream.initialise();
  audioStream.list_devices();
  std::cout << "\nGive input device number: ";
  std::cin >> input_device;
  audioStream.set_input_device(input_device);
  std::cout << "Give output device number: ";
  std::cin >> output_device;
  audioStream.set_output_device(output_device);
  audioStream.start_server();
}

void audioIO::readSamples() {
  audioStream.read(buffer);
}

float* audioIO::getBuffer() {
  return buffer;
}

void audioIO::writeSamples() {
  audioStream.write(buffer);
}

void audioIO::stopAudio() {
  audioStream.finalise();
}
