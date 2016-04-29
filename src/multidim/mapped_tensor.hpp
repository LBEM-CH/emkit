
#ifndef EM_MULTIDIM_MAPPED_TENSOR_HPP
#define EM_MULTIDIM_MAPPED_TENSOR_HPP

#include <iostream>
#include <string>
#include <cstddef>
#include <cassert>
#include <type_traits>
#include <map>

#include "range.hpp"
#include "index.hpp"

namespace em {
    namespace multidim {

        template<typename ValueType_, size_t rank_ = 1, StorageOrder format_ = StorageOrder::COLUMN_MAJOR>
        class MappedTensor {
        public:
            
            static const int rank = rank_;
            static const StorageOrder format = format_;
            using index_type = Index<rank_>;
            using range_type = Range<rank_>;
            using format_helper_type = StorageOrderArranger<rank_, format_>;
            using data_container_type = std::map<size_type, ValueType_>;
            using self_ = MappedTensor<ValueType_, rank_, format_>;
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

            MappedTensor()
            : range_(), container_() {
            };

            MappedTensor(range_type range)
            : range_(range), container_() {
            };

            MappedTensor(range_type range, const data_container_type& data) {
                static_assert(range.size() > data.rbegin()->first, "The range in data should contain elements of the container");
                range_ = range;
                container_ = data;
            };

            MappedTensor(const MappedTensor& other) {
                range_ = other.range_;
                container_ = other.container_;
            }

            //TODO Constructor of MappedTensor from Tensor

            /**
             * Iterators
             */
            iterator begin() {
                return iterator(this, 0);
            }

            iterator end() {
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

            reference operator[](const index_type& idx) {
                size_type memory_id = format_helper_type::map(idx, range_);
                return container_[memory_id];
            }


            /**
             * Slicing
             */

            Tensor<value_type, rank_ - 1 > slice(typename index_type::value_type slice) const {
                static_assert(rank_ > 1, "The rank should be more than 1 for slicing");
                static_assert(slice < range[rank_ - 1], "The slice number overflows the range");

                Range < rank_ - 1 > slice_range;
                for (int i = 0; i < rank_ - 1; ++i) slice_range[i] = range_[i];

                auto stride_wanted = stride()[rank_ - 1];
                size_type limit_start = slice*stride_wanted;
                size_type limit_end = (slice + 1) * stride_wanted;
                data_container_type sliced_container;
                for (auto d : container_)
                    if (d->first >= limit_start, d->first < limit_end)
                        sliced_container[d->first] = d->second;

                return Tensor<value_type, rank_ - 1 > (slice_range, sliced_container);
            }


        private:

            bool key_exists(const size_type& key) {
                if (container_.find(key) == container_.end()) return false;
                else return true;
            }

            range_type range_;
            data_container_type container_;
        };
    }
}

#endif /* MAPPED_ARRAY_HPP */

