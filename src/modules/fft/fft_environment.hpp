/* 
 * File:   FourierTransformEnvironment.hpp
 * Author: biyanin
 *
 * Created on March 21, 2016, 10:56 AM
 */

#ifndef FFT_ENVIRONMENT_HPP
#define FFT_ENVIRONMENT_HPP

#include "fft_interface.hpp"
#include "fourier_transform_fftw.hpp"

#include <memory>

namespace em {
    namespace fft {

        /**
         * A singleton class to provide environment of the correct
         * FFT class using installation variables.
         */
        class FFTEnvironment {
        public:

            static FFTEnvironment Instance() {
                static FFTEnvironment instance;
                return instance;
            };

            std::shared_ptr<FFTInterface> global_transformer() {
                return _transformer;
            };
            
            static std::shared_ptr<FFTInterface> new_transformer() {
                //The default transformer
                return std::shared_ptr<FFTInterface>(new FourierTransformFFTW());
#ifdef TDX_USE_CUDA
                //Use CUDA counterpart of the Fourier transform.
#endif
            }

        private:

            FFTEnvironment() {
                _transformer = new_transformer();
            };
            
            std::shared_ptr<FFTInterface> _transformer;
        };
    }
}


#endif /* FFT_ENVIRONMENT_HPP */

