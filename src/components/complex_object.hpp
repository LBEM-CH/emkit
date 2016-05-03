
#ifndef EM_COMPLEX_OBJECT_HPP
#define EM_COMPLEX_OBJECT_HPP

#include "../helper/complex.hpp"
#include "object.hpp"
#include "fourier_space_interface.hpp"

namespace em {

    template<typename APHValueType_, size_t rank_>
    class ComplexObject : public Object<Complex<APHValueType_>,rank_>, public FourierSapceInterface {
    public:
        
        typedef Object<Complex<APHValueType_>,rank_> base_type;
        
        ComplexObject()
        : base_type() {
        };

        ComplexObject(const typename base_type::container_type& container)
        : base_type(container) {
        };

        ComplexObject(const ComplexObject& other) = default;

    };
}

#endif /* REAL_OBJECT_HPP */

