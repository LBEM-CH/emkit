
#ifndef EM_MULTIDIM_RANGE_HPP
#define EM_MULTIDIM_RANGE_HPP


#include <iostream>
#include <cassert>
#include <string>
#include <type_traits>
#include <cstddef>
#include <initializer_list>

#include "index.hpp"

namespace em {
    namespace multidim {
        
        template<int rank_=1>
        class Range {
            
        public:
            static const size_t rank = rank_;
            using value_type = std::ptrdiff_t;
            using reference =  std::ptrdiff_t&;
            using const_reference = std::add_const<std::ptrdiff_t&>;
            using size_type = size_t;

            /**
             * Constructors
             */
            
            Range() 
            : data_() {
                static_assert(rank_ <= 0, "Rank should be grater than 0 to construct range");
            };

            Range(value_type value){
                static_assert(rank_ == 1, "Rank should be 1 for the given Range constructor");
                data_ = value;
            };

            Range(const std::initializer_list<value_type>& list)
            : data_(list) {
                static_assert(rank_ == list.size(), "Rank should be equal to the size of the list in Range constructor");
            };
            
            Range(const Range& other) {
                data_ = other.data_;
            };
            
            /**
             * Assignment operator
             */
            Range& operator=(const Range& rhs) {
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
                static_assert(idx < rank_ && idx>0, "id should be smaller than the rank" );
                return data_[idx];
            }

            const_reference at(value_type idx) const {
                static_assert(idx < rank_ && idx>0, "id should be smaller than the rank" );
                return data_[idx];
            }


            /**
             * Comparison operators
             */
            bool operator!=(const Range& rhs) const {
                for(int idx=0; idx < rank_; ++idx) 
                    if(at(idx) != rhs.at(idx))
                        return false;
                
                return true;
            }
            
            bool operator==(const Range& rhs) const {
                return !(operator!=(rhs));
            }
            
            /**
             * Arithmetic operators
             */
            Range operator+(const Index<rank_>& rhs) const {
                Range updated;
                for(int idx=0; idx < rank_; ++idx)
                    updated[idx] = at(idx) + rhs.at(idx);
                return updated;
            };
            
            Range operator-(const Index<rank_>& rhs) const {
                Range updated;
                for(int idx=0; idx < rank_; ++idx)
                    updated[idx] = at(idx) - rhs.at(idx);
                return updated;
            }
            
            Range& operator+=(const Index<rank_>& rhs) const {
                return *this + rhs;
            }
            
            Range& operator-=(const Index<rank_>& rhs) const {
                return *this - rhs;
            }
            
            /**
             * Scaling operators
             */
            
            template<typename ArithmeticType>
            Range operator*(ArithmeticType v) const {
                static_assert(std::is_arithmetic<ArithmeticType>::value, "The type in Range's scaling operator should be of arithmetic type");
                Range updated;
                for(int idx=0; idx < rank_; ++idx)
                    updated[idx] = at(idx)*v;
            }
            
            template<typename ArithmeticType>
            Range operator/(ArithmeticType v) const {
                static_assert(std::is_arithmetic<ArithmeticType>::value, "The type in Range's scaling operator should be of arithmetic type");
                return operator*(1/v);
            }
            
            template<typename ArithmeticType>
            Range& operator*=(ArithmeticType v) const {
                static_assert(std::is_arithmetic<ArithmeticType>::value, "The type in Range's scaling operator should be of arithmetic type");
                return (*this) * v;
            }
            
            template<typename ArithmeticType>
            Range& operator/=(ArithmeticType v) const {
                static_assert(std::is_arithmetic<ArithmeticType>::value, "The type in Range's scaling operator should be of arithmetic type");
                return (*this) / v;
            }
            
            /**
             * Functions
             */
            
            /**
             * @brief       Returns the hyper volume of the rectangular space 
             *              enclosed ,ie, product of all components  
             * @return the size
             */
            const value_type size() const {
                value_type size = 1;
                for(int idx=0; idx<rank_; ++idx) size *= at(idx);
                return size;
            }
            
            bool contains(const Index<rank_>& index) const {
                for(int idx=0; idx<rank_; ++idx)
                    if(index[idx] >= at(idx) || index.at(idx) < 0) 
                        return false;
                return true;
            } 
            
            
        private:
            value_type data_[rank_];

        };
        
        template<size_t rank>
        Range<rank> operator+(const Index<rank>& lhs, const Range<rank>& rhs) {
            return rhs+lhs;
        };
        
        template<typename ArithmeticType, size_t rank>
        Range<rank> operator*(ArithmeticType v, const Range<rank>& rhs) {
            static_assert(std::is_arithmetic<ArithmeticType>::value, "The type in Range's scaling operator should be of arithmetic type");
            return rhs*v;
        };

    }
}

#endif

