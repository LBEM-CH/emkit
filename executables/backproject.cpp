#include <iostream>
#include <map>
#include <thread>
#include "objects.h"
#include "elements.h"
#include "algorithms.h"
#include "fileio.h"

using namespace std;
using namespace em;

typedef RealObject<double, 3> Stack;
typedef RealObject<double, 3> Volume;
typedef RealObject<double, 2> Image;
typedef RealObject<double, 2> Matrix;
typedef ComplexHalfObject<double, 2> FourierImage;
typedef ComplexHalfObject<double, 3> FourierVolume;
typedef std::multimap<Index3d, element::Complex<double> > FourierMultiVolume;
typedef std::pair<Index3d, element::Complex<double> > IndexComplexPair;

void phase_shift(FourierImage& input, double x_shift, double y_shift) {
    
    int dim_x = input.logical_range().at(0);
    int dim_y = input.logical_range().at(1);

    for (auto& data : input) {
        double phase = (-2 * M_PI) * ((x_shift * data.index().at(0) * 1.0/dim_x)  + (y_shift * data.index().at(1) * 1.0/dim_y));
        element::Complex<double> current_complex = data.value();
        element::Complex<double> shift_complex(cos(phase), sin(phase));
        input[data.index()] = current_complex * shift_complex;
    }
}

Index3d tranformed_index(const Index3d& input, double psi, double theta, double phi) {

    // Values for transformation
    double cpsi = cos(psi);
    double cthe = cos(theta);
    double cphi = cos(phi);
    double spsi = sin(psi);
    double sthe = sin(theta);
    double sphi = sin(phi);

    //TRANFORMATION MATRICES
    Matrix A(Index2d({3,3}), std::vector<double>({cpsi, -1*spsi, 0, spsi, cpsi, 0, 0, 0, 1}));
    Matrix B(Index2d({3,3}), std::vector<double>({cthe, 0, sthe, 0, 1, 0, -1*sthe, 0, cthe}));
    Matrix C(Index2d({3,3}), std::vector<double>({cphi, -1*sphi, 0, sphi, cphi, 0, 0, 0, 1}));
    Matrix in(Index2d({1,3}), std::vector<double>({input.at(0)*1.0, input.at(1)*1.0, input.at(2)*1.0}));
    
    Matrix result = matrix_multiply(in, matrix_multiply(A, matrix_multiply(B, C)));
    auto result_vec = result.vectorize();
    
    //cout << input << " -> ";
    //for(auto v : result_vec) cout << " " << v;
    //cout << endl;
    
    Index3d transformed;
    transformed[0] = round(result_vec.at(0));
    transformed[1] = round(result_vec.at(1));
    transformed[2] = round(result_vec.at(2));

    // std::cout << "Transformed: " << input << " -> " << transformed << endl;
    
    return transformed;
}

element::Complex<double> average_reflections(const std::vector<element::Complex<double>>& current_reflections) {
    element::Complex<double> avg(0.0, 0.0);
    for (const auto& c : current_reflections) avg = avg + c;
    return avg*(1.0/current_reflections.size());
}

void average_fourier_volume(const FourierMultiVolume& reflections, FourierVolume& volume) {

    bool initialized = false;

    Index3d current_index;
    std::vector<element::Complex<double> > current_reflections;

    for (const auto& itr : reflections) {
        // cout << itr.first << itr.second << endl;
        // Initialize for the start
        if (!(initialized)) {
            current_index = itr.first;
            initialized = true;
        }

        // Average and insert accumulated spots
        if (!(current_index == itr.first)) {
            
            // cout << current_index << " -> " << average_reflections(current_reflections) <<  endl;
            
            if (volume.range().contains(current_index)) volume[current_index] = average_reflections(current_reflections);
            else std::cerr << "Out of Bounds: Fourier space with size " << volume.range() << " cannot set reflection " << current_index << endl;

            current_reflections.clear();
        }

        //Accumulate the spots in a list
        current_reflections.push_back(itr.second);
        current_index = itr.first;

    }

    //insert the final reflection
    if (volume.range().contains(current_index)) volume[current_index] = average_reflections(current_reflections);
    else std::cerr << "Out of Bounds: Fourier space with size " << volume.range() << " cannot set reflection " << current_index << endl;
}

