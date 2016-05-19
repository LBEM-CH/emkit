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

#ifndef OBJECT_BASE_TYPES_HPP
#define OBJECT_BASE_TYPES_HPP

#include <type_traits>

#include "../elements/complex.hpp"
#include "../elements/tensor.hpp"

namespace em {
    
    namespace object {
        
        template<typename DataType_>
        struct is_real_valued {
            static const bool value = std::is_fundamental<DataType_>::value;
        };
        
        /*
        template<typename DataType_>
        struct is_complex_valued {
            static const bool value = false;
        };
        
        template<>
        template<typename ValueType_>
        struct is_complex_valued<element::Complex<ValueType_> {
            static const bool value = true;
        };
        */
        
        template<typename ObjectType_>
        struct object_traits {
            static const size_t rank = ObjectType_::rank;
            typedef typename ObjectType_::index_type index_type;
            typedef typename ObjectType_::data_type data_type;
        };
        
        
        template<typename ValueType_, size_t rank_>
        using RealObject = element::Tensor<ValueType_, rank_, element::StorageOrder::COLUMN_MAJOR>;
        
        
        template<typename ValueType_, size_t rank_>
        using ComplexObject = element::Tensor<element::Complex<ValueType_>, rank_, element::StorageOrder::COLUMN_MAJOR>;
        
    }
}

#endif /* OBJECT_BASE_TYPES_HPP */

