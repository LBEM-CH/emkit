
#include <iostream>
#include "objects.h"
#include "elements.h"
#include "algorithms.h"
#include "fileio.h"

using namespace std;
using namespace em;

int main(int argc, char** argv) {

    if(argc < 4) {
        std::cerr << "Usage:\n\t" << argv[0] << " <MRC FILE> <PROJECTION AXIS(x/y,z)> <OUTPUT FILE>\n\n";
        exit(1);
    }
    
    typedef RealObject<double, 3> Volume;
    typedef RealObject<double, 2> Image;
    
    string axis = argv[2];

    Volume input;
    PropertiesMap header_values;
    MRCFile(argv[1]).load(input, header_values);
    
    Image projection(Index2d({input.range()[0], input.range()[1]}));
    
    for(const auto& voxel : input) {
        if(axis == "z") projection[Index2d({voxel.index()[0], voxel.index()[1]})] += voxel.value();
        if(axis == "y") projection[Index2d({voxel.index()[0], voxel.index()[2]})] += voxel.value();
        if(axis == "x") projection[Index2d({voxel.index()[1], voxel.index()[2]})] += voxel.value();
        
    } 
    
    write(argv[3], projection);
    
    return 0;
    
}



