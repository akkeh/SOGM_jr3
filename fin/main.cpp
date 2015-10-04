#include <fstream>

#include "neuron.h"
#include "art.h"
#include "wavio.h"

#define ARGC 2
int main(int argc, char** argv) {
    if(argc < ARGC) {
        return -1;  
    };
    float vigilance = atof(argv[1]);
    std::ifstream files;
    files.open("files.txt");
    char* file;
    
    STFT* stft = new STFT();

    ART* art = new ART(4096, 0.1);
    unsigned long N;
    while(getline(files, file)) {
        file << files;
        N = -10;
        float* x = readWav(file, &N, 0);
        if(N > 0) {
            float** X = stft->stft(x, N, 512, 1024, 128);
            art->eval(X, 0);
        };
    }
};
