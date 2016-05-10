/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   convert.hpp
 * Author: biyanin
 *
 * Created on May 4, 2016, 8:08 PM
 */

#ifndef CONVERT_HPP
#define CONVERT_HPP

#include "../../multidim/tensor.hpp"
#include "../../multidim/range.hpp"
#include "../../multidim/storage_order.hpp"
#include "../fourier_transforms/fft_environment.hpp"
#include "../../helper/complex.hpp"
#include "../../components/real_object.hpp"
#include "../../components/complex_object.hpp"

namespace em {
    
    /**
     * ROW MAJOR to COLUMN MAJOR converter
     * @param input
     * @param output
     */
    template<typename TensorInputType_, typename TensorOutputType_>
    void convert(const TensorInputType_& input,
                       TensorOutputType_& output) {
        
        assert(input.range() == output.range());
        
        for(const auto& itr : input) {
            output[itr.index()] = itr.value();
        }
        
    }
    
    
    /**
     * REAL to COMPLEX FFT conversion
     * @param logical_range
     * @param real
     * @param complex
     */
    template<typename ValueType_, size_t rank_>
    void convert(const multidim::Range<rank_>& logical_range,
                 const multidim::Tensor<ValueType_         , rank_, multidim::StorageOrder::COLUMN_MAJOR>& real, 
                       multidim::Tensor<Complex<ValueType_>, rank_, multidim::StorageOrder::COLUMN_MAJOR>& complex) {
        
        //Check the provided size
        assert(logical_range.size() <= real.range().size());
        
        //Do the FFT
        std::vector<int> sizes;
        for(int i=0; i< rank_; ++i) sizes.push_back((int)logical_range[i]);
        auto output = FFTEnvironment::Instance().get_transformer()->forward_fourier(sizes, real.vectorize());
        
        //Create the complex valued tensor
        multidim::Range<rank_> complex_container_range = logical_range;
        complex_container_range[0] = complex_container_range[0] / 2 + 1;
        complex = multidim::Tensor<Complex<ValueType_>, rank_, multidim::StorageOrder::COLUMN_MAJOR>(complex_container_range, Complex<ValueType_>());
        
        for(int id=0; id<complex.range().size(); id++)
        {
            complex[id] = Complex<ValueType_>( (ValueType_) output[2*id], (ValueType_) output[2*id+1]);
        };
    }
    
    /**
     * COMPLEX TO REAL InverseFFT conversion
     * @param logical_range
     * @param complex
     * @param real
     */
    template<typename ValueType_, size_t rank_>
    void convert(const multidim::Range<rank_>& logical_range,
                 const multidim::Tensor<Complex<ValueType_>, rank_, multidim::StorageOrder::COLUMN_MAJOR>& complex,
                       multidim::Tensor<ValueType_         , rank_, multidim::StorageOrder::COLUMN_MAJOR>& real) {
        
        //Check the provided size
        assert(logical_range.size() <= complex.range().size()*2);
        
        std::vector<double> input;
        for(int id=0; id<complex.range().size(); id++)
        {
            input[2*id] = (double) complex[id].real();
            input[2*id+1] = (double) complex[id].imag();
        };
        
        //Do the FFT
        std::vector<int> sizes;
        for(int i=0; i< rank_; ++i) sizes.push_back((int) logical_range[i]);
        auto output = FFTEnvironment::Instance().get_transformer()->inverse_fourier(sizes, input);
        
        //Create the complex valued tensor
        real = multidim::Tensor<ValueType_,rank_,multidim::StorageOrder::COLUMN_MAJOR>(logical_range, output);
    }
    
}

#endif /* CONVERT_HPP */

