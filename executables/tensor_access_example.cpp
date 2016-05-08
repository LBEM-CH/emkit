
#include <iostream>
#include <string>
#include <algorithm>

#include "multidim.h"
#include "algorithms.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {

    using namespace em;
    using namespace em::multidim;
    
    typedef Tensor<double, 2> Image;
    
    Image image({5,3}, 0.0);
    
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
    
    std::cout << "Negative Index access: \n";
    std::cout << "Setting {-1, -1} = 3.456\n";
    image[{-1, -1}] = 3.456;
    
    for(const auto& itr : image) {
        std::cout << itr.index() << " -> " << itr.value() <<std::endl;
    }
    
    std::cout << "Getting values for negative indices\n";
    for(const auto& itr: image) {
        std::cout << itr.index()*(-1) << " -> " << image[itr.index()*int(-1)] << std::endl;
    }
    
    std::cout << "Fourier Transforming the image:\n";
    
    /*
    typedef Tensor<Complex<double>, 2> ComplexImage;
    ComplexImage complex_image;
    convert(image, complex_image);
            
    std::cout << "Fourier Transformed Image:\n";
    for(const auto& itr: complex_image) {
        std::cout << itr.index() << " -> " << itr.value() << std::endl;
    }
    */
    std::cout << "\n";
    
}

