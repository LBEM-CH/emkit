
#ifndef EM_REAL_OBJECT_HPP
#define EM_REAL_OBJECT_HPP

#include "object.hpp"
#include "real_space_interface.hpp"

namespace em {
    
    template<typename DensityValueType_, size_t rank_>
    class RealObject : public ObjectBase<multidim::Tensor<DensityValueType_, rank_> >, public RealSpaceInterface {
    public:
        
        typedef typename ObjectBase<multidim::Tensor<DensityValueType_, rank_>> tensor_type;
        
        RealObject()
        : tensor_type() {
        };

        RealObject(const tensor_type::container_type& container)
        : tensor_type(container) {
        };

        RealObject(const RealObject& other) = default;

    };
}

#endif /* REAL_OBJECT_HPP */

