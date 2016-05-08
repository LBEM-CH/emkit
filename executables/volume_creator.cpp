
#include <iostream>
#include "components.h"
#include "multidim.h"
#include "file_handler.h"

using namespace std;
using namespace em;
using namespace em::multidim;

using namespace std;


double distance(Index<3> i1, Index<3> i2) {
    Index<3> diff = i1-i2;
    double dist = 0;
    for(int i=0; i<3; ++i) dist += diff[i]*diff[i];
    return dist;
}

int main(int argc, char** argv) {

    if(argc < 5) {
        std::cerr << "Usage:\n\t" << argv[0] << " <nx> <ny> <nz> <radius>\n\n";
        exit(1);
    }
    
    int nx = stoi(argv[1]);
    int ny = stoi(argv[2]);
    int nz = stoi(argv[3]);
    double radius = stoi(argv[4]);
    
    typedef Object<double, 3> Volume;
    typedef Tensor<double, 3> Array3D;
    
    Array3D arr({nx, ny, nz}, 0.0);
    
    Volume circle(arr);
    
    Index<3> center = {nx/2, ny/2, nz/2};
    for(auto& val : circle.container()) {
        if(distance(center, val.index()) < radius) {
            val.value() = 1.0;
        }
    }
    
    MRCFileAdaptor mrc_file("circle.mrc");
    mrc_file.save_object(circle);
    
    return 0;
}

