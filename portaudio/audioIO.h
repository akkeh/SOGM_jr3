#ifndef _AUDIO_IO_H
#define  _AUDIO_IO_H

#include <iostream>
#include "portaudio_io.h"

#define sampleRate 44100
#define numChannels 1
#define numFrames	64

class audioIO : public Audio_IO {

public:
  Audio_IO audioStream;
  audioIO();
  ~audioIO();
  void startAudio();
  void stopAudio();
  void readSamples();
  float* getBuffer();
  void writeSamples();

protected:
  float* buffer;
  unsigned long bufIndex;
  unsigned int input_device;
  unsigned int output_device;

};

#endif
