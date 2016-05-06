
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
        typedef Complex<APHValueType_> complex_type;
        
        /**
         * Constructors
         */
        
        ComplexObject()
        : base_type() {
        };
        
        ComplexObject(const typename base_type::range_type& range)
        : base_type(base_type::container_type(range), complex_type()) {
        };

        ComplexObject(const typename base_type::container_type& container)
        : base_type(container) {
        };
        
        ComplexObject(std::string file_name) {
            load(file_name);
        }

        ComplexObject(const ComplexObject& other) = default;
        
        /**
         * Element Access
         */
        complex_type& operator[](const typename base_type::index_type& index){
            
        }
        
        /**
         * File load/save
         */
        
        void load(std::string file_name) {
            //TODO load from file
        };
        
        void save(std::string file_name) {
            //TODO save to file
        }

    };
}

#endif /* REAL_OBJECT_HPP */

