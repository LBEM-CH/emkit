
#include <iostream>
#include <string>
#include <algorithm>

#include "elements.h"
#include "algorithms.h"

using namespace std;
using namespace em;
using namespace em::element;
typedef Tensor<double, 2, StorageOrder::ROW_MAJOR> ImageRM;
typedef Tensor<double, 2, StorageOrder::COLUMN_MAJOR> ImageCM;
typedef Tensor<Complex<double>, 2, StorageOrder::ROW_MAJOR> ComplexImageRM;
typedef Tensor<Complex<double>, 2, StorageOrder::COLUMN_MAJOR> ComplexImageCM;

/*
 * 
 */
int main(int argc, char** argv) {
    
    if(argc < 3) {
        cerr << "Usage:\n\t" << argv[0] << " <nx> <ny>\n\n";
        exit(1);
    }
    
    int nx = stoi(argv[1]);
    int ny = stoi(argv[2]);
    
    ImageRM image({nx, ny}, 0.0);
    cout << "Initial ROW MAJOR image\n" << image << endl;
    
    /************************
     * POINTER STYLE ACCESS
     ************************/
    for(auto itr = image.begin(); itr != image.end(); ++itr) {
        itr->value() = 1.0;
    }
    //cout << "Pointer style access - Setting all values to 1.0:\n" << image << endl;
    
    
    
    /************************
     * RANGE BASED FOR LOOP
     ************************/
    for(auto& itr : image) {
        itr.value() = 2.0;
    } 
    //cout << "Range based for loop style access - Setting all values to 2.0:\n" << image << endl;
    
    
    
    /*************************************
     * ACESSING THE MEMORY LOCATIONS
     *************************************/
    for(auto& itr: image) {
        itr.value() = ImageRM::arranger_type::map(itr.index(), image.range(), image.origin());
    }
    cout << "Setting the indices to their memory locations\n" << image;
    
    
    
    /*************************************
     * Negative indices access
     *************************************/
    cout << "Printing the whole range:\n";
    for(int i=-1*image.range()[0] + 1; i< image.range()[0]; ++i) {
        for(int j=-1*image.range()[1] + 1; j< image.range()[1]; ++j) {
            cout << Index<2>({i,j}) << " -> " << image[{i,j}] << endl;
        }
    }
    cout << "\n";
    
    
    /*************************************
     * Slicing example
     *************************************/
    for(size_t i=0; i< image.range()[image.rank - 1]; ++i){
        cout << "Slice: " << i << "\n" << image.slice(i);
    }
    cout << "\n";
    
    
    /*************************************
     * Sectioning example
     *************************************/
    Index<2> start = {nx/2, ny/2};
    Range<2> extent = image.range() - start;
    cout << "Section starting from: " << start << " of length: " << extent << "\n";
    cout << image.section(start, extent);
    cout << "\n";
    
    
    /*************************************
     * A different origin image
     *************************************/
    ImageRM image_centered_origin({nx,ny}, {nx/2,ny/2}, 0.0);
    for(auto& itr: image_centered_origin) {
        itr.value() = ImageRM::arranger_type::map(itr.index(), image_centered_origin.range(), image_centered_origin.origin());
    }
    cout << "Origin of image as "<< Index<2>({nx/2, ny/2}) << "\n" << image_centered_origin;
    
    std::cout << "Printing the whole range:\n";
    for(int i=-1*image.range()[0] + 1; i< image.range()[0]; ++i) {
        for(int j=-1*image.range()[1] + 1; j< image.range()[1]; ++j) {
            cout << Index<2>({i,j}) << " -> " << image[{i,j}] << endl;
        }
    }
    cout << "\n";
    
    
    /*************************************
     * Converting the storage order
     *************************************/
    ImageRM image_rm = image;
    ImageCM image_cm({nx, ny}, 0.0);
    convert(image_rm, image_cm);       
    cout << "Changing the storage order of the image:\n" << image_cm << endl;
    
    
    /*************************************
     * Fourier transform of the image
     *************************************/
    ComplexImageCM complex_image;
    fourier_transform(image_cm.range(), image_cm, complex_image);
    cout << "Fourier Transforming the image:\n" << complex_image << endl;
    
    complex_image.transform_origin({0, ny/2});
    cout << "Transformed origin Fourier image:\n" << complex_image << endl;
    
    ComplexImageRM complex_image_rm(complex_image.range(), complex_image.origin(), ComplexImageCM::value_type(0,0));
    convert(complex_image, complex_image_rm);
    cout << "Reordered Fourier transformed image:\n" << complex_image_rm << endl; 
    
    
    cout << "\n\n";
    
}

