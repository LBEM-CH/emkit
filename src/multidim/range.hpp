
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

        /**
         * @brief       A set of indices of a particular rank
         * @description Defines a set of indices with rank given as templated
         *              parameter. The indices stored can be signed integers like
         *              the ptrdiff_t
         * @see         em::multidim::Range
         */
        template<size_t rank_>
        class Range {
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
             * @ingroup constructor
             */
            Range() = default;

            /**
             * @brief   Sets all the indices to the given value
             * @ingroup constructor
             * @param   value   value to be set for all the indices
             */
            Range(value_type value) {
                for (int idx=0; idx< rank_; ++idx)
                    data_[idx] = value;
            };

            /**
             * @brief   Constructor using initializer list
             * @ingroup constructor
             * @pre     The size of list should be equal to the rank
             * @param   list
             */
            Range(const std::initializer_list<value_type>& list) {
                assert(rank_ == list.size());
                std::copy(list.begin(), list.end(), data_.begin());
            };

            /*
             * @brief   Copy constructor, copies all the indices
             * @ingroup constructor
             */
            Range(const Range& other) {
                data_ = other.data_;
            };

            /**@}*/


            /*********************************************
             * @defgroup AssignmentOperators
             * @{
             *********************************************/

            /**
             * @brief   Assignment operator, equates all the indices
             * @ingroup assignment
             */
            Range& operator=(const Range& rhs) {
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
             * @ingroup element_access
             * @param   position
             * @return  reference
             */
            reference operator[](value_type position) {
                return data_[position];
            }

            /**
             * @brief   Returns the const reference of index at the position provided
             * @ingroup element_access
             * @param   position
             * @return  const reference
             */
            const_reference operator[](value_type position) const {
                return data_[position];
            }

            /**
             * @brief   Returns the reference of index at the position provided
             * @ingroup element_access
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
             * @ingroup element_access
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
             * @ingroup comparison_operator
             * @param   rhs
             * @return  true for unmatch, false otherwise
             */
            bool operator!=(const Range& rhs) const {
                for (int idx = 0; idx < rank_; ++idx)
                    if (at(idx) != rhs.at(idx))
                        return true;

                return false;
            }

            /**
             * @brief   Compares elements and if any one of the element does 
             *          not equals the corresponding rhs element returns false
             *          else returns true
             * @ingroup comparison_operator
             * @param   rhs
             * @return  true for match, false otherwise
             */
            bool operator==(const Range& rhs) const {
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
             * @ingroup arithmetic_operators
             * @param   rhs
             * @return  added index
             */
            Range operator+(const Index<rank_>& rhs) const {
                Range updated;
                for (int idx = 0; idx < rank_; ++idx)
                    updated[idx] = at(idx) + rhs.at(idx);
                return updated;
            };

            /**
             * @brief   Subtraction operator, performs element-wise numerical 
             *          subtraction.
             * @ingroup arithmetic_operators
             * @param   rhs
             * @return  subtracted index
             */
            Range operator-(const Index<rank_>& rhs) const {
                Range updated;
                for (int idx = 0; idx < rank_; ++idx)
                    updated[idx] = at(idx) - rhs.at(idx);
                return updated;
            }

            /**
             * @brief   Inplace addition operator, performs element-wise numerical 
             *          addition.
             * @ingroup arithmetic_operators
             * @param   rhs
             * @return  added index
             */
            Range& operator+=(const Index<rank_>& rhs) const {
                return *this +rhs;
            }

            /**
             * @brief   Inplace subtraction operator, performs element-wise numerical 
             *          subtraction.
             * @ingroup arithmetic_operators
             * @param   rhs
             * @return  subtracted index
             */
            Range& operator-=(const Index<rank_>& rhs) const {
                return *this -rhs;
            }

            /**@}*/



            /*********************************************
             * @defgroup ScalingOperators scaling operators
             * @{
             *********************************************/

            /**
             * @brief   Multiplies all the indices by the factor provided
             * @ingroup scaling_operators
             * @pre     Template parameter is of arithmetic type
             * @param   factor
             * @return  multiplied index
             */
            template<typename ArithmeticType>
            Range operator*(ArithmeticType factor) const {
                static_assert(std::is_arithmetic<ArithmeticType>::value, "The type in Range's scaling operator should be of arithmetic type");
                Range updated;
                for (int idx = 0; idx < rank_; ++idx)
                    updated[idx] = at(idx) * factor;
            }

            /**
             * @brief   Divides all the indices by the factor provided
             * @ingroup scaling_operators
             * @pre     Template parameter is of arithmetic type
             * @param   factor
             * @return  divided index
             */
            template<typename ArithmeticType>
            Range operator/(ArithmeticType factor) const {
                static_assert(std::is_arithmetic<ArithmeticType>::value, "The type in Range's scaling operator should be of arithmetic type");
                return operator*(1 / factor);
            }

            /**
             * @brief   Multiplies all the indices by the factor provided in place
             * @ingroup scaling_operators
             * @pre     Template parameter is of arithmetic type
             * @param   factor
             * @return  multiplied index
             */
            template<typename ArithmeticType>
            Range& operator*=(ArithmeticType v) const {
                static_assert(std::is_arithmetic<ArithmeticType>::value, "The type in Range's scaling operator should be of arithmetic type");
                return (*this) * v;
            }

            /**
             * @brief   Divides all the indices by the factor provided in place
             * @ingroup scaling_operators
             * @pre     Template parameter is of arithmetic type
             * @param   factor
             * @return  divided index
             */
            template<typename ArithmeticType>
            Range& operator/=(ArithmeticType v) const {
                static_assert(std::is_arithmetic<ArithmeticType>::value, "The type in Range's scaling operator should be of arithmetic type");
                return (*this) / v;
            }


            /**@}*/


            /*********************************************
             * @defgroup MemberFunctions member functions
             * @{
             *********************************************/

            /**
             * @brief       Returns the hyper volume of the rectangular space 
             *              enclosed ,ie, product of all components  
             * @ingroup     member_functions
             * @return the size
             */
            const value_type size() const {
                value_type sz = 1;
                for (int idx = 0; idx < rank_; ++idx) sz = sz*at(idx);
                return sz;
            }

            /**
             * @brief       Checks if the index provided is contained in the range
             * @ingroup     member_functions
             * @param       index
             * @return 
             */
            bool contains(const Index<rank_>& index) const {
                for (int idx = 0; idx < rank_; ++idx)
                    if (std::abs(index[idx]) >= at(idx))
                        return false;
                return true;
            }
            
            /**
             * @brief       Checks if the range provided is contained in the range
             * @ingroup     member_functions
             * @param       range
             * @return 
             */
            bool contains(const Range& index) const {
                for (int idx = 0; idx < rank_; ++idx)
                    if (std::abs(index[idx]) >= at(idx))
                        return false;
                return true;
            }

            /**@}*/
            
            
            friend inline std::ostream& operator<<(std::ostream& os, const Range& obj) {
                os << " ( ";
                for(const auto& i: obj.data_) os << i << " ";
                os << ") ";
                return os;
            }

        private:
            std::array<value_type, rank_> data_; //Data stored as array
        };

        /**
         * @brief   Adds an index to the range
         * @param   lhs index
         * @param   rhs range
         * @return 
         */
        template<size_t rank>
        Range<rank> operator+(const Index<rank>& lhs, const Range<rank>& rhs) {
            return rhs + lhs;
        };

        /**
         * @brief   Multiplies a factor to the range
         * @param   lhs index
         * @param   rhs range
         * @return 
         */
        template<typename ArithmeticType, size_t rank>
        Range<rank> operator*(ArithmeticType v, const Range<rank>& rhs) {
            static_assert(std::is_arithmetic<ArithmeticType>::value, "The type in Range's scaling operator should be of arithmetic type");
            return rhs*v;
        };

    }
}

#endif