int main(int argc, char** argv) {

    if (argc < 5) {
        std::cerr << "Usage:\n\t./" << argv[0] << " <PARTICLE STACK FILE> <PARAMETER FILE> <PIXEL SIZE> <OUTPUT VOLUME FILE>\n\n";
        exit(1);
    }
    
    // Reading the stack
    cout << "Reading the particles...\n";
    Stack particles;
    PropertiesMap header_values;
    MRCFile(argv[1]).load(particles, header_values);

    // Calculate properties from input stack
    int num_particles = particles.range().at(2);
    //num_particles = 1;
    int columns = particles.range().at(0);
    int rows = particles.range().at(1);
    
    //Read pixel size
    double pixel_size = std::stod(argv[3]);

    std::cout << "Number of particles found: " << num_particles << endl;
    std::cout << "The pixel size read: " << pixel_size << endl;

    // Read the par file
    cout << "Reading the parameters file...\n";
    Table par_table = Table::read_table(argv[2], 6, ' ', 'C');

    // Gather data for threading
    int num_threads = 1;

    int thread_load = num_particles / num_threads;
    int extra_load = num_particles % num_threads;

    std::cout << "Running on " << num_threads << " threads\n";
    std::cout << "Thread load: " << thread_load << endl;

    // Locks for concurrent processing
    std::vector<std::thread> threads(num_threads);
    std::mutex critical;

    // Fourier Volume to gather data
    FourierMultiVolume reflections;
    
    for (int t = 0; t < num_threads; ++t) {
        int begin = t * thread_load;
        int end = (t + 1) * thread_load;

        //Last one has to take the extra load
        if (t == num_threads - 1) end += extra_load;
        threads[t] = thread(bind([&](int begin, int end) {

            for (int particle = begin; particle < end; ++particle) {
                Image image = particles.slice(particle);
                std::cout << "Processing particle: " << particle +1 << endl;
                
                std::vector<double> pars = par_table.get_row<double>(particle);
                double psi = pars.at(1) * M_PI / 180;
                double theta = pars.at(2) * M_PI / 180;
                double phi = pars.at(3) * M_PI / 180;
                double x_shift = pars.at(4) / pixel_size;
                double y_shift = pars.at(5) / pixel_size;

                //std::cout << "Fourier transforming:...\n"; 
                FourierImage fourier_image;
                fourier_transform(image, fourier_image, em::fft::FFTEnvironment::Instance().new_transformer());

                // Do the phase shift
                //std::cout << "Phase shifting:...\n";
                //phase_shift(fourier_image, x_shift, y_shift);

                // Transform the indices and place in 3D Fourier space
                for (const auto& itr : fourier_image) {
                    Index2d im_idx = itr.index();
                    Index3d vol_idx = tranformed_index(Index3d{im_idx.at(0), im_idx.at(1), 0}, psi, theta, phi);
                    
                    //Bring the index to the current Fourier space size using periodic nature
                    while(vol_idx[0] > fourier_image.miller_index_max()[0]) vol_idx[0] = vol_idx[0] - (fourier_image.logical_range()[0]);
                    while(vol_idx[0] < fourier_image.miller_index_min()[0]) vol_idx[0] = vol_idx[0] + (fourier_image.logical_range()[0]);
                    while(vol_idx[1] > fourier_image.miller_index_max()[1]) vol_idx[1] = vol_idx[1] - fourier_image.logical_range()[1];
                    while(vol_idx[1] < fourier_image.miller_index_min()[1]) vol_idx[1] = vol_idx[1] + fourier_image.logical_range()[1];
                    
                    auto current_complex = itr.value();
                    if (vol_idx.at(0) < 0) {
                        vol_idx = vol_idx * (-1);
                        current_complex.set_phase(current_complex.phase() * (-1));
                    }

                    // Synchronously write to the 3D multi reflections volume
                    critical.lock();
                    if (itr.value().amplitude() > 0.000001) reflections.insert(IndexComplexPair(vol_idx, current_complex));
                    critical.unlock();
                }
            }


        }, begin, end));
    }

    for (thread& t : threads) t.join();

    //Compute the final Fourier volume
    std::cout << "Total number of reflections found in 3D Fourier space: " << reflections.size() << endl;
    Index3d volume_size({columns, rows, max(columns, rows)});
    Index3d origin = volume_size*0.5;
    origin[0] = 0;
    FourierVolume final_fourier = FourierVolume(volume_size, origin);
    average_fourier_volume(reflections, final_fourier);

    write("output.hkl", final_fourier);
    
    std::cout << "Inverse Fourier transforming the final volume\n";
    Volume output;
    fourier_transform(final_fourier, output);

    std::cout << "Writing the output volume: " << argv[4] << endl;
    MRCFile(argv[4]).save(output, header_values);

    return 0;

}
