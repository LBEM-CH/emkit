

#include <iostream>
#include "objects.h"
#include "elements.h"

using namespace std;
using namespace em;

int main(int argc, char** argv) {

    if(argc < 2) {
        std::cerr << "Usage:\n\t" << argv[0] << " <MRC FILE>\n\n";
        exit(1);
    }
    
    typedef Object<double, 3> Volume;
    typedef Tensor<double, 3> Array3D;

    MRCObject<double> mrc_file(argv[1]);
    mrc_file.load();
    std::cout << mrc_file.properties() << std::endl;
    
    return 0;
    
}

