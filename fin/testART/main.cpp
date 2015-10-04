#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "STFT.h"
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
    std::string file;
    
    STFT* stft = new STFT(4096, 512, 1024, 128);

    ART* art = new ART(4096/128, 1024, 0.1);
    unsigned long N;
    std::cout << "start testing\n";
    while(std::getline(files, file)) {
        N = -10;
        std::cout << "loading file: " << file << std::endl;
        float* x = readWav(file.c_str(), &N, 0);
        std::cout << "loaded file: " << file << std::endl;
        if(N > 0) {
            std::cout << "STFT\n";
            float** X = stft->stft(x, 4096, 512, 1024, 128);
            std::cout << "ART\n";
            art->eval(X, 0);
        };
    }
};
