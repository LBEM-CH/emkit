/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   tensor_format.hpp
 * Author: biyanin
 *
 * Created on April 22, 2016, 4:28 PM
 */

#ifndef TENSOR_FORMAT_HPP
#define TENSOR_FORMAT_HPP

namespace em {
    namespace multidim {
        
        enum class TensorFormat {
            COLUMN_MAJOR,   //FORTRAN/MATLAB LIKE 
            ROW_MAJOR       //C++ LIKE
        };
    }
}

#endif /* TENSOR_FORMAT_HPP */

