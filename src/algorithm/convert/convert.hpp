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
    
    //Real to Complex conversion
    
    template<typename ValueType_, size_t rank_>
    void convert(const RealObject<ValueType_, rank_>& real, ComplexObject<ValueType_, rank_>& complex) {
        std::cout << "Conversion: REAL -> FOURIER\n";
        
        //Do the FFT
        std::vector<int> sizes;
        for(int i=0; i< rank_; ++i) sizes.push_back((int)real.range()[i]);
        auto output = FFTEnvironment::Instance().get_transformer()->forward_fourier(sizes, real.container().vectorize());
        
        //Create the complex valued tensor
        complex = ComplexObject<ValueType_, rank_>(real.range());
        
        for(int id=0; id<complex.range().size(); id++)
        {
            complex[id] = Complex<ValueType_>( (ValueType_) output[2*id], (ValueType_) output[2*id+1]);
        };
    }
    
    template<typename ValueType_, size_t rank_>
    void convert(const ComplexObject<ValueType_, rank_>& complex, RealObject<ValueType_, rank_>& real) {
        std::cout << "Conversion: FOURIER -> REAL\n";
        
        std::vector<double> input;
        for(int id=0; id<complex.range().size(); id++)
        {
            input[2*id] = (double) complex[id].real();
            input[2*id+1] = (double) complex[id].imag();
        };
        
        //Do the FFT
        std::vector<int> sizes;
        for(int i=0; i< rank_; ++i) sizes.push_back((int)complex.logical_range()[i]);
        auto output = FFTEnvironment::Instance().get_transformer()->inverse_fourier(sizes, input);
        
        //Create the complex valued tensor
        auto tensor = multidim::Tensor<ValueType_,rank_,multidim::StorageOrder::COLUMN_MAJOR>(complex.logical_range(), output);
        real = RealObject<ValueType_, rank_>(tensor);
    }
    
}

#endif /* CONVERT_HPP */

