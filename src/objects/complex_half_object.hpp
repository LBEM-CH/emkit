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

#ifndef EM_COMPLEX_HALF_OBJECT_HPP
#define EM_COMPLEX_HALF_OBJECT_HPP

#include "../elements/complex.hpp"
#include "../elements/tensor.hpp"
#include "../elements/tensor_iterator.hpp"

namespace em {

    namespace object {

        template<typename APHValueType_, size_t rank_>
        class ComplexHalfObject : public element::Tensor<element::Complex<APHValueType_>, rank_, element::StorageOrder::COLUMN_MAJOR> {
        private:
            typedef element::Tensor<element::Complex<APHValueType_>, rank_, element::StorageOrder::COLUMN_MAJOR> BaseType_;

        public:
            typedef element::Complex<APHValueType_> data_type;
            typedef typename BaseType_::index_type index_type;

            /**
             * Constructors
             */

            ComplexHalfObject()
            : BaseType_(), even_size_x_(false) {
            };

            /**
             * Constructor using a logical size (size of the corresponding real object)
             * @param range
             */
            ComplexHalfObject(const index_type& range)
            : BaseType_(range) {
                index_type complex_container_range = range;
                complex_container_range[0] = complex_container_range[0] / 2 + 1;
                BaseType_::resize(complex_container_range);
                even_size_x_ = false;
                if (range[0] % 2 == 0) even_size_x_ = true;
            };

            ComplexHalfObject(const index_type& range, const index_type& origin)
            : BaseType_(range, origin) {
                index_type complex_container_range = range;
                complex_container_range[0] = complex_container_range[0] / 2 + 1;
                BaseType_::resize(complex_container_range);
                even_size_x_ = false;
                if (range[0] % 2 == 0) even_size_x_ = true;
            };

            ComplexHalfObject(const BaseType_& tensor, bool is_first_dim_even = false)
            : BaseType_(tensor), even_size_x_(is_first_dim_even) {
            };

            ComplexHalfObject(const ComplexHalfObject& other) = default;

            /**
             * Accessing elements
             */

            data_type& operator[](const index_type& idx) {
                if (idx[0] < 0) {
                    std::cerr << "Expected only positive values of h, found: " << idx[0] << "\n";
                    exit(1);
                }
                return BaseType_::operator[](idx);
            }

            const data_type& operator[](const index_type& idx) const {
                if (idx[0] < 0) {
                    std::cerr << "Expected only positive values of h, found: " << idx[0] << "\n";
                    exit(1);
                }
                return BaseType_::operator[](idx);
            }

            data_type& at(const index_type& idx) {
                if (idx[0] < 0) {
                    std::cerr << "Expected only positive values of h, found: " << idx[0] << "\n";
                    exit(1);
                }
                return at(idx);
            }

            const data_type& at(const index_type& idx) const {
                if (idx[0] < 0) {
                    std::cerr << "Expected only positive values of h, found: " << idx[0] << "\n";
                    exit(1);
                }
                return at(idx);
            }

            /**
             * Capacity
             */

            index_type miller_index_max() const {
                index_type max_idx;
                index_type rnge = BaseType_::range();
                max_idx[0] = rnge[0] - 1;
                for (int r = 1; r < max_idx.rank; ++r) max_idx[r] = rnge[r] / 2;
                return max_idx;
            }

            index_type miller_index_min() const {
                index_type max_idx = miller_index_max();
                index_type min_idx;
                index_type rnge = BaseType_::range();
                if (even_size_x_) min_idx[0] = -1 * (max_idx[0] - 1);
                else min_idx[0] = -1 * max_idx[0];
                for (int r = 1; r < max_idx.rank; ++r) min_idx[r] = max_idx[r] - rnge[r] + 1;
                return min_idx;
            }

            index_type logical_range() const {
                index_type rnge = BaseType_::range();
                if (even_size_x_) rnge[0] = (rnge[0] - 1)*2;
                else rnge[0] = rnge[0]*2 - 1;
                return rnge;
            }


        private:
            bool even_size_x_;

        };
    }
}

#endif /* REAL_OBJECT_HPP */

