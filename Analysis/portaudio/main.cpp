#include "audioIO.h"

int main() {

  audioIO audioIn;
  audioIn.startAudio();

  // while (true) {
  //   audioIn.readSamples();
  //   audioIn.writeSamples();
  // }

  audioIn.stopAudio();

  return 0;
}
