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

#ifndef MATRIX_MULTIPLICATION_HPP
#define MATRIX_MULTIPLICATION_HPP

#include "../elements/tensor.hpp"
#include "../elements/tensor_storage_order.hpp"
#include "../objects/object_base_types.hpp"
#include "../objects/complex_half_object.hpp"

namespace em {
    
    namespace algorithm {
    
        template<typename ValueType_, element::StorageOrder order_>
        element::Tensor<ValueType_, 2, order_>
        matrix_multiply(const element::Tensor<ValueType_, 2, order_>& mat1, const element::Tensor<ValueType_, 2, order_>& mat2){
            assert(mat1.range().at(1) == mat2.range().at(0));
            int r1 = mat1.range().at(0);
            int c1 = mat1.range().at(1);
            int c2 = mat1.range().at(1);
            
            element::Tensor<ValueType_, 2, order_> mult(Index2d{r1, c2}, ValueType_());
            
            for(int i = 0; i < r1; ++i) {
                for(int j = 0; j < c2; ++j) {
                    for(int k = 0; k < c1; ++k) {
                        mult[i][j] += mat1[i][k] * mat2[k][j];
                    }
                }
            }
            
            return mult;
            
        }
        
    }
}





#endif /* MATRIX_MULTIPLICATION_HPP */

