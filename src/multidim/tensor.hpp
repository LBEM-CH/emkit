
#ifndef EM_MULTIDIM_TENSOR_HPP
#define EM_MULTIDIM_TENSOR_HPP

#include <iostream>
#include <string>
#include <cstddef>
#include <type_traits>
#include <vector>
#include <algorithm>
#include <functional>

#include "storage_order.hpp"


namespace em {
    namespace multidim {

        template<typename ValueType_, size_t rank_, StorageOrder order_>
        class TensorIterator;
        
        template<typename ValueType_, size_t rank_ = 1, StorageOrder order_ = StorageOrder::COLUMN_MAJOR>
        class Tensor {
        protected:
            using index_type = Index<rank_>;
            using range_type = Range<rank_>;
            using order_arranger = StorageOrderArranger<rank_, order_>;
            using data_type = std::vector<ValueType_>;
            using self_ = Tensor<ValueType_, rank_, order_>;

        public:
            static const int rank = rank_;
            static const StorageOrder storage_order = order_;
            using value_type = ValueType_;
            using iterator = TensorIterator<ValueType_, rank_, order_>;
            using const_iterator = const iterator;
            using const_reverse_iterator = std::reverse_iterator<const_iterator>;
            using reverse_iterator = std::reverse_iterator<iterator>;
            using reference = ValueType_&;
            using const_reference = const ValueType_&;
            using pointer = typename std::add_pointer<ValueType_>::type;
            using difference_type = std::ptrdiff_t;
            using size_type = typename Range<rank_>::size_type;

            /**
             * Constructors
             */

            Tensor()
            : range_(), container_() {
            };
            
            Tensor(const range_type& range, const value_type default_value)
            : range_(range), container_(data_type(range.size(), default_value)) {
            }

            Tensor(const range_type& range, const data_type& data) {
                static_assert(range.size() == data.size(), "The size of data and the range should match");
                range_ = range;
                container_ = data;
            };

            Tensor(const range_type& range, const pointer& data) {
                range_ = range;
                container_ = data_type(data, data + range.size());
            };

            Tensor(const Tensor& other) {
                range_ = other.range_;
                container_ = other.container_;
            };
            

            /**
             * Element Access
             */
            
            const_reference operator[](const index_type& idx) const {
                size_type memory_id = order_arranger::map(idx, range_);
                return container_[memory_id];
            }

            reference operator[](const index_type& idx) {
                return const_cast<reference> (static_cast<const Tensor<value_type, rank_> &> (*this).operator[](idx));
            }

            const_reference at(const index_type& idx) const {
                size_type memory_id = order_arranger::map(idx, range_);
                return container_.at(memory_id);
            }
            
            reference at(const index_type& idx) {
                size_type memory_id = order_arranger::map(idx, range_);
                return container_.at(memory_id);
            }

            const pointer data() const {
                return container_.data();
            }

            pointer data() {
                return container_.data();
            }

            data_type& vectorize() {
                return container_;
            }

            const data_type& vectorize() const {
                return container_;
            }
            
            const reference front() const {
                return container_.front();
            }
            
            reference front() {
                return container_.front();
            }
            
            const reference back() const {
                return container_.back();
            }
            
            reference back() {
                return container_.back();
            }

            
            /**
             * Iterators
             */
            
            iterator begin() {
                return iterator(this, 0);
            }

            const_iterator begin() const {
                return iterator(this, 0);
            }
            
            const_iterator cbegin() const {
                return iterator(this, 0);
            }

            iterator end() {
                return iterator(this, size());
            }

            const_iterator end() const {
                return iterator(this, size());
            }
            
            const_iterator cend() const {
                return  iterator(this, size());
            }
            
            
            /*
             * Capacity
             */

            range_type range() const {
                return range_;
            }

            size_type size() const {
                return range_.size();
            }

            index_type stride() const {
                return order_arranger::get_stride(range_);
            }

            bool empty() const {
                return container_.empty();
            }
            
            /*
             * Modifiers
             */
            
            void reshape(const range_type& new_range) {
                assert(new_range.size() ==  range_.size());
                range_ = new_range;
            }
            
            void resize(const range_type& range) {
                range_ = range;
                container_.resize(range.size());
            }
            
            void clear() {
                container_.clear();
            }
            
            void insert(const index_type& idx, const value_type& value) {
                at(idx) = value;
            }
            
            void swap(Tensor other) const {
                Tensor temp = *this;
                this = other;
                other = temp;
            }
            
            /**
             * Arithmetic operations
             */
            self_ operator+(const self_& right) const {
                assert(range() == right.range());
                static_assert(std::is_arithmetic<value_type>::value, "The value_type in the tensor class should be arithmetic");
                self_ result(range());
                std::transform(container_.begin(), container_.end(), right.container_.begin(),
                        std::back_inserter(result.container_), std::plus<value_type>());
                return result;
            }

            self_& operator+=(const self_& right) {
                return *this +right;
            }

