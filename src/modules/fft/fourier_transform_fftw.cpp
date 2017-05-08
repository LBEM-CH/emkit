/* 
 * Author: Nikhil Biyani - nikhil(dot)biyani(at)gmail(dot)com
 *
 * This file is a part of 2dx.
 * 
 * 2dx is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or any 
 * later version.
 * 
 * 2dx is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public 
 * License for more details <http://www.gnu.org/licenses/>.
 */

#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>

#include "fourier_transform_fftw.hpp"

using namespace em::fft;

std::mutex FourierTransformFFTW::mutex_;

FourierTransformFFTW::FourierTransformFFTW()
{
    sizes_ = std::vector<int>();
    _initialized = false;
    
    //Initialize the threaded environment in FFTW
    fftw_init_threads();
    
    //Set maximum number of threads
    fftw_plan_with_nthreads(std::thread::hardware_concurrency());
}

FourierTransformFFTW::FourierTransformFFTW(const std::vector<int>& sizes)
{
    sizes_ = sizes;
    _initialized = false;
    
    //Initialize the threaded environment in FFTW
    fftw_init_threads();
    
    //Set maximum number of threads
    fftw_plan_with_nthreads(std::thread::hardware_concurrency());
    
    //Create plans
    create_plans();
}

FourierTransformFFTW::FourierTransformFFTW(const FourierTransformFFTW& copy)
{
    reset(copy);
}

FourierTransformFFTW::~FourierTransformFFTW()
{
    clear();
    fftw_cleanup_threads();
    fftw_cleanup();
}

void FourierTransformFFTW::reset(const FourierTransformFFTW& copy)
{  
    if(sizes_ != copy.sizes_)
    {
        sizes_ = copy.sizes_;
        
        clear();
        
        if(copy._initialized)
        {
            create_plans();
        }
    }
    
}

void FourierTransformFFTW::clear()
{
    if(_initialized)
    {
        
        fftw_free(_real_data);
        fftw_free(_complex_data);
        fftw_destroy_plan(_plan_r2c);
        fftw_destroy_plan(_plan_c2r);
        
    }
    _initialized = false;
}

void FourierTransformFFTW::create_plans()
{
    //std::cout << "Creating plans\n";
    mutex_.lock();
    clear();
    _real_data = fftw_alloc_real(real_size());
    _complex_data =  fftw_alloc_complex(fourier_size());
    
    _plan_r2c = fftw_plan(fftw_plan_dft_r2c(sizes_.size(), reversed_sizes().data(), _real_data, _complex_data, FFTW_MEASURE));
    _plan_c2r = fftw_plan(fftw_plan_dft_c2r(sizes_.size(), reversed_sizes().data(), _complex_data, _real_data, FFTW_MEASURE));
    _initialized = true;
    mutex_.unlock();
    
    //std::cout << "Finished creating plans\n";
}

FourierTransformFFTW& FourierTransformFFTW::operator=(const FourierTransformFFTW& rhs)
{
    reset(rhs);
    return *this;
}

std::vector<int> FourierTransformFFTW::reversed_sizes() const {
    std::vector<int> rev = sizes_;
    std::reverse(rev.begin(), rev.end());
    return rev;
}

size_t FourierTransformFFTW::real_size() const
{
    size_t sz = 1;
    for(int i=0; i<sizes_.size(); ++i) {
        sz *= sizes_[i];
    }
    return sz;
}

size_t FourierTransformFFTW::fourier_size() const
{
    size_t sz = sizes_[0]/2+1;
    for(int i=1; i<sizes_.size(); ++i) {
        sz *= sizes_[i];
    }
    return sz;
}

double FourierTransformFFTW::normalization_factor()
{
    if(real_size() < 1) return 1;
    else {
        return 1.0/sqrt((double)real_size());
    }
}

std::vector<double> FourierTransformFFTW::forward_fourier(const std::vector<int>& sizes, const std::vector<double>& input)
{
    //std::cout << "Forward transform\n";
    //Re-plan if required
    if(!_initialized || sizes_ != sizes)
    {
        sizes_ = sizes;
        create_plans();
    }
    
    #pragma omp parallel for
    for(int id=0; id<real_size(); id++) _real_data[id] = input[id];
    
    //Execute the plan
    fftw_execute(_plan_r2c);
    
    std::vector<double> output(fourier_size()*2, 0.0);
    
    //Normalize and assign
    double factor = this->normalization_factor();
    
    #pragma omp parallel for
    for(int id=0; id<fourier_size(); id++)
    {
        output[2*id] = ((fftw_complex*)_complex_data)[id][0] * factor;
        output[2*id+1] = ((fftw_complex*)_complex_data)[id][1] * -1 * factor;
    }
    //std::cout << "Finished forward\n";
    return output;
}

std::vector<double> FourierTransformFFTW::inverse_fourier(const std::vector<int>& sizes, const std::vector<double>& input)
{
    //std::cout << "Inverse transform\n";
    //Re-plan if required
    if(!_initialized || sizes_ != sizes)
    {
        sizes_ = sizes;
        create_plans();
    }
    
    //Normalize
    double factor = this->normalization_factor();
    
    #pragma omp parallel for
    for(int id=0; id<fourier_size(); id++)
    {
        ((fftw_complex*)_complex_data)[id][0] = input[2*id] * factor;
        ((fftw_complex*)_complex_data)[id][1] = input[2*id+1] * -1* factor;
    };

    //Execute the plan
    fftw_execute(_plan_c2r);
    
    std::vector<double> output(real_size());
    
    #pragma omp parallel for
    for(int id=0; id<real_size(); id++) output[id] = _real_data[id];
    //std::cout << "Finished inverse\n";
    return output;
}