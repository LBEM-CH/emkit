
#ifndef EM_OBJECT_HPP
#define EM_OBJECT_HPP

#include <iostream>

#include "../elements/properties_map.hpp"

#include "../elements/index.hpp"
#include "../elements/range.hpp"
#include "../elements/tensor.hpp"

namespace em {
    
    namespace object {

        template <typename ValueType_, size_t rank_>
        class Object {
        public:
            typedef typename element::Tensor<ValueType_, rank_> container_type;
            static const int rank = container_type::rank;
            typedef typename element::Index<rank_> index_type;
            typedef typename element::Range<rank_> range_type;
            typedef ValueType_ value_type;

            /**
             * Constructors
             */
            Object()
            : container_() {
            };
            
            Object(const range_type& range)
            : container_(container_type(range, ValueType_())){
            }
            
            Object(const range_type& range, const index_type& origin)
            : container_(container_type(range, origin, ValueType_())){
            }

            Object(const container_type& container)
            : container_(container) {
            };

            Object(const Object& other)
            : container_(other.container_), properties_(other.properties_) {
            }

            element::PropertiesMap& properties() {
                return properties_;
            };

            const element::PropertiesMap& properties() const {
                return properties_;
            };

            container_type& container() {
                return container_;
            };

            const container_type& container() const {
                return container_;
            };

        private:
            container_type container_;
            element::PropertiesMap properties_;

        };
    }
}

#endif

