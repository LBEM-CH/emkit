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

        template<typename DataType_, size_t rank_, StorageOrder order_, bool is_const_iterator_>
        class TensorIterator;
        
        template<typename DataType_, size_t rank_, StorageOrder order_>
        class Tensor {
        public:
            static const int rank = rank_;
            static const StorageOrder storage_order = order_;
            using index_type = Index<rank_>;
            using data_type = DataType_;
            using arranger_type = MemoryArranger<rank_, order_>;

            //For STL Random Access Iterator
            using value_type = IndexValuePair<DataType_, rank_>;
            using iterator = TensorIterator<DataType_, rank_, order_, false>;
            using const_iterator = TensorIterator<DataType_, rank_, order_, true>;
            using reference = DataType_&;
            using const_reference = const DataType_&;
            using pointer = typename std::add_pointer<DataType_>::type;
            using difference_type = std::ptrdiff_t;
            using size_type = typename Index<rank_>::size_type;
            using reverse_iterator = std::reverse_iterator<iterator>;
            using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        private:
            using SelfType_ = Tensor<DataType_, rank_, order_>;
            using RepType_ = std::vector<DataType_>;
        
        public:
            /**
             * Constructors
             */

            Tensor()
            : range_(), origin_(0), data_container_() {
            };
            
            Tensor(const index_type& range, const data_type& default_value = DataType_())
            : range_(range), origin_(0), data_container_(RepType_(range.size(), default_value)) {
            }
            
            Tensor(const index_type& range, const index_type& origin, const data_type& default_value = DataType_())
            : range_(range), origin_(origin), data_container_(RepType_(range.size(), default_value)) {
                assert(range.contains(origin));
            }

            Tensor(const index_type& range, const RepType_& data) 
            : range_(range), origin_(0), data_container_(data) {
                assert(range.size() == data.size());
            };
            
            Tensor(const index_type& range, const index_type& origin, const RepType_& data) 
            : range_(range), origin_(origin), data_container_(data) {
                assert(range.size() == data.size());
                assert(range.contains(origin));
            };
            
            Tensor(const index_type& range, const pointer& data) 
            : range_(range_), origin_(0), data_container_(RepType_(data, data + range.size())){
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
            
            virtual const_reference operator[](const index_type& idx) const {
                size_type memory_id = arranger_type::map(idx, range_, origin_);
                return data_container_[memory_id];
            }

            virtual reference operator[](const index_type& idx) {
                return const_cast<reference> (static_cast<const Tensor<data_type, rank_, order_> &> (*this).operator[](idx));
            }

            virtual const_reference at(const index_type& idx) const {
                size_type memory_id = arranger_type::map(idx, range_, origin_);
                return data_container_.at(memory_id);
            }
            
            virtual reference at(const index_type& idx) {
                size_type memory_id = arranger_type::map(idx, range_, origin_);
                return data_container_.at(memory_id);
            }
            
            const_reference operator[](const size_type& idx) const {
                assert(range_.size() > idx);
                return data_container_[idx];
            }

            reference operator[](const size_type& idx) {
                assert(range_.size() > idx);
                return data_container_[idx];
            }

            const_reference at(const size_type& idx) const {
                assert(range_.size() > idx);
                return data_container_.at(idx);
            }
            
            reference at(const size_type& idx) {
                assert(range_.size() > idx);
                return data_container_.at(idx);
            }

            const pointer data() const {
                return data_container_.data();
            }

            pointer data() {
                return data_container_.data();
            }

            RepType_& vectorize() {
                return data_container_;
            }

            const RepType_& vectorize() const {
                return data_container_;
            }
            
            const reference front() const {
                return data_container_.front();
            }
            
            reference front() {
                return data_container_.front();
            }
            
            const reference back() const {
                return data_container_.back();
            }
            
            reference back() {
                return data_container_.back();
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
            
            reverse_iterator rbegin() {
                return reverse_iterator(end());
            }
            
            const_reverse_iterator rbegin() const {
                return const_reverse_iterator(end());
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
            
            reverse_iterator rend() {
                return reverse_iterator(begin());
            }
            
            const_reverse_iterator rend() const {
                return const_reverse_iterator(begin());
            }
            
            
            /*
             * Capacity
             */

            index_type range() const {
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
                return data_container_.empty();
            }
            
            /*
             * Modifiers
             */
            
            void transform_origin(const index_type& new_origin) {
       
                //TODO: Program this efficiently with memory copy
                
                Tensor<data_type, rank_, order_> temp(range(), new_origin, data_type());
                for (const auto& itr : *this) {
                    temp.at(itr.index()) = itr.value();
                }
                this->reset(temp);
            }
            
            void reshape(const index_type& new_range) {
                assert(new_range.size() ==  range_.size());
                range_ = new_range;
            }
            
            void resize(const index_type& range) {
                assert(range.contains(origin()));
                range_ = range;
                data_container_.resize(range.size());
            }
            
            template<StorageOrder order>
            void reset(const Tensor<data_type, rank_, order>& other) {
                range_ = other.range_;
                origin_ = other.origin_;
                if(order == order_) data_container_ = other.data_container_;
                else {
                    for(const auto& data : other) {
                        at(data.index()) = data.value();
                    }
                }
            }
            
            void reset(Tensor&& other) {
                range_ = std::move(other.range_);
                origin_ = std::move(other.origin_);
                data_container_ = std::move(other.data_container_);
            }
            
            void clear() {
                data_container_.clear();
            }
            
            void insert(const index_type& idx, const data_type& value) {
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
            friend inline std::ostream& operator<<(std::ostream& os, const SelfType_& obj) {
                for(const auto& itr : obj) {
                    os << itr.index() << " -> " << itr.value() << "\n";
                }
                return os;
            }
            
            /**
             * Arithmetic operations
             */
            SelfType_ operator+(const SelfType_& right) const {
                assert(range() == right.range());
                static_assert(std::is_arithmetic<data_type>::value, "The value_type in the tensor class should be arithmetic");
                SelfType_ result(range());
                std::transform(data_container_.begin(), data_container_.end(), right.data_container_.begin(),
                        std::back_inserter(result.data_container_), std::plus<data_type>());
                return result;
            }

            SelfType_& operator+=(const SelfType_& right) {
                return *this +right;
            }

            SelfType_ operator-(const SelfType_& right) const {
                assert(range() == right.range());
                static_assert(std::is_arithmetic<data_type>::value, "The value_type in the tensor class should be arithmetic");
                SelfType_ result(range());
                std::transform(data_container_.begin(), data_container_.end(), right.data_container_.begin(),
                        std::back_inserter(result.data_container_), std::minus<data_type>());
                return result;
            }

            SelfType_& operator-=(const SelfType_& right) {
                return *this-right;
            }

            template<typename ArithmaticType_>
            SelfType_ operator+(const ArithmaticType_& right) {
                static_assert(std::is_arithmetic<data_type>::value, "The value_type in the tensor class should be arithmetic");
                static_assert(std::is_arithmetic<ArithmaticType_>::value, "The value_type should be arithmetic");
                std::for_each(data_container_.begin(), data_container_.end(), [&](data_type val) {
                    return val + right;
                });
            }

            template<typename ArithmaticType_>
            SelfType_ operator-(const ArithmaticType_& right) {
                static_assert(std::is_arithmetic<data_type>::value, "The value_type in the tensor class should be arithmetic");
                static_assert(std::is_arithmetic<ArithmaticType_>::value, "The value_type should be arithmetic");
                std::for_each(data_container_.begin(), data_container_.end(), [&](data_type val) {
                    return val - right;
                });
            }

            template<typename ArithmaticType_>
            SelfType_ operator*(const ArithmaticType_& right) {
                static_assert(std::is_arithmetic<data_type>::value, "The value_type in the tensor class should be arithmetic");
                static_assert(std::is_arithmetic<ArithmaticType_>::value, "The value_type should be arithmetic");
                std::for_each(data_container_.begin(), data_container_.end(), [&](data_type val) {
                    return val*right;
                });
            }

            /**
             * Slicing
             */

            Tensor<data_type, rank_ - 1, order_> slice(typename index_type::value_type slice_number) const {
                static_assert(rank_ > 1, "The rank should be more than 1 for slicing");
                assert(slice_number < range_[rank_ - 1]);
                Index < rank_ - 1 > slice_range;
                for (int i = 0; i < rank_ - 1; ++i) slice_range[i] = range_[i];
                size_t start = slice_number * slice_range.size();
                size_t extent = (slice_number+1) * slice_range.size();
                RepType_ slice_vector = RepType_(data_container_.data() + start, data_container_.data() + extent);
                Tensor<data_type, rank_ - 1, order_> sliced(slice_range, slice_vector); 
                return sliced;
            }
            
            void set_slice(typename index_type::value_type slice_number, Tensor<data_type, rank_ - 1, order_> slice) {
                static_assert(rank_ > 1, "The rank should be more than 1 for setting slices");
                assert(slice_number < range_[rank_ - 1]);
                
                Index < rank_ - 1 > slice_range;
                for (int i = 0; i < rank_ - 1; ++i) slice_range[i] = range_[i];
                assert(slice_range.size() == slice.range().size());
                
                RepType_ slice_vector = slice.vectorize();
                for(int i=0; i<slice_range.size(); ++i) {
                    data_container_[i + slice_number * slice_range.size()] = slice_vector[i];
                }
            }


            /**
             * Sectioning
             */

            SelfType_ section(const index_type& begin_index, const index_type& section_range) const {
                assert(range_.contains(section_range + begin_index - index_type(1)));
                SelfType_ cut(section_range, data_type());
                for(auto& val : cut) {
                    val.value() = at(val.index()+begin_index);
                }
                return cut;
            }


        private:

            index_type range_;
            index_type origin_;
            RepType_ data_container_;

        };
        
        
        template<typename DataType_, size_t rank_, StorageOrder order_>
        bool operator==(const Tensor<DataType_, rank_, order_>& lhs,
                const Tensor<DataType_, rank_, order_>& rhs) {
            return(lhs.range() == rhs.range() && lhs.vectorize() == rhs.vectorize());
        };
        
        template<typename DataType_, size_t rank_, StorageOrder order_>
        bool operator!=(const Tensor<DataType_, rank_, order_>& lhs,
                const Tensor<DataType_, rank_, order_>& rhs) {
            return(lhs.range() != rhs.range() || lhs.vectorize() != rhs.vectorize());
        };
        
        template<typename DataType_, size_t rank_, StorageOrder order_>
        bool operator<(const Tensor<DataType_, rank_, order_>& lhs,
                const Tensor<DataType_, rank_, order_>& rhs) {
            return(lhs.vectorize() < rhs.vectorize());
        };
        
        template<typename DataType_, size_t rank_, StorageOrder order_>
        bool operator<=(const Tensor<DataType_, rank_, order_>& lhs,
                const Tensor<DataType_, rank_, order_>& rhs) {
            return(lhs.vectorize() <= rhs.vectorize());
        };
        
        template<typename DataType_, size_t rank_, StorageOrder order_>
        bool operator>(const Tensor<DataType_, rank_, order_>& lhs,
                const Tensor<DataType_, rank_, order_>& rhs) {
            return(lhs.vectorize() > rhs.vectorize());
        };
        
        template<typename DataType_, size_t rank_, StorageOrder order_>
        bool operator>=(const Tensor<DataType_, rank_, order_>& lhs,
                const Tensor<DataType_, rank_, order_>& rhs) {
            return(lhs.vectorize() >= rhs.vectorize());
        };
    }
}

#endif

