
#include <iostream>
#include <string>

#include "objects.h"
#include "elements.h"

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
    
    MRCObject<double> mrc_object(Range3d({nx, ny, nz}), output_file);
    
    Index3d center = {nx/2, ny/2, nz/2};
    for(auto& val : mrc_object.container()) {
        if(center.distance(val.index()) < radius) {
            val.value() = 1.0;
        }
    }
    
    mrc_object.save();
    
    return 0;
}

