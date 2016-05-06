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

namespace em {
    
    //Real to Complex conversion
    
    template<typename ValueType_, size_t rank_>
    void convert(const multidim::Tensor<ValueType_, rank_, multidim::StorageOrder::COLUMN_MAJOR>& real, 
                        multidim::Tensor<Complex<ValueType_>, rank_, multidim::StorageOrder::COLUMN_MAJOR>& complex) {
        std::cout << "Conversion: REAL -> COMPLEX\n";
        
        //Do the FFT
        std::vector<int> sizes;
        for(int i=0; i< rank_; ++i) sizes.push_back((int)real.range()[i]);
        auto output = FFTEnvironment::Instance().get_transformer()->forward_fourier(sizes, real.vectorize());
        
        //Create the complex valued tensor
        auto cmplx_range = real.range();
        cmplx_range[0] = cmplx_range[0]/2+1; // Half Space
        complex = multidim::Tensor<Complex<ValueType_>, rank_>(cmplx_range, Complex<ValueType_>());
        
        for(int id=0; id<cmplx_range.size(); id++)
        {
            complex[id] = Complex<ValueType_>( (ValueType_) output[2*id], (ValueType_) output[2*id+1]);
        };
    }
    
}

#endif /* CONVERT_HPP */

