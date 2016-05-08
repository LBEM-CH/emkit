

#include <iostream>
#include "components.h"
#include "multidim.h"
#include "file_handler.h"

using namespace std;
using namespace em;
using namespace em::multidim;

int main(int argc, char** argv) {

    if(argc < 2) {
        std::cerr << "Usage:\n\t" << argv[0] << " <MRC FILE>\n\n";
        exit(1);
    }
    
    typedef Object<double, 3> Volume;
    typedef Tensor<double, 3> Array3D;

    Volume image;
    
    MRCFileAdaptor mrc_file(argv[1]);
    
    mrc_file.load_object(image);
    
    std::cout << image.properties() << std::endl;
    
    
    
    return 0;
    
}

