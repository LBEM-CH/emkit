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

#ifndef FOURIER_TRANSFORM_FFTW_HPP
#define FOURIER_TRANSFORM_FFTW_HPP

#include <iostream>
#include <math.h>
#include <memory>

#include <fftw3.h>

#include "fft_interface.hpp"

namespace em {
    
    namespace fft {

        /**
         * A class used for FFTW3 wisdom and Fourier Transforms.
         * Objective: Class which can provide methods for doing Fourier 
         * transforms using FFTW3 lib. The useful functionality of wisdom 
         * is used by recycling plans if possible.
         * Semantics: Uses the same plan if the size of the input remains 
         * same, otherwise generates a new plan using FFTW_ESTIMATE
         * Use of same class is recommended if using same data size
         */
        class FourierTransformFFTW : public FFTInterface {
        public:
            /**
             * Default constructor
             * Sets the size variables to 0
             */
            FourierTransformFFTW();

            /**
             * Copy constructor
             */
            FourierTransformFFTW(const FourierTransformFFTW& copy);

            /**
             * Default destructor
             */
            ~FourierTransformFFTW();

            /**
             * Definition of operator =
             */
            FourierTransformFFTW& operator=(const FourierTransformFFTW& rhs);

            /**
             * Resets the instance with the other
             */
            void reset(const FourierTransformFFTW& other);

            /**
             * Clears the data
             */
            void clear();

            /**
             * Method to implement the function to provide forward Fourier
             * transform from the abstract class.
             * 
             * Transform the input real data to complex data.
             * Internally uses FFTW r2c plans and executions for the
             * conversion. 
             * 
             * The input real data is with origin at the 
             * lower left corner. x is the fastest changing direction 
             * and z is the slowest changing direction.
             * 
             * The output complex data with x dimension trimmed to half 
             * (due to the symmetric nature of the output). 
             * The origin of the frequency is in the lower left corner, 
             * followed by positive frequencies and the terminal indices
             * gives highest frequency values. Eg. order for 1D:
             *  Memory order: 0  1  2  3  4  5  6  7
             *  Frequencies : 0  1  2  3  4 -3 -2 -1
             * 
             * @param nx: size of x-dimension
             * @param ny: size of y-dimension
             * @param nz: size of z-dimension
             * @param[in] real input data
             * @param[out] output complex data
             */
            std::vector<double> forward_fourier(const std::vector<int>& sizes, const std::vector<double>& input) override;

            /**
             * Method to implement the function to provide inverse Fourier
             * transform from the abstract class.
             * 
             * Transform the input real data to complex data.
             * Internally uses FFTW c2r plans and executions for the
             * conversion. 
             * 
             * The input complex data with x dimension trimmed to half 
             * (due to the symmetric nature of Fourier space). 
             * The origin of the frequency is in the lower left corner, 
             * followed by positive frequencies and the terminal indices
             * gives highest frequency values. Eg. order for 1D:
             *  Memory order: 0  1  2  3  4  5  6  7
             *  Frequencies : 0  1  2  3  4 -3 -2 -1
             * 
             * The output real data is with origin at the 
             * lower left corner. x is the fastest changing direction 
             * and z is the slowest changing direction.
             * 
             * @param nx: size of x-dimension
             * @param ny: size of y-dimension
             * @param nz: size of z-dimension
             * @param[in] input complex data
             * @param[out] output real data
             */
            std::vector<double> inverse_fourier(const std::vector<int>& sizes, const std::vector<double>& input) override;

        private:

            /**
             * Creates the plans.
             */
            void create_plans();

            /**
             * Returns the normalization factor needed to scale the complex data
             * @return normalization factor
             */
            double normalization_factor();

            /**
             * Returns the size of the fftw_complex for the corresponding size.
             * The Fourier space is trimmed to half as is symmetric. So this
             * function returns this size of the trimmed space
             * @return Fourier size
             */
            size_t fourier_size() const;

            size_t real_size() const;

            std::vector<int> reversed_sizes() const;


            /*===================
             * Members
             ====================*/

            /**
             * Holds the plan for real to complex conversion
             */
            fftw_plan _plan_r2c;

            /**
             * Holds the plan for complex to real conversion
             */
            fftw_plan _plan_c2r;

            /**
             * Holds the size of the transform
             */
            std::vector<int> sizes_;

            /**
             * Holds the real space data allocated using fftw_malloc
             */
            double* _real_data;

            /**
             * Holds the complex data allocated using fftw_malloc 
             */
            fftw_complex* _complex_data;

            /**
             * Flag to check if the members are initialized
             */
            bool _initialized;



        }; // class FourierTransformFFTW

    }

}
#endif /* FOURIER_TRANSFORM_FFTW_HPP */

