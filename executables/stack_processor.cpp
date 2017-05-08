#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include "objects.h"
#include "elements.h"
#include "algorithms.h"
#include "fileio.h"

using namespace std;
using namespace em;

int main(int argc, char** argv) {

    if (argc < 3) {
        std::cerr << "Usage:\n\t" << argv[0] << " <Input MRCS FILE> <Output MRCS FILE>\n\n";
        exit(1);
    }

    typedef RealObject<double, 3> Volume;
    typedef RealObject<double, 2> Image;
    typedef ComplexHalfObject<double, 2> ComplexImage;

    Volume input;
    PropertiesMap header_values;
    MRCFile(argv[1]).load(input, header_values);

    //Write out the header
    std::cout << "Read " << argv[1] << " with following header fields:\n";
    std::cout << header_values << std::endl;

    //Calculate some values
    int sections = input.range().at(2);
    int columns = input.range().at(0);
    int rows = input.range().at(1);

    //Process the file
    int num_threads = thread::hardware_concurrency();

    int thread_load = sections / num_threads;
    int extra_load = sections % num_threads;

    std::cout << "Running on " << num_threads << " threads\n";
    std::cout << "Thread load " << thread_load << endl;

    std::vector<std::thread> threads(num_threads);
    std::mutex critical;
    Volume output({columns / 2, rows / 2, sections}, 0.0);
    Index2d output_range = Index2d({columns / 2, rows / 2});
    for (int t = 0; t < num_threads; ++t) {
        int begin = t * thread_load;
        int end = (t + 1) * thread_load;

        //Last one has to take the extra load
        if (t == num_threads - 1) end += extra_load;
        
        threads[t] = thread(bind([&](int begin, int end) {
            
            for (int stack = begin; stack < end; ++stack) {
                Image image = input.slice(stack);
                ComplexImage fourier_image;
                
                fourier_transform(image, fourier_image, fft::FFTEnvironment::Instance().new_transformer());
                
                //Crop the image
                ComplexImage fourier_cropped(output_range);
                for (auto& data : fourier_image) if (fourier_cropped.range().contains(data.index())) fourier_cropped[data.index()] = data.value();
                Image cropped;

                fourier_transform(fourier_cropped, cropped, fft::FFTEnvironment::Instance().new_transformer());

                //Write to the output synchronously
                {
                    std::lock_guard<std::mutex> lock(critical);
                    std::cout << "Setting stack " << stack << std::endl;
                    output.set_slice(stack, cropped);
                }
            }
        }, begin, end));
    }

    for (thread& t : threads) t.join();

    std::cout << "Writing out output...\n";
    MRCFile(argv[2]).save(output, header_values);

    return 0;

}

