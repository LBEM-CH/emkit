
#ifndef TENSOR_ITERATOR_HPP
#define TENSOR_ITERATOR_HPP

#include <utility>
#include <iterator>
#include <cstddef>
#include <type_traits>

#include "tensor.hpp"
#include "index_value_pair.hpp"

namespace em {
    namespace multidim {

        template<typename ValueType_, size_t rank_, StorageOrder order_>
        class TensorIterator {
        protected:
            typedef TensorIterator<ValueType_, rank_, order_> Self_;
            typedef Tensor<ValueType_, rank_, order_> TensorType_;
            typedef Index<rank_> index_type;
            typedef Range<rank_> range_type;
            typedef typename index_type::size_type size_type;
            typedef StorageOrderArranger<rank_, order_> order_arranger_type;

        public:
            static const size_t rank = rank_;
            static const StorageOrder storage_order = order_;
            using iterator_category = std::random_access_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = IndexValuePair<ValueType_, rank_>;
            using pointer = value_type*;
            using reference = value_type&;

            TensorIterator()
            : tensor_container_(), index_(0) {
            }

            TensorIterator(TensorType_* base_ptr, size_type index)
            : tensor_container_(base_ptr), index_(index) {
            }

            TensorIterator(const Self_& other) = default;
            
            ~TensorIterator() {
                //if(pair_initialized_) {
                //    tensor_container_->at(pair_.first) = pair_.second;
                //}
            }

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

            size_type index() {
                return index_;
            }

        protected:
            
            void rebook_pair(size_type index) {
                //if(pair_initialized_) {
                //    tensor_container_->at(pair_.first) = pair_.second;
                //}
                index_type idx = order_arranger_type::map(index, tensor_container_->range());
                pair_ = value_type(idx, &tensor_container_->at(idx));
                pair_initialized_ = true;
            }
            
            size_type index_;
            TensorType_* tensor_container_;
            value_type pair_;
            bool pair_initialized_ = false;

        };

        // Forward iterator requirements

        template<typename ValueType_, int rank_, StorageOrder order_>
        inline bool
        operator==(const TensorIterator<ValueType_, rank_, order_>& __lhs,
                const TensorIterator<ValueType_, rank_, order_>&& __rhs) {
            return (__lhs.base() == __rhs.base() && __lhs.index() == __rhs.index());
        }

        template<typename ValueType_, int rank_, StorageOrder order_>
        inline bool
        operator!=(const TensorIterator<ValueType_, rank_, order_>& __lhs,
                const TensorIterator<ValueType_, rank_, order_>&& __rhs) {
            return (__lhs.base() != __rhs.base() || __lhs.index() != __rhs.index());
        }

        // Random access iterator requirements

        template<typename ValueType_, int rank_, StorageOrder order_>
        inline bool
        operator<(const TensorIterator<ValueType_, rank_, order_>& __lhs,
                const TensorIterator<ValueType_, rank_, order_>&& __rhs) {
            return __lhs.index() < __rhs.index();
        }

        template<typename ValueType_, int rank_, StorageOrder order_>
        inline bool
        operator>(const TensorIterator<ValueType_, rank_, order_>& __lhs,
                const TensorIterator<ValueType_, rank_, order_>&& __rhs) {
            return __lhs.index() > __rhs.index();
        }

        template<typename ValueType_, int rank_, StorageOrder order_>
        inline bool
        operator<=(const TensorIterator<ValueType_, rank_, order_>& __lhs,
                const TensorIterator<ValueType_, rank_, order_>&& __rhs) {
            return __lhs.index() <= __rhs.index();
        }

        template<typename ValueType_, int rank_, StorageOrder order_>
        inline bool
        operator>=(const TensorIterator<ValueType_, rank_, order_>& __lhs,
                const TensorIterator<ValueType_, rank_, order_>&& __rhs) {
            return __lhs.index() >= __rhs.index();
        }

    }
}

#endif