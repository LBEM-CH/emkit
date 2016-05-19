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

#ifndef ELEMENT_H
#define ELEMENT_H

#include "../src/elements/file.hpp"
#include "../src/elements/binary_file.hpp"
#include "../src/elements/complex.hpp"
#include "../src/elements/string.hpp"
#include "../src/elements/properties_map.hpp"
#include "../src/elements/index.hpp"
#include "../src/elements/tensor.hpp"
#include "../src/elements/tensor_iterator.hpp"

namespace em {
    
    using namespace element;
    
    using ComplexF = Complex<float>;
    using ComplexD = Complex<double>;
    
    using Index2d = Index<2>;
    using Index3d = Index<3>;
    
    using TensorF2dC = Tensor<float, 2, StorageOrder::COLUMN_MAJOR>;
    using TensorF3dC = Tensor<float, 3, StorageOrder::COLUMN_MAJOR>;
    using TensorF2dR = Tensor<float, 2, StorageOrder::ROW_MAJOR>;
    using TensorF3dR = Tensor<float, 3, StorageOrder::ROW_MAJOR>;
    using TensorD2dC = Tensor<double, 2, StorageOrder::COLUMN_MAJOR>;
    using TensorD3dC = Tensor<double, 3, StorageOrder::COLUMN_MAJOR>;
    using TensorD2dR = Tensor<double, 2, StorageOrder::ROW_MAJOR>;
    using TensorD3dR = Tensor<double, 3, StorageOrder::ROW_MAJOR>;
    
    using TensorCF2dC = Tensor<ComplexF, 2, StorageOrder::COLUMN_MAJOR>;
    using TensorCF3dC = Tensor<ComplexF, 3, StorageOrder::COLUMN_MAJOR>;
    using TensorCF2dR = Tensor<ComplexF, 2, StorageOrder::ROW_MAJOR>;
    using TensorCF3dR = Tensor<ComplexF, 3, StorageOrder::ROW_MAJOR>;
    using TensorCD2dC = Tensor<ComplexD, 2, StorageOrder::COLUMN_MAJOR>;
    using TensorCD3dC = Tensor<ComplexD, 3, StorageOrder::COLUMN_MAJOR>;
    using TensorCD2dR = Tensor<ComplexD, 2, StorageOrder::ROW_MAJOR>;
    using TensorCD3dR = Tensor<ComplexD, 3, StorageOrder::ROW_MAJOR>;
    
}

#endif /* ELEMENT_H */

