
#ifndef EM_OBJECT_HPP
#define EM_OBJECT_HPP

#include <iostream>

#include "properties_map.hpp"

namespace em {

    template <typename ValueType_, size_t rank_>
    class Object {
    public:
        typedef typename multidim::Tensor<ValueType_, rank_> container_type;
        static const int rank = container_type::rank;
        typedef typename multidim::Index<rank_> index_type;
        typedef typename multidim::Range<rank_> range_type;
        typedef typename ValueType_ value_type;

        /**
         * Constructors
         */
        Object()
        : container_() {
        };

        Object(const container_type& container)
        : container_(container) {
        };

        Object(const Object& other)
        : container_(other.container_), properties_(other.properties_) {
        }

        PropertiesMap& properties() {
            return properties_;
        };

        const PropertiesMap& properties() const {
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
        PropertiesMap properties_;

    };
}

#endif

