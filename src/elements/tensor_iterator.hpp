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

#ifndef TENSOR_ITERATOR_HPP
#define TENSOR_ITERATOR_HPP

#include <utility>
#include <iterator>
#include <cstddef>
#include <type_traits>

#include "tensor.hpp"
#include "index_value_pair.hpp"

namespace em {
    namespace element {

        template<typename ValueType_, size_t rank_, StorageOrder order_, bool is_const_iterator_>
        class TensorIterator {
        protected:
            typedef TensorIterator<ValueType_, rank_, order_, is_const_iterator_> Self_;
            typedef typename std::conditional<is_const_iterator_, const Tensor<ValueType_, rank_, order_>, Tensor<ValueType_, rank_, order_>>::type TensorType_;
            typedef Index<rank_> index_type;
            typedef Range<rank_> range_type;
            typedef typename index_type::size_type size_type;
            typedef MemoryArranger<rank_, order_> order_arranger_type;

        public:
            static const size_t rank = rank_;
            static const StorageOrder storage_order = order_;
            using iterator_category = std::random_access_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = typename std::conditional<is_const_iterator_, IndexValuePair<const ValueType_, rank_>, IndexValuePair<ValueType_, rank_>>::type;
            using pointer = value_type*;
            using reference = value_type&;

            TensorIterator()
            : tensor_container_(), index_(0) {
            }

            TensorIterator(TensorType_* base_ptr, size_type index)
            : tensor_container_(base_ptr), index_(index) {
            }

            TensorIterator(const Self_& other) = default;

            /*
             * Forward iterator requirements
             */

            Self_& operator=(const Self_& rhs) = default;

            reference operator*() {
                rebook_pair(index_);
                return pair_;
            }

            pointer operator->() {
                rebook_pair(index_);
                return &pair_;
            }

            Self_& operator++() {
                index_++;
                return *this;
            }

            Self_ operator++(int increment) const {
                Self_ temp = *this;
                temp.index_ += increment;
                return temp;
            }
            
            bool operator==(const Self_& rhs) const {
                return (tensor_container_ == rhs.tensor_container_ && index_ == rhs.index_);
            }

            bool operator!=(const Self_& rhs) const {
                return (tensor_container_ != rhs.tensor_container_ || index_ != rhs.index_);
            }

            /**
             * Bidirectional iterator requirements 
             */

            Self_& operator--() {
                index_--;
                return *this;
            }

            Self_ operator--(int decrement) const {
                Self_ temp = *this;
                temp.index_ -= decrement;
                return temp;
            }

            /**
             * Random access iterator requirements
             */

            reference operator[](const difference_type& __n) const {
                rebook_pair(index_ + __n);
                return pair_;
            }

            Self_& operator+=(const difference_type& __n) {
                index_ += __n;
                return *this;
            }

            Self_ operator+(const difference_type& __n) const {
                Self_ temp = *this;
                temp.index_ += __n;
                return temp;
            }

            Self_& operator-=(const difference_type& __n) {
                index_ -= __n;
                return *this;
            }

            Self_ operator-(const difference_type& __n) const {
                Self_ temp = *this;
                temp.index_ -= __n;
                return temp;
            }

            TensorType_* base() {
                return tensor_container_;
            }

            size_type index() const {
                return index_;
            }

        protected:
            
            void rebook_pair(size_type index) {
                index_type idx = order_arranger_type::map(index, tensor_container_->range(), tensor_container_->origin());
                pair_ = value_type(idx, &tensor_container_->at(index));
            }
            
            size_type index_;
            TensorType_* tensor_container_;
            value_type pair_;

        };

        // Random access iterator requirements

        template<typename ValueType_, size_t rank_, StorageOrder order_, bool is_const_iterator_>
        inline bool
        operator<(const TensorIterator<ValueType_, rank_, order_, is_const_iterator_>& __lhs,
                const TensorIterator<ValueType_, rank_, order_, is_const_iterator_>& __rhs) {
            return __lhs.index() < __rhs.index();
        }

        template<typename ValueType_, size_t rank_, StorageOrder order_, bool is_const_iterator_>
        inline bool
        operator>(const TensorIterator<ValueType_, rank_, order_, is_const_iterator_>& __lhs,
                const TensorIterator<ValueType_, rank_, order_, is_const_iterator_>& __rhs) {
            return __lhs.index() > __rhs.index();
        }

        template<typename ValueType_, size_t rank_, StorageOrder order_, bool is_const_iterator_>
        inline bool
        operator<=(const TensorIterator<ValueType_, rank_, order_,is_const_iterator_>& __lhs,
                const TensorIterator<ValueType_, rank_, order_, is_const_iterator_>& __rhs) {
            return __lhs.index() <= __rhs.index();
        }

        template<typename ValueType_, size_t rank_, StorageOrder order_, bool is_const_iterator_>
        inline bool
        operator>=(const TensorIterator<ValueType_, rank_, order_, is_const_iterator_>& __lhs,
                const TensorIterator<ValueType_, rank_, order_, is_const_iterator_>& __rhs) {
            return __lhs.index() >= __rhs.index();
        }
        
        template<typename ValueType_, size_t rank_, StorageOrder order_, bool is_const_iterator_>
        inline typename TensorIterator<ValueType_, rank_, order_, is_const_iterator_>::difference_type
        operator-(const TensorIterator<ValueType_, rank_, order_,is_const_iterator_>& __lhs,
                const TensorIterator<ValueType_, rank_, order_, is_const_iterator_>& __rhs) {
            return __lhs.index() - __rhs.index();
        }

        template<typename ValueType_, size_t rank_, StorageOrder order_, bool is_const_iterator_>
        inline typename TensorIterator<ValueType_, rank_, order_, is_const_iterator_>::difference_type
        operator+(const TensorIterator<ValueType_, rank_, order_,is_const_iterator_>& __lhs,
                const TensorIterator<ValueType_, rank_, order_, is_const_iterator_>& __rhs) {
            return __lhs.index() + __rhs.index();
        }

    }
}

#endif