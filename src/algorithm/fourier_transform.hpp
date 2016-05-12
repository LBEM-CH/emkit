/* 
 * This file is a part of emkit.
 * 
 * emkit is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or any 
 * later version.
 * 
 * emkit is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public 
 * License for more details <http://www.gnu.org/licenses/>
 * 
 * Author:
 * Nikhil Biyani: nikhil(dot)biyani(at)gmail(dot)com
 * 
 */

#ifndef FOURIER_TRANSFORM_HPP
#define FOURIER_TRANSFORM_HPP

#include "../modules/fft/fft_environment.hpp"
#include "../elements/complex.hpp"
#include "../elements/tensor.hpp"
#include "../elements/range.hpp"
#include "../elements/tensor_storage_order.hpp"
#include "../objects/real_object.hpp"
#include "../objects/complex_object.hpp"


namespace em {
    
    namespace algorithm {
        
        /**
         * REAL to COMPLEX FFT Tensor conversion
         * @param logical_range
         * @param real
         * @param complex
         */
        template<typename ValueType_, size_t rank_>
        void fourier_transform(const element::Range<rank_>& logical_range,
                const element::Tensor<ValueType_, rank_, element::StorageOrder::COLUMN_MAJOR>& real,
                element::Tensor<element::Complex<ValueType_>, rank_, element::StorageOrder::COLUMN_MAJOR>& complex) {

            //Check the provided size
            assert(logical_range.size() <= real.range().size());

            //Do the FFT
            std::vector<int> sizes;
            for (int i = 0; i < rank_; ++i) sizes.push_back((int) logical_range[i]);
            auto output = fft::FFTEnvironment::Instance().get_transformer()->forward_fourier(sizes, real.vectorize());

            //Create the complex valued tensor
            element::Range<rank_> complex_container_range = logical_range;
            complex_container_range[0] = complex_container_range[0] / 2 + 1;
            complex = element::Tensor<element::Complex<ValueType_>, rank_, element::StorageOrder::COLUMN_MAJOR>(complex_container_range, element::Complex<ValueType_>());

            for (int id = 0; id < complex.range().size(); id++) {
                complex[id] = element::Complex<ValueType_>((ValueType_) output[2 * id], (ValueType_) output[2 * id + 1]);
            };
        }

        /**
         * COMPLEX TO REAL InverseFFT Tensor conversion
         * @param logical_range
         * @param complex
         * @param real
         */
        template<typename ValueType_, size_t rank_>
        void fourier_transform(const element::Range<rank_>& logical_range,
                const element::Tensor<element::Complex<ValueType_>, rank_, element::StorageOrder::COLUMN_MAJOR>& complex,
                element::Tensor<ValueType_, rank_, element::StorageOrder::COLUMN_MAJOR>& real) {

            //Check the provided size
            assert(logical_range.size() <= complex.range().size()*2);

            std::vector<double> input;
            for (int id = 0; id < complex.range().size(); id++) {
                input[2 * id] = (double) complex[id].real();
                input[2 * id + 1] = (double) complex[id].imag();
            };

            //Do the FFT
            std::vector<int> sizes;
            for (int i = 0; i < rank_; ++i) sizes.push_back((int) logical_range[i]);
            auto output = fft::FFTEnvironment::Instance().get_transformer()->inverse_fourier(sizes, input);

            //Create the complex valued tensor
            real = element::Tensor<ValueType_, rank_, element::StorageOrder::COLUMN_MAJOR>(logical_range, output);
        }
        
        template<typename ValueType_, size_t rank_>
        void fourier_transform(const object::RealObject<ValueType_, rank_>& real, 
                               object::ComplexObject<ValueType_, rank_>& complex) {
            complex = object::ComplexObject<ValueType_, rank_>(real.range());
            fourier_transform(real.range(), real.container(), complex.container());
        }
        
        template<typename ValueType_, size_t rank_>
        void fourier_transform(const object::ComplexObject<ValueType_, rank_>& complex, 
                               object::RealObject<ValueType_, rank_>& real) {
            real = object::RealObject<ValueType_, rank_>(complex.logical_range());
            fourier_transform(real.range(), complex.container(), real.container());
        }
    }
}

#endif /* FOURIER_TRANSFORM_HPP */

