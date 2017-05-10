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
typedef ComplexHalfObject<double, 2> FourierImage;
typedef ComplexHalfObject<double, 3> FourierVolume;
typedef std::multimap<Index3d, element::Complex<double> > FourierMultiVolume;
typedef std::pair<Index3d, element::Complex<double> > IndexComplexPair;


FourierImage phase_shift(FourierImage input, double x_shift, double y_shift) {
    
    int dim = input.range().at(1);
    
    FourierImage output = input;
    
    for(const auto& data : input) {
        auto current_complex = data.value();
        double new_phase = current_complex.phase() + (2/dim) * (x_shift*data.index().at(0) + y_shift*data.index().at(1));
        current_complex.set_phase(new_phase);
        output.at(data.index()) = current_complex;
    }
    
    return output;
    
}

Index3d tranformed_index(const Index3d& input, double psi, double theta, double phi) {
    
    assert(input.rank == 3);
    
    // Values for transformation
    double cpsi = cos(psi);
    double cthe = cos(theta);
    double cphi = cos(phi);
    double spsi = sin(psi);
    double sthe = sin(theta);
    double sphi = sin(phi);
    
    Index3d transformed = input;
    transformed.at(0) = round(input.at(0)*(cpsi*cthe*cphi - spsi*sphi) 
                        + input.at(1)*(-1*spsi*cthe*cphi - cpsi*sphi)
                        + input.at(2)*(sthe*cphi));
    
    transformed.at(1) = round(input.at(0)*(cpsi*cthe*sphi + spsi*cphi)
                        + input.at(1)*(-1*spsi*cthe*sphi + cpsi*cphi)
                        + input.at(2)*(sthe*sphi));
    
    transformed.at(2) = round(input.at(0)*(-1*cpsi*sthe) + input.at(1)*(spsi*sthe) + input.at(2)*cthe);
    
    return transformed;
    
}

void average_reflections(const FourierMultiVolume& reflections, FourierVolume& volume)
{
    
    bool initialized = false;
    
    Index3d current_index;
    std::vector<element::Complex<double> > current_reflections;
    
    for(FourierMultiVolume::const_iterator spot_itr=reflections.begin(); spot_itr!=reflections.end(); ++spot_itr)
    {
        // Initialize for the start
        if(!(initialized))
        {
            current_index = (*spot_itr).first;
            initialized = true;
        }
        
        // Average and insert accumulated spots
        if(!(current_index == (*spot_itr).first))
        {
            element::Complex<double> avg_spot = std::accumulate(current_reflections.begin(), current_reflections.end(), element::Complex<double>(0.0, 0.0)) * (1/current_reflections.size());
                        
            volume.insert(current_index, avg_spot);
            current_reflections.clear();
        }
        
        //Accumulate the spots in a list
        current_reflections.push_back((*spot_itr).second);
        current_index = (*spot_itr).first;
        
    }
    
    //insert the final reflection
    element::Complex<double> avg_spot = std::accumulate(current_reflections.begin(), current_reflections.end(), element::Complex<double>(0.0, 0.0)) * (1/current_reflections.size());
    volume.insert(current_index, avg_spot);
}


int main(int argc, char** argv) {

    if(argc < 4) {
        std::cerr << "Usage:\n\t./" << argv[0] << " <PARTICLE STACK FILE> <PARAMETER FILE> <OUTPUT VOLUME FILE>\n\n";
        exit(1);
    }
    
    // Reading the stack
    cout << "Reading the particles...\n";
    Stack particles;
    PropertiesMap header_values;
    MRCFile(argv[1]).load(particles, header_values);
    
    // Read the par file
    cout << "Reading the parameters file...\n";
    Table par_table;
    par_table.read_table(argv[1], 6, ' ', 'C');
    
    // Calculate grid size
    int num_particles = particles.range().at(2);
    int columns = particles.range().at(0);
    int rows = particles.range().at(1);

    // Gather data for threading
    int num_threads = thread::hardware_concurrency();

    int thread_load = num_particles / num_threads;
    int extra_load = num_particles % num_threads;

    std::cout << "Running on " << num_threads << " threads\n";
    std::cout << "Thread load: " << thread_load << endl;

    // Locks for concurrent processing
    std::vector<std::thread> threads(num_threads);
    std::mutex critical;

    // Fourier Volume to gather data
    int sections = max(columns, rows);
    FourierMultiVolume reflections;
    
    // Create transformers containing FFTW plans with required sizes
    auto f_transformer = em::fft::FFTEnvironment::Instance().new_transformer({columns, rows});
    
    for (int t = 0; t < num_threads; ++t) {
        int begin = t * thread_load;
        int end = (t + 1) * thread_load;

        //Last one has to take the extra load
        if (t == num_threads - 1) end += extra_load;
        threads[t] = thread(bind([&](int begin, int end) {
            
            for (int particle = begin; particle < end; ++particle) {
                std::cout << "Processing particle " << particle << std::endl;
                Image image = particles.slice(particle);
                FourierImage fourier_image;
                               
                std::vector<double> pars = par_table.get_row<double>(particle);
                double psi = pars.at(1)*M_PI/180;
                double theta = pars.at(2)*M_PI/180;
                double phi = pars.at(3)*M_PI/180;
                double x_shift = pars.at(4);
                double y_shift = pars.at(5);
                
                fourier_transform(image, fourier_image, f_transformer);
                
                // Do the phase shift
                fourier_image = phase_shift(fourier_image, x_shift, y_shift);
                
                // Transform the indices and place in 3D Fourier space
                for(const auto& itr : fourier_image) {
                    Index2d im_idx = itr.index();
                    Index3d vol_idx = tranformed_index(Index3d{im_idx.at(0), im_idx.at(1), 0}, psi, theta, phi);
                    
                    // Synchronously write to the 3D multi reflections volume
                    critical.lock();
                    if(itr.value().amplitude() > 0.0001) reflections.insert(IndexComplexPair(vol_idx, itr.value()));
                    critical.unlock();
                }
            }
        }, begin, end));
    }

    for (thread& t : threads) t.join();

    std::cout << "Averaging the reflections\n";
    FourierVolume final_fourier = FourierVolume(Index3d({columns, rows, sections}));
    average_reflections(reflections, final_fourier);
    
    std::cout << "Inverse Fourier transforming the final volume\n";
    Volume output;
    fourier_transform(final_fourier, output);
    
    std::cout << "Writing the output volume: " << argv[3];
    MRCFile(argv[3]).save(output, header_values);
    
    return 0;
    
}
