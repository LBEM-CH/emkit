
#ifndef INDEX_VALUE_PAIR_HPP
#define INDEX_VALUE_PAIR_HPP

#include <iostream>
#include <type_traits>

#include "index.hpp"
#include "tensor_storage_order.hpp"

namespace em {
    namespace element {

        template<typename ValueType_, size_t rank_>
        class IndexValuePair {
            
        public:
            
            static const size_t rank = rank_;
            typedef ValueType_ value_type;
            typedef Index<rank_> index_type;
            
            IndexValuePair() {};
            
            IndexValuePair(Index<rank_>& idx, ValueType_* val)
            : index_(idx), value_(val) {
            };
            
            Index<rank_> index() const{
                return index_;
            }
            
            ValueType_& value() {
                return *(value_);
            }
            
            const ValueType_& value() const {
                return *(value_);
            }
            
            private:
            Index<rank_> index_;
            ValueType_* value_;
 
        };
        
        template<typename IndexValuePair_>
        typename IndexValuePair_::index_type 
        index(IndexValuePair_ pair) {
            return pair.index();
        }
        
        template<typename IndexValuePair_>
        typename IndexValuePair_::value_type 
        value(IndexValuePair_ pair) {
            return pair.value();
        }
    }
}


#endif 