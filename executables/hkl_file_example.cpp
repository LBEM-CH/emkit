

#include <iostream>
#include "objects.h"
#include "elements.h"
#include "algorithms.h"
#include "fileio.h"

using namespace std;
using namespace em;

int main(int argc, char** argv) {

    if(argc < 6) {
        std::cerr << "Usage:\n\t" << argv[0] << " <HKL FILE> <nx> <ny> <nz> <MRC FILE>\n\n";
        exit(1);
    }
    
    int nx = stoi(argv[2]);
    int ny = stoi(argv[3]);
    int nz = stoi(argv[4]);
    
    typedef RealObject<double, 3> Volume;
    typedef ComplexHalfObject<double, 3> ComplexVolume;

    ComplexVolume input(Index3d({nx, ny,nz}));
    read(argv[1], input);
    
    Volume output;
    fourier_transform(input, output);
    write(argv[5], output);
    
    return 0;
    
}

