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

#ifndef EM_MULTIDIM_TENSOR_HPP
#define EM_MULTIDIM_TENSOR_HPP

#include <iostream>
#include <string>
#include <cstddef>
#include <type_traits>
#include <vector>
#include <algorithm>
#include <functional>

#include "tensor_storage_order.hpp"
#include "index_value_pair.hpp"


namespace em {
    namespace element {

        template<typename ValueType_, size_t rank_, StorageOrder order_, bool is_const_iterator_>
        class TensorIterator;
        
        template<typename ValueType_, size_t rank_ = 1, StorageOrder order_ = StorageOrder::COLUMN_MAJOR>
        class Tensor {
        public:
            using index_type = Index<rank_>;
            using range_type = Range<rank_>;
            using arranger_type = MemoryArranger<rank_, order_>;
            using data_type = std::vector<ValueType_>;
            using self_ = Tensor<ValueType_, rank_, order_>;

            static const int rank = rank_;
            static const StorageOrder storage_order = order_;
            using value_type = ValueType_;
            using iterator = TensorIterator<ValueType_, rank_, order_, false>;
            using const_iterator = TensorIterator<ValueType_, rank_, order_, true>;
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
            : range_(), origin_(0), container_() {
            };
            
            Tensor(const range_type& range, const value_type& default_value = ValueType_())
            : range_(range), origin_(0), container_(data_type(range.size(), default_value)) {
            }
            
            Tensor(const range_type& range, const index_type& origin, const value_type& default_value = ValueType_())
            : range_(range), origin_(origin), container_(data_type(range.size(), default_value)) {
                assert(range.contains(origin));
            }

            Tensor(const range_type& range, const data_type& data) 
            : range_(range), origin_(0), container_(data) {
                assert(range.size() == data.size());
            };
            
            Tensor(const range_type& range, const index_type& origin, const data_type& data) 
            : range_(range), origin_(origin), container_(data) {
                assert(range.size() == data.size());
                assert(range.contains(origin));
            };
            
            Tensor(const range_type& range, const pointer& data) 
            : range_(range_), origin_(0), container_(data_type(data, data + range.size())){
            }
            
            Tensor(const Tensor& other) {
                reset(other);
            };

            Tensor(Tensor&& other) {
                reset(other);
            };
            
            Tensor& operator=(const Tensor& right) {
                if (this != &right) reset(right);
                return *this;
            }
 
            Tensor& operator=(Tensor&& right) {
                if (this != &right) reset(right);
                return *this;
            }
            

            /**
             * Element Access
             */
            
            const_reference operator[](const index_type& idx) const {
                size_type memory_id = arranger_type::map(idx, range_, origin_);
                return container_[memory_id];
            }

            reference operator[](const index_type& idx) {
                return const_cast<reference> (static_cast<const Tensor<value_type, rank_, order_> &> (*this).operator[](idx));
            }

            const_reference at(const index_type& idx) const {
                size_type memory_id = arranger_type::map(idx, range_, origin_);
                return container_.at(memory_id);
            }
            
            reference at(const index_type& idx) {
                size_type memory_id = arranger_type::map(idx, range_, origin_);
                return container_.at(memory_id);
            }
            
            const_reference operator[](const size_type& idx) const {
                assert(range_.size() > idx);
                return container_[idx];
            }

            reference operator[](const size_type& idx) {
                assert(range_.size() > idx);
                return container_[idx];
            }

            const_reference at(const size_type& idx) const {
                assert(range_.size() > idx);
                return container_.at(idx);
            }
            
            reference at(const size_type& idx) {
                assert(range_.size() > idx);
                return container_.at(idx);
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
                return cbegin();
            }
            
            const_iterator cbegin() const {
                return const_iterator(this, 0);
            }

            iterator end() {
                return iterator(this, size());
            }

            const_iterator end() const {
                return cend();
            }
            
            const_iterator cend() const {
                return const_iterator(this, size());
            }
            
            
            /*
             * Capacity
             */

            range_type range() const {
                return range_;
            }
            
            index_type origin() const {
                return origin_;
            }

            size_type size() const {
                return range_.size();
            }

            index_type stride() const {
                return arranger_type::get_stride(range_);
            }

            bool empty() const {
                return container_.empty();
            }
            
            /*
             * Modifiers
             */
            
            void transform_origin(const index_type& new_origin) {
       
                //TODO: Program this efficiently with memory copy
                
                Tensor<value_type, rank_, order_> temp(range(), new_origin, value_type());
                for (const auto& itr : *this) {
                    temp.at(itr.index()) = itr.value();
                }
                this->reset(temp);
            }
            
            void reshape(const range_type& new_range) {
                assert(new_range.size() ==  range_.size());
                range_ = new_range;
            }
            
            void resize(const range_type& range) {
                range_ = range;
                container_.resize(range.size());
            }
            
            template<StorageOrder order>
            void reset(const Tensor<value_type, rank_, order>& other) {
                range_ = other.range_;
                origin_ = other.origin_;
                if(order == order_) container_ = other.container_;
                else {
                    for(const auto& data : other) {
                        at(data.index()) = data.value();
                    }
                }
            }
            
            void reset(Tensor&& other) {
                range_ = std::move(other.range_);
                origin_ = std::move(other.origin_);
                container_ = std::move(other.container_);
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
             * Output/Print
             */
            friend inline std::ostream& operator<<(std::ostream& os, const self_& obj) {
                for(const auto& itr : obj) {
                    os << itr.index() << " -> " << itr.value() << "\n";
                }
                return os;
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

            Tensor<value_type, rank_ - 1, order_> slice(typename index_type::value_type slice_number) const {
                static_assert(rank_ > 1, "The rank should be more than 1 for slicing");
                assert(slice_number < range_[rank_ - 1]);
                Range < rank_ - 1 > slice_range;
                for (int i = 0; i < rank_ - 1; ++i) slice_range[i] = range_[i];
                size_t start = slice_number * slice_range.size();
                size_t extent = (slice_number+1) * slice_range.size();
                data_type slice_vector = data_type(container_.data() + start, container_.data() + extent);
                Tensor<value_type, rank_ - 1, order_> sliced(slice_range, slice_vector); 
                return sliced;
            }


            /**
             * Sectioning
             */

            self_ section(const index_type& begin_index, const range_type& section_range) const {
                assert(range_.contains(section_range + begin_index - index_type(1)));
                self_ cut(section_range, value_type());
                for(auto& val : cut) {
                    val.value() = at(val.index()+begin_index);
                }
                return cut;
            }


        private:

            range_type range_;
            index_type origin_;
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

