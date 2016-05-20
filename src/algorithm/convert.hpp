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

#ifndef EM_TENSOR_CONVERT_HPP
#define EM_TENSOR_CONVERT_HPP

#include <iostream>
#include <cassert>
#include <type_traits>

namespace em {
    
    namespace algorithm {

        /**
         * Tensor Storage Order Converter
         * @param input
         * @param output
         */
        template<typename TensorInputType_, typename TensorOutputType_>
        typename std::enable_if< std::is_same<typename TensorInputType_::data_type, typename TensorOutputType_::data_type>::value 
                        && TensorInputType_::rank == TensorOutputType_::rank, void>::type
        convert(const TensorInputType_& input, TensorOutputType_& output) {

            assert(input.range() == output.range());

            for (const auto& itr : input) {
                //auto idx = TensorOutputType_::arranger_type::map(itr.index(), output.range(), output.origin());
                //std::cout << itr.index() << " -> " << TensorOutputType_::arranger_type::map(idx, output.range(), output.origin()) << std::endl;
                output[itr.index()] = itr.value();
            }

        }

    }
}

#endif /* CONVERT_HPP */

