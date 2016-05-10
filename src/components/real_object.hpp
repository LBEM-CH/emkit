
#ifndef EM_REAL_OBJECT_HPP
#define EM_REAL_OBJECT_HPP

#include "object.hpp"
#include "complex_object.hpp"

namespace em {
    
    template<typename DensityValueType_, size_t rank_>
    class RealObject {
    public:

        typedef multidim::Tensor<DensityValueType_, rank_, multidim::StorageOrder::COLUMN_MAJOR> container_type;
        typedef DensityValueType_ value_type;
        typedef multidim::Index<rank_> index_type;
        typedef multidim::Range<rank_> range_type;
        typedef typename container_type::iterator iterator;
        typedef typename container_type::const_iterator const_iterator;
        

        RealObject()
        : container_() {
        };

        RealObject(const range_type& range)
        : container_(container_type(range, value_type())) {
        };

        RealObject(const container_type& container)
        : container_(container) {
        };
        
        RealObject(const ComplexObject<DensityValueType_, rank_>& complex) {
            convert(complex.logical_range(), complex.container(), container_);
        };
        
        RealObject(const std::string file_name) {
            //TODO, load the file to the container_
        }

        RealObject(const RealObject& other) = default;
        
        
        /**
         * Accessing elements
         */
        
        value_type& operator[](const index_type& idx) {
            return container_[idx];
        }
        
        const value_type& operator[](const index_type& idx) const{
            return container_[idx];
        }
        
        value_type& at(const index_type& idx) {
            return container_.at(idx);
        }
        
        const value_type& at(const index_type& idx) const {
            return container_.at(idx);
        }
        
        container_type& container() {
            return container_;
        }
        
        const container_type& container() const {
            return container_;
        }
        
        /**
         * Iterators
         */
        
        iterator begin() {
            return container_.begin();
        }
        
        const_iterator begin() const {
            return container_.begin();
        }
        
        iterator end() {
            return container_.end();
        }
        
        const_iterator end() const {
            return container_.end();
        }
        
        /**
         * Capacity
         */
        range_type range() const {
            return container_.range();
        }
        
        /**
         * Arithmetic operators
         */
        
        RealObject<DensityValueType_, rank_> operator+(const RealObject<DensityValueType_, rank_>& right) const {
            RealObject<DensityValueType_, rank_> result(*this); // Make a copy of myself.
            result += right; // Reuse compound assignment
            return result;
        }

        RealObject<DensityValueType_, rank_>& operator+=(const RealObject<DensityValueType_, rank_>& right) {
            container_ += right.container_;
            return *this;
        }

        RealObject<DensityValueType_, rank_> operator-(const RealObject<DensityValueType_, rank_>& right) const {
            RealObject<DensityValueType_, rank_> result(*this); // Make a copy of myself.
            result -= right; // Reuse compound assignment
            return result;
        }

        RealObject<DensityValueType_, rank_>& operator-=(const RealObject<DensityValueType_, rank_>& right) {
            container_ -= right.container_;
            return *this;
        }
        
        /**
         * Scaling operators
         */
        template<typename ArithmeticType_>
        RealObject<DensityValueType_, rank_> operator*(const ArithmeticType_& right) const {
            RealObject<DensityValueType_, rank_> result(*this); // Make a copy of myself.
            result *= right; // Reuse compound assignment
            return result;
        }

        template<typename ArithmeticType_>
        RealObject<DensityValueType_, rank_>& operator*=(const ArithmeticType_& right) {
            container_ *= right;
            return *this;
        }

        template<typename ArithmeticType_>
        RealObject<DensityValueType_, rank_> operator/(const ArithmeticType_& right) const {
            RealObject<DensityValueType_, rank_> result(*this); // Make a copy of myself.
            result /= right; // Reuse compound assignment
            return result;
        }

        template<typename ArithmeticType_>
        RealObject<DensityValueType_, rank_>& operator/=(const ArithmeticType_& right) {
            container_ /= right;
            return *this;
        }



    private:
        container_type container_;

    };
}

#endif

