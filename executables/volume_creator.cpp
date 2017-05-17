
#include <iostream>
#include <string>

#include "objects.h"
#include "elements.h"
#include "fileio.h"

using namespace std;
using namespace em;

int main(int argc, char** argv) {

    if(argc < 6) {
        std::cerr << "Usage:\n\t" << argv[0] << " <nx> <ny> <nz> <radius> <output file name>\n\n";
        exit(1);
    }
    
    int nx = stoi(argv[1]);
    int ny = stoi(argv[2]);
    int nz = stoi(argv[3]);
    double radius = stoi(argv[4]);
    std::string output_file = argv[5];
    
    RealObject<double, 3> circle(Index3d({nx, ny, nz}));
    
    Index3d center = {nx/2, ny/2, nz/2};
    for(auto& val : circle) {
        auto index = val.index() - center;
        for(int i=0; i<3; ++i) {
            if(index[i] > -1* radius && index[i] < radius) val.value() = 1.0;
        }
    }
    
    write(output_file, circle);
    
    return 0;
}