            self_ operator-(const self_& right) const {
                assert(range() == right.range());
                static_assert(std::is_arithmetic<value_type>::value, "The value_type in the tensor class should be arithmetic");
                self_ result(range());
                std::transform(container_.begin(), container_.end(), right.container_.begin(),
                        std::back_inserter(result.container_), std::minus<value_type>());
                return result;
            }

            self_& operator-=(const self_& right) {
                return *this-right;
            }

            template<typename ArithmaticType_>
            self_ operator+(const ArithmaticType_& right) {
                static_assert(std::is_arithmetic<value_type>::value, "The value_type in the tensor class should be arithmetic");
                static_assert(std::is_arithmetic<ArithmaticType_>::value, "The value_type should be arithmetic");
                std::for_each(container_.begin(), container_.end(), [&](value_type val) {
                    return val + right;
                });
            }

            template<typename ArithmaticType_>
            self_ operator-(const ArithmaticType_& right) {
                static_assert(std::is_arithmetic<value_type>::value, "The value_type in the tensor class should be arithmetic");
                static_assert(std::is_arithmetic<ArithmaticType_>::value, "The value_type should be arithmetic");
                std::for_each(container_.begin(), container_.end(), [&](value_type val) {
                    return val - right;
                });
            }

            template<typename ArithmaticType_>
            self_ operator*(const ArithmaticType_& right) {
                static_assert(std::is_arithmetic<value_type>::value, "The value_type in the tensor class should be arithmetic");
                static_assert(std::is_arithmetic<ArithmaticType_>::value, "The value_type should be arithmetic");
                std::for_each(container_.begin(), container_.end(), [&](value_type val) {
                    return val*right;
                });
            }

            /**
             * Slicing
             */

            const Tensor<value_type, rank_ - 1 > & slice(typename index_type::value_type slice_number) const {
                static_assert(rank_ > 1, "The rank should be more than 1 for slicing");
                assert(slice_number < range[rank_ - 1]);
                Range < rank_ - 1 > slice_range;
                for (int i = 0; i < rank_ - 1; ++i) slice_range[i] = range_[i];
                return Tensor<value_type, rank_ - 1 > (slice_range, container_.data() + slice_number * stride()[rank_ - 1]);
            }

            Tensor<value_type, rank_ - 1 > & slice(typename index_type::value_type slice_number) {
                return const_cast<Tensor<value_type, rank_ - 1 > &> (static_cast<const Tensor<value_type, rank_> &> (*this).slice(slice_number));
            }

            /**
             * Sectioning
             */

            self_ section(const index_type& begin_index, const range_type& section_range) {
                self_ cut(section_range);
                assert(range_.contains(section_range + begin_index));
                index_type strides = stride();
                index_type new_id(0);
                index_type old_id(begin_index);
                for (int r = 0; r < rank_; ++r) {
                    for (size_type id = 0; id < section_range[r]; ++id) {
                        new_id[r] += id;
                        old_id[r] += id;
                        cut[new_id] = operator[](new_id);
                    }
                }

                return cut;
            }




        private:

            range_type range_;
            data_type container_;

        };
        
        
        template<typename ValueType_, int rank_, StorageOrder order_>
        bool operator==(const Tensor<ValueType_, rank_, order_>& lhs,
                const Tensor<ValueType_, rank_, order_>& rhs) {
            return(lhs.range() == rhs.range() && lhs.vectorize() == rhs.vectorize());
        };
        
        template<typename ValueType_, int rank_, StorageOrder order_>
        bool operator!=(const Tensor<ValueType_, rank_, order_>& lhs,
                const Tensor<ValueType_, rank_, order_>& rhs) {
            return(lhs.range() != rhs.range() || lhs.vectorize() != rhs.vectorize());
        };
        
        template<typename ValueType_, int rank_, StorageOrder order_>
        bool operator<(const Tensor<ValueType_, rank_, order_>& lhs,
                const Tensor<ValueType_, rank_, order_>& rhs) {
            return(lhs.vectorize() < rhs.vectorize());
        };
        
        template<typename ValueType_, int rank_, StorageOrder order_>
        bool operator<=(const Tensor<ValueType_, rank_, order_>& lhs,
                const Tensor<ValueType_, rank_, order_>& rhs) {
            return(lhs.vectorize() <= rhs.vectorize());
        };
        
        template<typename ValueType_, int rank_, StorageOrder order_>
        bool operator>(const Tensor<ValueType_, rank_, order_>& lhs,
                const Tensor<ValueType_, rank_, order_>& rhs) {
            return(lhs.vectorize() > rhs.vectorize());
        };
        
        template<typename ValueType_, int rank_, StorageOrder order_>
        bool operator>=(const Tensor<ValueType_, rank_, order_>& lhs,
                const Tensor<ValueType_, rank_, order_>& rhs) {
            return(lhs.vectorize() >= rhs.vectorize());
        };
    }
}

#endif

