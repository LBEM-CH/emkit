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

#ifndef EM_MULTIDIM_INDEX_HPP
#define EM_MULTIDIM_INDEX_HPP

#include <iostream>
#include <array>
#include <cassert>
#include <string>
#include <type_traits>
#include <cstddef>
#include <initializer_list>

namespace em {
    namespace element {

        /**
         * @brief       A set of indices of a particular rank
         * @description Defines a set of indices with rank given as templated
         *              parameter. The indices stored can be signed integers like
         *              the ptrdiff_t
         * @see         em::multidim::Range
         */
        template<size_t rank_>
        class Index {
        public:
            static const size_t rank = rank_;
            using value_type = std::ptrdiff_t;
            using reference = value_type&;
            using const_reference = const value_type&;
            using size_type = size_t;

            /******************************
             * @defgroup constructors
             * @{
             ******************************/

            /**
             * @brief   Default constructor
             */
            Index() = default;

            /**
             * @brief   Sets all the indices to the given value
             * @param   value   value to be set for all the indices
             */
            Index(value_type value) {
                for (int idx=0; idx< rank_; ++idx)
                    data_[idx] = value;
            };

            /**
             * @brief   Constructor using initializer list
             * @pre     The size of list should be equal to the rank
             * @param   list
             */
            Index(const std::initializer_list<value_type>& list) {
                assert(rank_ == list.size());
                std::copy(list.begin(), list.end(), data_.begin());
            };

            /*
             * @brief   Copy constructor, copies all the indices
             */
            Index(const Index& other) {
                data_ = other.data_;
            };


            /**@}*/


            /*********************************************
             * @defgroup AssignmentOperators
             * @{
             *********************************************/

            /**
             * @brief   Assignment operator, equates all the indices
             */
            Index& operator=(const Index& rhs) {
                data_ = rhs.data_;
                return *this;
            }

            /**@}*/


            /*********************************************
             * @defgroup ElementAccess Accessing elements
             * @{
             *********************************************/

            /**
             * @brief   Returns the reference of index at the position provided
             * @param   position
             * @return  reference
             */
            reference operator[](value_type position) {
                return data_[position];
            }

            /**
             * @brief   Returns the const reference of index at the position provided
             * @param   position
             * @return  const reference
             */
            const_reference operator[](value_type position) const {
                return data_[position];
            }

            /**
             * @brief   Returns the reference of index at the position provided
             * @pre     Position should be in [0, rank)
             * @param   position
             * @return  reference
             */
            reference at(value_type position) {
                assert(position < rank_ && position >= 0);
                return data_.at(position);
            }

            /**
             * @brief   Returns the const reference of index at the position provided
             * @pre     Position should be in [0, rank)
             * @param   position
             * @return  const reference
             */
            const_reference at(value_type position) const {
                assert(position < rank_ && position >= 0);
                return data_.at(position);
            }

            /**@}*/


            /*********************************************
             * @defgroup ComparisonOperators comparison operators
             * @{
             *********************************************/

            /**
             * @brief   Compares elements and if any one of the element does 
             *          not equals the corresponding rhs element returns true
             *          else returns false
             * @param   rhs
             * @return  true for unmatch, false otherwise
             */
            bool operator!=(const Index& rhs) const {
                for (int idx = 0; idx < rank_; ++idx)
                    if (at(idx) != rhs.at(idx))
                        return true;

                return false;
            }

            /**
             * @brief   Compares elements and if any one of the element does 
             *          not equals the corresponding rhs element returns false
             *          else returns true
             * @param   rhs
             * @return  true for match, false otherwise
             */
            bool operator==(const Index& rhs) const {
                for (int idx = 0; idx < rank_; ++idx)
                    if (at(idx) != rhs.at(idx))
                        return false;

                return true;
            }

            /**@}*/


            /*********************************************
             * @defgroup ArithmeticOperators arithmetic operators
             * @{
             *********************************************/

            /**
             * @brief   Addition operator, performs element-wise numerical 
             *          addition.
             * @param   rhs
             * @return  added index
             */
            Index operator+(const Index<rank_>& rhs) const {
                Index updated;
                for (int idx = 0; idx < rank_; ++idx)
                    updated[idx] = at(idx) + rhs.at(idx);
                return updated;
            };

            /**
             * @brief   Subtraction operator, performs element-wise numerical 
             *          subtraction.
             * @param   rhs
             * @return  subtracted index
             */
            Index operator-(const Index<rank_>& rhs) const {
                Index updated;
                for (int idx = 0; idx < rank_; ++idx)
                    updated[idx] = at(idx) - rhs.at(idx);
                return updated;
            }

            /**
             * @brief   Inplace addition operator, performs element-wise numerical 
             *          addition.
             * @param   rhs
             * @return  added index
             */
            Index& operator+=(const Index<rank_>& rhs) const {
                return *this +rhs;
            }

            /**
             * @brief   Inplace subtraction operator, performs element-wise numerical 
             *          subtraction.
             * @param   rhs
             * @return  subtracted index
             */
            Index& operator-=(const Index<rank_>& rhs) const {
                return *this -rhs;
            }

            /**@}*/


            /*********************************************
             * @defgroup ScalingOperators scaling operators
             * @{
             *********************************************/

            /**
             * @brief   Multiplies all the indices by the factor provided
             * @pre     Template parameter is of arithmetic type
             * @param   factor
             * @return  multiplied index
             */
            template<typename ArithmeticType>
            Index operator*(ArithmeticType factor) const {
                static_assert(std::is_arithmetic<ArithmeticType>::value, "The type in Index's scaling operator should be of arithmetic type");
                Index updated;
                for (int idx = 0; idx < rank_; ++idx)
                    updated[idx] = at(idx) * factor;
                return updated;
            }

            /**
             * @brief   Divides all the indices by the factor provided
             * @pre     Template parameter is of arithmetic type
             * @param   factor
             * @return  divided index
             */
            template<typename ArithmeticType>
            Index operator/(ArithmeticType factor) const {
                static_assert(std::is_arithmetic<ArithmeticType>::value, "The type in Index's scaling operator should be of arithmetic type");
                return operator*(1 / factor);
            }

            /**
             * @brief   Multiplies all the indices by the factor provided in place
             * @pre     Template parameter is of arithmetic type
             * @param   factor
             * @return  multiplied index
             */
            template<typename ArithmeticType>
            Index& operator*=(ArithmeticType v) const {
                static_assert(std::is_arithmetic<ArithmeticType>::value, "The type in Index's scaling operator should be of arithmetic type");
                return (*this) * v;
            }

            /**
             * @brief   Divides all the indices by the factor provided in place
             * @pre     Template parameter is of arithmetic type
             * @param   factor
             * @return  divided index
             */
            template<typename ArithmeticType>
            Index& operator/=(ArithmeticType v) const {
                static_assert(std::is_arithmetic<ArithmeticType>::value, "The type in Index's scaling operator should be of arithmetic type");
                return (*this) / v;
            }

            /**@}*/
            
            friend inline std::ostream& operator<<(std::ostream& os, const Index& obj) {
                os << " ( ";
                for(const auto& i: obj.data_) os << i << " ";
                os << ") ";
                return os;
            }

            double distance(const Index& other) {
                Index<3> diff = *this-other;
                double dist = 0;
                for(int i=0; i<3; ++i) dist += diff[i]*diff[i];
                return dist;
            }
            
        private:
            std::array<value_type, rank_> data_; //Data stored as array
        };
        
        template<size_t rank_>
        double distance(const Index<rank_>& idx1, const Index<rank_>& idx2) {
            return idx1.distance(idx2);
        }
    }
}

#endif 

