
#ifndef EM_COMPLEX_OBJECT_HPP
#define EM_COMPLEX_OBJECT_HPP

#include "../helper/complex.hpp"
#include "object.hpp"
#include "fourier_space_interface.hpp"

namespace em {

    template<typename APHValueType_, size_t rank_>
    class ComplexObject : public ObjectBase<multidim::Tensor<Complex<APHValueType_>, rank_>>, public FourierSapceInterface {
    public:
        
        typedef typename ObjectBase<multidim::Tensor<Complex<APHValueType_>, rank_>> container_type;
        
        ComplexObject()
        : container_type() {
        };

        ComplexObject(const container_type::container_type& container)
        : container_type(container) {
        };

        ComplexObject(const ComplexObject& other) = default;

    };
}

#endif /* REAL_OBJECT_HPP */

