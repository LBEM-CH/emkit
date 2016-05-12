
#ifndef EM_COMPLEX_OBJECT_HPP
#define EM_COMPLEX_OBJECT_HPP

#include "../elements/complex.hpp"
#include "../elements/tensor.hpp"
#include "../elements/tensor_iterator.hpp"

namespace em {
    
    namespace object {

        template<typename APHValueType_, size_t rank_>
        class ComplexObject {
        public:

            typedef element::Tensor<element::Complex<APHValueType_>, rank_, element::StorageOrder::COLUMN_MAJOR> container_type;
            typedef element::Complex<APHValueType_> value_type;
            typedef element::Index<rank_> index_type;
            typedef element::Range<rank_> range_type;
            typedef typename container_type::iterator iterator;
            typedef typename container_type::const_iterator const_iterator;

            /**
             * Constructors
             */

            ComplexObject()
            : container_(), even_size_x_(false) {
            };

            /**
             * Constructor using a logical size (size of the corresponding real object)
             * @param range
             */
            ComplexObject(const range_type& range) {
                range_type complex_container_range = range;
                complex_container_range[0] = complex_container_range[0] / 2 + 1;
                container_ = container_type(complex_container_range, value_type());
                even_size_x_ = false;
                if (range[0] % 2 == 0) even_size_x_ = true;
            };

            ComplexObject(const index_type& miller_index_min, const index_type& miller_index_max) {
                range_type range;
                range[0] = (miller_index_max[0] + 1);
                for (int i = 1; i < range.rank; ++i) range[i] = std::abs(miller_index_max[i] - miller_index_min[i]) + 1;
                even_size_x_ = false;
                if ((std::abs(miller_index_max[0] - miller_index_min[0]) + 1) % 2 == 0) even_size_x_ = true;
            };

            ComplexObject(const container_type& container, bool is_first_dim_even)
            : container_(container), even_size_x_(is_first_dim_even) {
            };

            ComplexObject(const ComplexObject& other) = default;

            /**
             * Accessing elements
             */

            value_type& operator[](const index_type& idx) {
                if (idx[0] < 0) {
                    std::cerr << "Expected only positive values of h, found: " << idx[0] << "\n";
                    exit(1);
                }
                return container_[idx];
            }

            const value_type& operator[](const index_type& idx) const {
                if (idx[0] < 0) {
                    std::cerr << "Expected only positive values of h, found: " << idx[0] << "\n";
                    exit(1);
                }
                return container_[idx];
            }

            value_type& at(const index_type& idx) {
                if (idx[0] < 0) {
                    std::cerr << "Expected only positive values of h, found: " << idx[0] << "\n";
                    exit(1);
                }
                return container_.at(idx);
            }

            const value_type& at(const index_type& idx) const {
                if (idx[0] < 0) {
                    std::cerr << "Expected only positive values of h, found: " << idx[0] << "\n";
                    exit(1);
                }
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

            index_type miller_index_max() const {
                index_type max_idx;
                max_idx[0] = container_.range()[0] - 1;
                for (int r = 1; r < max_idx.rank; ++r) max_idx[r] = container_.range()[r] / 2;
                return max_idx;
            }

            index_type miller_index_min() const {
                index_type max_idx = miller_index_max();
                index_type min_idx;
                if (even_size_x_) min_idx[0] = -1 * (max_idx[0] - 1);
                else min_idx[0] = -1 * max_idx[0];
                for (int r = 1; r < max_idx.rank; ++r) min_idx[r] = max_idx[r] - container_.range()[r] + 1;
                return min_idx;
            }

            range_type logical_range() const {
                range_type rnge = range();
                if (even_size_x_) rnge[0] = (rnge[0] - 1)*2;
                else rnge[0] = rnge[0]*2 - 1;
                return rnge;
            }

            range_type range() const {
                return container_.range();
            }

            /**
             * Arithmetic operators
             */
            ComplexObject<APHValueType_, rank_> operator+(const ComplexObject<APHValueType_, rank_>& right) const {
                ComplexObject<APHValueType_, rank_> result(*this); // Make a copy of myself.
                result += right; // Reuse compound assignment
                return result;
            }

            ComplexObject<APHValueType_, rank_>& operator+=(const ComplexObject<APHValueType_, rank_>& right) {
                container_ += right.container_;
                return *this;
            }

            ComplexObject<APHValueType_, rank_> operator-(const ComplexObject<APHValueType_, rank_>& right) const {
                ComplexObject<APHValueType_, rank_> result(*this); // Make a copy of myself.
                result -= right; // Reuse compound assignment
                return result;
            }

            ComplexObject<APHValueType_, rank_>& operator-=(const ComplexObject<APHValueType_, rank_>& right) {
                container_ -= right.container_;
                return *this;
            }

            /**
             * Scaling operators
             */
            template<typename ArithmeticType_>
            ComplexObject<APHValueType_, rank_> operator*(const ArithmeticType_& right) const {
                ComplexObject<APHValueType_, rank_> result(*this); // Make a copy of myself.
                result *= right; // Reuse compound assignment
                return result;
            }

            template<typename ArithmeticType_>
            ComplexObject<APHValueType_, rank_>& operator*=(const ArithmeticType_& right) {
                container_ *= right;
                return *this;
            }

            template<typename ArithmeticType_>
            ComplexObject<APHValueType_, rank_> operator/(const ArithmeticType_& right) const {
                ComplexObject<APHValueType_, rank_> result(*this); // Make a copy of myself.
                result /= right; // Reuse compound assignment
                return result;
            }

            template<typename ArithmeticType_>
            ComplexObject<APHValueType_, rank_>& operator/=(const ArithmeticType_& right) {
                container_ /= right;
                return *this;
            }


        private:
            container_type container_;
            bool even_size_x_;

        };
    }
}

#endif /* REAL_OBJECT_HPP */

