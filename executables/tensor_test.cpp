
#include <iostream>
#include <string>
#include <algorithm>

#include "multidim.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    using namespace em::multidim;
    
    typedef Tensor<double, 2> Image;
    
    Image image({2,3}, 0.0);
    
    image[{1,2}] = 4.34;
    
    std::cout << "Now printing\n";
    
    for(auto itr = image.begin(); itr != image.end(); ++itr) {
        itr->second = 1.0;
    }
    
    for(auto& itr : image) {
        itr.second = 2.0;
    } 
    
    for(const auto& itr : image) {
        std::cout << itr.first << " -> " << itr.second <<std::endl;
    }
    
    std::cout << "\n";
    
}

