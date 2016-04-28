
#ifndef EM_MULTIDIM_TENSOR_HPP
#define EM_MULTIDIM_TENSOR_HPP

#include <iostream>
#include <string>
#include <cstddef>
#include <type_traits>
#include <vector>
#include <algorithm>
#include <functional>

#include "range.hpp"
#include "index.hpp"
#include "tensor_format.hpp"
#include "tensor_format_helper.hpp"
#include "tensor_iterator.hpp"


namespace em {
    namespace multidim {

        template<typename ValueType_, size_t rank_ = 1, TensorFormat format_ = TensorFormat::COLUMN_MAJOR>
        class Tensor {
        public:
            static const int rank = rank_;
            static const TensorFormat format = format_;
            using index_type = Index<rank_>;
            using range_type = Range<rank_>;
            using format_helper_type = FormatHelper<rank_, format_>;
            using data_container_type = std::vector<ValueType_>;
            using self_ = Tensor<ValueType_, rank_, format_>;
            using value_type = ValueType_;
            using iterator = TensorIterator<self_>;
            using const_iterator = const iterator;
            using reference = std::add_lvalue_reference<ValueType_>;
            using const_reference = const reference&;
            using pointer = std::add_pointer<ValueType_>;
            using difference_type = typename iterator::difference_type;
            using size_type = typename Range<rank_>::size_type;

            /**
             * Constructors
             */

            Tensor()
            : range_(), container_() {
            };

            Tensor(range_type range)
            : range_(range), container_(data_container_type(range.size())) {
            };

            Tensor(range_type range, const data_container_type& data) {
                static_assert(range.size() == data.size(), "The size of data and the range should match");
                range_ = range;
                container_ = data;
            };

            Tensor(range_type range, const pointer& data) {
                range_ = range;
                container_ = std::vector(data, data + range.size());
            };

            Tensor(const Tensor& other) {
                range_ = other.range_;
                container_ = other.container_;
            };

            /**
             * Iterators
             */
            iterator begin() {
                return iterator(this, 0);
            }

            const_iterator begin() const {
                return iterator(this, 0);
            }

            iterator end() {
                return iterator(this, size());
            }

            const_iterator end() const {
                return iterator(this, size());
            }

            /**
             * Observers
             */

            range_type range() const {
                return range_;
            }

            size_type size() const {
                return range_.size();
            }

            index_type stride() const {
                return format_helper_type::get_stride(range_);
            }

            bool empty() const {
                return container_.empty();
            }

            void swap(Tensor other) const {
                Tensor temp = *this;
                this = other;
                other = temp;
            }

            /**
             * Accessing elements
             */
            const reference operator[](const index_type& idx) const {
                size_type memory_id = format_helper_type::map(idx, range_);
                return container_[memory_id];
            }

            reference operator[](const index_type& idx) {
                return const_cast<reference> (static_cast<const Tensor<value_type, rank_> &> (*this).operator[](idx));
            }

            value_type at(const index_type& idx) {
                size_type memory_id = format_helper_type::map(idx, range_);
                return container_[memory_id];
            }

            const pointer data() const {
                return container_.data();
            }

            pointer data() const {
                return container_.data();
            }

            /**
             * Arithmetic operations
             */
            self_ operator+(const self_& right) const {
                assert(a.size() == b.size());
                assert(std::is_arithmetic<value_type>);
                self_ result(range());
                std::transform(container_.begin(), container_.end(), right.container_.begin(),
                        std::back_inserter(result.container_), std::plus<value_type>());
                return result;
            }

            self_& operator+=(const self_& right) {
                return *this +right;
            }

            Tensor<ValueType_, rank_, format_> operator-(const Tensor<ValueType_, rank_, format_>& right) const {
                assert(a.size() == b.size());
                assert(std::is_arithmetic<value_type>);
                self_ result(range());
                std::transform(container_.begin(), container_.end(), right.container_.begin(),
                        std::back_inserter(result.container_), std::minus<value_type>());
                return result;
            }

            Tensor<ValueType_, rank_, format_>& operator-=(const Tensor<ValueType_, rank_, format_>& right) {
                return *this-right;
            }

            template<typename ArithmaticType_>
            self_ operator+(const ArithmaticType_& right) {
                assert(std::is_arithmetic<value_type>);
                assert(std::is_arithmetic<typename ArithmaticType_>);
                std::for_each(container_.begin(), container_.end(), [&](value_type val) {
                    return val + right;
                });
            }

            template<typename ArithmaticType_>
            self_ operator-(const ArithmaticType_& right) {
                assert(std::is_arithmetic<value_type>);
                assert(std::is_arithmetic<typename ArithmaticType_>);
                std::for_each(container_.begin(), container_.end(), [&](value_type val) {
                    return val - right;
                });
            }

            template<typename ArithmaticType_>
            self_ operator*(const ArithmaticType_& right) {
                assert(std::is_arithmetic<value_type>);
                assert(std::is_arithmetic<typename ArithmaticType_>);
                std::for_each(container_.begin(), container_.end(), [&](value_type val) {
                    return val*right;
                });
            }

            /**
             * Slicing
             */

            const Tensor<value_type, rank_ - 1 > & slice(typename index_type::value_type slice_number) const {
                static_assert(rank_ > 1, "The rank should be more than 1 for slicing");
                static_assert(slice_number < range[rank_ - 1], "The slice number overflows the range");
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
            Range<rank_> range_;
            std::vector<value_type> container_;

        };
    }
}

#endif

