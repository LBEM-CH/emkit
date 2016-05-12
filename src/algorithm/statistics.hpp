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

#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <iterator>

namespace em {

    namespace algorithm {

        template<typename TensorIterator_>
        auto max(TensorIterator_ begin, TensorIterator_ end) -> typename std::iterator_traits<TensorIterator_>::value_type {
            using pair_type = typename std::iterator_traits<TensorIterator_>::value_type;
            return *std::max_element(begin, end,
                    [] (const pair_type & p1, const pair_type & p2) {
                        return p1.value() < p2.value();
                    });
        }
        
        template<typename TensorIterator_>
        auto min(TensorIterator_ begin, TensorIterator_ end) -> typename std::iterator_traits<TensorIterator_>::value_type {
            using pair_type = typename TensorIterator_::value_type;
            return *std::max_element(begin, end,
                    [] (const pair_type & p1, const pair_type & p2) {
                        return p1.value() > p2.value();
                    });
        }
        
        template<typename TensorIterator_>
        auto max_value(TensorIterator_ begin, TensorIterator_ end) -> typename std::iterator_traits<TensorIterator_>::value_type::value_type {
            return max(begin, end).value();
        }
        
        template<typename TensorIterator_>
        auto min_value(TensorIterator_ begin, TensorIterator_ end) -> typename std::iterator_traits<TensorIterator_>::value_type::value_type {
            return min(begin, end).value();
        }
        
        template<typename TensorIterator_>
        auto max_index(TensorIterator_ begin, TensorIterator_ end) -> typename std::iterator_traits<TensorIterator_>::value_type::index_type {
            return max(begin, end).index();
        }
        
        template<typename TensorIterator_>
        auto min_index(TensorIterator_ begin, TensorIterator_ end) -> typename std::iterator_traits<TensorIterator_>::value_type::index_type {
            return min(begin, end).index();
        }
        

    }
}

#endif /* STATISTICS_HPP */

