
#ifndef TENSOR_ITERATOR_HPP
#define TENSOR_ITERATOR_HPP

#include <utility>
#include <iterator>
#include <cstddef>
#include <type_traits>

#include "index.hpp"
#include "tensor_traits.hpp"

namespace em {
    namespace multidim {

        template<typename TensorType_>
        class TensorIterator {
        public:

            typedef typename tensor_traits<TensorType_>::index_type index_type;
            using range_type = typename tensor_traits<TensorType_>::range_type;
            using size_type = typename tensor_traits<TensorType_>::size_type;
            using value_type = typename tensor_traits<TensorType_>::value_type;
            using format_helper_type = typename tensor_traits<TensorType_>::format_helper_type;

            using Self_ = TensorIterator<TensorType_>;
            using tensor_type = TensorType_;
            using tensor_container_pointer = std::add_pointer<tensor_type>;

            /*
             * For std::iterator_traits to behave correctly
             */
            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using pointer = std::pair<index_type, value_type>*;
            using reference = std::pair<index_type, value_type>&;

            TensorIterator()
            : container_(), _index(0) {
            }

            TensorIterator(tensor_container_pointer base_ptr, index_type index)
            : container_(base_ptr), _index(index) {
            }

            TensorIterator(const Self_& other) = default;

            Self_& operator=(const Self_& rhs) {
                _index = rhs._index;
                container_ = rhs.container_;
                return *this;
            }

            reference operator*() const {
                index_type idx = format_helper_type::map(_index, container_->range());
                return std::make_pair(idx, container_[_index]);
            }

            pointer operator->() const {
                index_type idx = format_helper_type::map(_index, container_->range());
                return std::addressof(std::make_pair(idx, container_[_index]));
            }

            Self_& operator++() {
                _index++;
                return *this;
            }

            Self_ operator++(int increment) const {
                Self_ temp = *this;
                temp._index += increment;
                return temp;
            }

            Self_& operator--() {
                _index++;
                return *this;
            }

            Self_ operator--(int decrement) const {
                Self_ temp = *this;
                temp._index -= decrement;
                return temp;
            }

            bool operator==(const Self_& rhs) const {
                return (container_ == rhs.container_ && _index == rhs._index);
            }

            bool operator!=(const Self_& rhs) const {
                return (container_ != rhs.container_ || _index != rhs._index);
            }



        private:
            size_type _index;
            tensor_container_pointer container_;

        };
    }
}

#endif