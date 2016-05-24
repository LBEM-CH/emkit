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

#ifndef MILLER_INDEX_HPP
#define MILLER_INDEX_HPP

#include "index.hpp"


namespace em {

    namespace element {

        template<size_t rank_>
        class MillerIndex : public Index<rank_> {
        public:

            using base_type = Index<rank_>;

            static const size_t rank = rank_;
            using value_type = typename base_type::value_type;
            using size_type = typename base_type::size_type;

            MillerIndex()
            : base_type() {
            };

            MillerIndex(const value_type& value)
            : base_type(value) {
            };

            MillerIndex(const std::initializer_list<value_type>& list)
            : base_type(list) {
            };

            MillerIndex(const base_type& idx)
            : base_type(idx) {
            };

            MillerIndex(const MillerIndex& idx) = default;

            bool operator<(const MillerIndex& rhs) const {
                for (int i = 0; i < rank_; ++i) {
                    if (this->at(i) != rhs.at(i)) return this->at(i) < rhs.at(i);
                }
                return this->at(rank_ - 1) < rhs.at(rank_ - 1);
            }

            MillerIndex friedel() const {
                return (*this)*(-1);
            }
        };

    }

}

#endif /* MILLER_INDEX_HPP */

