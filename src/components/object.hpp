
#ifndef EM_OBJECT_HPP
#define EM_OBJECT_HPP

#include <iostream>

#include "properties_map.hpp"
#include "../multidim/tensor_traits.hpp"

namespace em {

    template <typename TensorContainer_>
    class ObjectBase {
    public:
        typedef typename TensorContainer_ container_type;
        static const int rank = multidim::tensor_traits<container_type>::rank;
        typedef typename multidim::tensor_traits<container_type>::index_type index_type;
        typedef typename multidim::tensor_traits<container_type>::range_type range_type;
        typedef typename multidim::tensor_traits<container_type>::size_type size_type;
        typedef typename multidim::tensor_traits<container_type>::value_type value_type;
        typedef typename multidim::tensor_traits<container_type>::iterator iterator;
        typedef typename multidim::tensor_traits<container_type>::const_iterator const_iterator;
        typedef typename multidim::tensor_traits<container_type>::reference reference;
        typedef typename multidim::tensor_traits<container_type>::const_reference const_reference;
        typedef typename multidim::tensor_traits<container_type>::pointer pointer;
        typedef typename multidim::tensor_traits<container_type>::difference_type difference_type;

        /**
         * Constructors
         */
        ObjectBase()
        : container_() {
        };

        ObjectBase(const container_type& container)
        : container_(container) {
        };

        ObjectBase(const ObjectBase& other)
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

