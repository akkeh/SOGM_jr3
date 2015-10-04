#include <iostream>

int main() {
    unsigned char i;
    float* buf = new float[2048];
    for(unsigned long n=0; n<2048; ++n)
        buf[n] = n;

    while(1==1)
        std::cout << buf[i++] << std::endl;
    
        
    
    return 0;
};
