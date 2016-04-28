
#ifndef EM_MULTIDIM_INDEX_HPP
#define EM_MULTIDIM_INDEX_HPP

#include <iostream>
#include <cassert>
#include <string>
#include <type_traits>
#include <cstddef>
#include <initializer_list>

namespace em {
    namespace multidim {

        template<int rank_>
        class Index {
        public:
            static const size_t rank = rank_;
            using value_type = std::ptrdiff_t;
            using reference = std::ptrdiff_t&;
            using const_reference = std::add_const<std::ptrdiff_t&>;
            using size_type = size_t;

            /**
             * Constructors
             */

            Index() = default;

            Index(value_type value) {
                for (int i = 0; i < rank_; ++i)
                    data_[i] = value;
            };

            Index(const std::initializer_list<value_type>& list)
            : data_(list) {
                static_assert(rank_ == list.size(), "Rank should be equal to the size of the list in Index constructor");
            };

            Index(const Index& other) {
                data_ = other.data_;
            };

            /**
             * Assignment operator
             */
            Index& operator=(const Index& rhs) {
                data_ = rhs.data_;
                return *this;
            }

            /**
             * Accessing components
             */

            reference operator[](value_type idx) {
                return at(idx);
            }

            const_reference operator[](value_type idx) const {
                return at(idx);
            }

            reference at(value_type idx) {
                static_assert(idx < rank_ && idx > 0, "id should be smaller than the rank");
                return data_[idx];
            }

            const_reference at(value_type idx) const {
                static_assert(idx < rank_ && idx > 0, "id should be smaller than the rank");
                return data_[idx];
            }

            /**
             * Comparison operators
             */
            bool operator!=(const Index& rhs) const {
                for (int idx = 0; idx < rank_; ++idx)
                    if (at(idx) != rhs.at(idx))
                        return false;

                return true;
            }

            bool operator==(const Index& rhs) const {
                return !(operator!=(rhs));
            }

            /**
             * Arithmetic operators
             */
            Index operator+(const Index<rank_>& rhs) const {
                Index updated;
                for (int idx = 0; idx < rank_; ++idx)
                    updated[idx] = at(idx) + rhs.at(idx);
                return updated;
            };

            Index operator-(const Index<rank_>& rhs) const {
                Index updated;
                for (int idx = 0; idx < rank_; ++idx)
                    updated[idx] = at(idx) - rhs.at(idx);
                return updated;
            }

            Index& operator+=(const Index<rank_>& rhs) const {
                return *this +rhs;
            }

            Index& operator-=(const Index<rank_>& rhs) const {
                return *this -rhs;
            }

            /**
             * Scaling operators
             */

            template<typename ArithmeticType>
            Index operator*(ArithmeticType v) const {
                static_assert(std::is_arithmetic<ArithmeticType>::value, "The type in Index's scaling operator should be of arithmetic type");
                Index updated;
                for (int idx = 0; idx < rank_; ++idx)
                    updated[idx] = at(idx) * v;
            }

            template<typename ArithmeticType>
            Index operator/(ArithmeticType v) const {
                static_assert(std::is_arithmetic<ArithmeticType>::value, "The type in Index's scaling operator should be of arithmetic type");
                return operator*(1 / v);
            }

            template<typename ArithmeticType>
            Index& operator*=(ArithmeticType v) const {
                static_assert(std::is_arithmetic<ArithmeticType>::value, "The type in Index's scaling operator should be of arithmetic type");
                return (*this) * v;
            }

            template<typename ArithmeticType>
            Index& operator/=(ArithmeticType v) const {
                static_assert(std::is_arithmetic<ArithmeticType>::value, "The type in Index's scaling operator should be of arithmetic type");
                return (*this) / v;
            }


        private:
            value_type data_[rank_];
        };
    }
}

#endif 

