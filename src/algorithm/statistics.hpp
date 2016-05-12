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
        typename std::iterator_traits<TensorIterator_>::value_type
        max(TensorIterator_ begin, TensorIterator_ end) {
            using pair_type = typename std::iterator_traits<TensorIterator_>::value_type;
            return *std::max_element(begin, end,
                    [] (const pair_type & p1, const pair_type & p2) {
                        return p1.value() < p2.value();
                    });
        }
        
        template<typename TensorIterator_>
        typename std::iterator_traits<TensorIterator_>::value_type
        min(TensorIterator_ begin, TensorIterator_ end) {
            using pair_type = typename TensorIterator_::value_type;
            return *std::max_element(begin, end,
                    [] (const pair_type & p1, const pair_type & p2) {
                        return p1.value() > p2.value();
                    });
        }
        
        template<typename TensorIterator_>
        typename std::iterator_traits<TensorIterator_>::value_type::value_type
        max_value(TensorIterator_ begin, TensorIterator_ end) {
            return max(begin, end).value();
        }
        
        template<typename TensorIterator_>
        typename std::iterator_traits<TensorIterator_>::value_type::value_type
        min_value(TensorIterator_ begin, TensorIterator_ end) {
            return min(begin, end).value();
        }
        
        template<typename TensorIterator_>
        typename std::iterator_traits<TensorIterator_>::value_type::index_type
        max_index(TensorIterator_ begin, TensorIterator_ end) {
            return max(begin, end).index();
        }
        
        template<typename TensorIterator_>
        typename std::iterator_traits<TensorIterator_>::value_type::index_type
        min_index(TensorIterator_ begin, TensorIterator_ end) {
            return min(begin, end).index();
        }

        template<typename TensorIterator_>
        typename std::iterator_traits<TensorIterator_>::value_type::value_type
        sum(TensorIterator_ begin, TensorIterator_ end) {
            using pair_type = typename TensorIterator_::value_type;
            using value_type = typename pair_type::value_type;
            return std::accumulate(begin, end, value_type(),
                    [] (int total, const pair_type & p) {
                        return total + p.value();
                    });
        }
        
        template<typename TensorIterator_>
        typename std::iterator_traits<TensorIterator_>::value_type::value_type
        squared_sum(TensorIterator_ begin, TensorIterator_ end) {
            using value_type = typename TensorIterator_::value_type::value_type;
            value_type ss;
            for(auto itr = begin; itr != end; ++itr) {
                ss += itr->value() * itr->value();
            }
            return ss;
        }
        
        template<typename TensorIterator_>
        typename std::iterator_traits<TensorIterator_>::value_type::value_type
        mean(TensorIterator_ begin, TensorIterator_ end) {
            return sum(begin, end)/std::abs(std::distance(begin, end));
        }
        
        template<typename TensorIterator_>
        typename std::iterator_traits<TensorIterator_>::value_type::value_type
        standard_deviation(TensorIterator_ begin, TensorIterator_ end) {
            auto ss = squared_sum(begin, end);
            auto len = std::abs(std::distance(begin, end));
            auto mn = mean(begin, end);
            return std::sqrt(ss/len - mn*mn);
        }

    }
}

#endif /* STATISTICS_HPP */

