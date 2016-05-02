
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
    
    typedef Tensor<double, 2, StorageOrder::ROW_MAJOR> Image;
    
    Image image({2,3}, 0.0);
    
    image[{1,2}] = 4.34;
    
    std::cout << "Initial\n";
    
    for(const auto& itr : image) {
        std::cout << itr.index() << " -> " << itr.value() <<std::endl;
    }
    
    std::cout << "First attempt(Pointer style access) - Setting all values to 1.0:\n";
    
    for(auto itr = image.begin(); itr != image.end(); ++itr) {
        itr->value() = 1.0;
    }
    
    for(const auto& itr : image) {
        std::cout << itr.index() << " -> " << itr.value() <<std::endl;
    }
    
    std::cout << "Second attempt(auto& style access) - Setting all values to 2.0:\n";
    
    for(auto& itr : image) {
        itr.value() = 2.0;
    } 
    
    for(const auto& itr : image) {
        std::cout << itr.index() << " -> " << itr.value() <<std::endl;
    }
    
    std::cout << "Third attempt(Classic index style access) - Doubling the alternate index values:\n";
    
    for(int i=0; i<image.size(); i = i+2) {
        image[i] *= 2.0;
    } 
    
    for(const auto& itr : image) {
        std::cout << itr.index() << " -> " << itr.value() <<std::endl;
    }
    
    std::cout << "\n";
    
}

