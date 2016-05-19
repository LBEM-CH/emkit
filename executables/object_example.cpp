

#include <iostream>
#include "objects.h"
#include "elements.h"
#include "algorithms.h"
#include "fileio.h"

using namespace std;
using namespace em;

int main(int argc, char** argv) {

    if(argc < 2) {
        std::cerr << "Usage:\n\t" << argv[0] << " <MRC FILE>\n\n";
        exit(1);
    }
    
    typedef RealObject<double, 3> Volume;

    Volume input;
    PropertiesMap header_values;
    MRCFile(argv[1]).load(input, header_values);
    
    std::cout << "Read the following header fields:\n";
    std::cout << header_values << std::endl;
    
    return 0;
    
}

