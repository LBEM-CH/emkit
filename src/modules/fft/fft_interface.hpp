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

#ifndef FOURIER_TRANSFORMER_HPP_AX89
#define FOURIER_TRANSFORMER_HPP_AX89

#include <vector>

namespace em {
    
    namespace fft {

        /**
         * An abstract class (interface) to provide Fourier transform method.
         */
        class FFTInterface {
        public:
            /**
             * A method which provides a Fourier transform on the input and generates
             * the output
             * @param nx
             * @param ny
             * @param nz
             * @param[in] input: All the real space data in column major order
             * @param[out] output: Column major order with size of (nz*ny(nx/2+1)*2) 
             *                     where each even index in x 0,2,4,.. 
             *                     provides values of real coefficients.
             *                     and each odd index in x: 1,3,5,..
             *                     provides values of imaginary coefficients.   
             */
            virtual std::vector<double> forward_fourier(const std::vector<int>& sizes, const std::vector<double>& input) = 0;

            /* A method which provides a inverse Fourier transform on the input and generates
             * the output
             * @param nx
             * @param ny
             * @param nz
             * @param[in] input: Column major order with size of (nz*ny(nx/2+1)*2) 
             *                     where each even index in x 0,2,4,.. 
             *                     provides values of real coefficients.
             *                     and each odd index in x: 1,3,5,..
             *                     provides values of imaginary coefficients. 
             * @param[out] output: All the density values in column major order  
             */
            virtual std::vector<double> inverse_fourier(const std::vector<int>& sizes, const std::vector<double>& input) = 0;


        };
    }
}

#endif /* FOURIERTRANSFORM_HPP */

