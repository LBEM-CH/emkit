
#ifndef TENSOR_TRAITS_HPP
#define TENSOR_TRAITS_HPP

namespace em {
    namespace multidim {

        template<typename TensorType_>
        class tensor_traits {
        public:

            //TODO: Check if the templated type is really a tensor?
            static const int rank = TensorType_::rank;
            typedef typename TensorType_::index_type index_type;
            typedef typename TensorType_::range_type range_type;
            typedef typename TensorType_::format_helper_type format_helper_type;
            typedef typename TensorType_::data_container_type data_container_type;
            typedef typename TensorType_::value_type value_type;
            typedef typename TensorType_::iterator iterator;
            typedef typename TensorType_::const_iterator const_iterator;
            typedef typename TensorType_::reference reference;
            typedef typename TensorType_::const_reference const_reference;
            typedef typename TensorType_::pointer pointer;
            typedef typename TensorType_::difference_type difference_type;
            typedef typename TensorType_::size_type size_type;

        };
    }
}

#endif /* TENSOR_TRAITS_HPP */

