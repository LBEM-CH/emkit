#include <iostream>
#include <string>

#include "objects.h"
#include "elements.h"
#include "algorithms.h"
#include "fileio.h"

using namespace std;
using namespace em;

int main(int argc, char** argv) {

    if(argc < 3) {
        std::cerr << "Usage:\n\t" << argv[0] << " <MRC FILE> <HKL FILE>\n\n";
        exit(1);
    }
    
    typedef RealObject<double, 3> Volume;
    typedef ComplexHalfObject<double, 3> FourierVolume;

    std::cout << "Reading the volume from file: " << argv[1] << " ...\n";
    Volume input;
    read(argv[1], input);
    
    
    std::cout << "Fourier transforming the volume ... \n";
    FourierVolume output;
    fourier_transform(input, output);
    
    std::cout << "Writing the volume to file: " << argv[2] << " ...\n";
    write(argv[2], output);
    
    return 0;
    
}
