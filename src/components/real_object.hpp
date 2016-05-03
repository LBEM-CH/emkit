
#ifndef EM_REAL_OBJECT_HPP
#define EM_REAL_OBJECT_HPP

#include "object.hpp"
#include "real_space_interface.hpp"

namespace em {
    
    template<typename DensityValueType_, size_t rank_>
    class RealObject : public Object<DensityValueType_, rank_>, public RealSpaceInterface {
    public:
        
        typedef Object<DensityValueType_, rank_> base_type;
        
        RealObject()
        : base_type() {
        };

        RealObject(const typename base_type::container_type& container)
        : base_type(container) {
        };

        RealObject(const RealObject& other) = default;

    };
}

#endif

