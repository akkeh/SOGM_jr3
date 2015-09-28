#include "audioIO.h"

int main() {

  audioIO audioIn;
  audioIn.startAudio();

  // while (true) {
  //   audioIn.readSamples();
  //   audioIn.writeSamples();
  // }

  return 0;
}
