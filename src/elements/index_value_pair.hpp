
#ifndef INDEX_VALUE_PAIR_HPP
#define INDEX_VALUE_PAIR_HPP

#include "index.hpp"

namespace em {
    namespace element {

        template<typename ValueType_, size_t rank_>
        class IndexValuePair {
            
        public:
            
            IndexValuePair() {};
            
            IndexValuePair(Index<rank_>& idx, ValueType_* val)
            : index_(idx), value_(val) {
            };
            
            Index<rank_>& index() {
                return index_;
            }
            
            const Index<rank_>& index() const{
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
    }
}


#endif 