
#ifndef EM_MULTIDIM_STORAGE_ORDER_HPP
#define EM_MULTIDIM_STORAGE_ORDER_HPP

#include <iostream>
#include <cassert>
#include <cstddef>
#include <type_traits>

#include "range.hpp"

namespace em {
    namespace multidim {

        enum class StorageOrder {
            COLUMN_MAJOR,   //FORTRAN/MATLAB LIKE 
            ROW_MAJOR       //C++ LIKE
        };
        
        
        template<int rank_, StorageOrder format_>
        class StorageOrderArranger {
        public:

            static const int kRank = rank_;
            static const StorageOrder kFormat = format_;
            using IndexType = Index<rank_>;
            using RangeType = Range<rank_>;
            using MemoryIdType = typename RangeType::value_type;

        public:

            static IndexType map(const MemoryIdType& id, const RangeType& range) {
                return IndexType(0);
            };

            static MemoryIdType map(const IndexType& idx, const RangeType& range) {
                return 0;
            }

            static IndexType get_stride(const RangeType& range) {
                return IndexType(0);
            }


        };

        template<int rank_>
        class StorageOrderArranger<rank_, StorageOrder::COLUMN_MAJOR> {
        public:
            static const int kRank = rank_;
            static const StorageOrder kFormat = StorageOrder::COLUMN_MAJOR;
            using IndexType = Index<rank_>;
            using RangeType = Range<rank_>;
            using MemoryIdType = typename RangeType::value_type;

            static IndexType map(const MemoryIdType& id, const RangeType& range) {
                assert(range.size() > id);
                IndexType idx;
                MemoryIdType temp_id = id;
                IndexType strides = get_stride(range);
                for(int i=rank_-1; i>=0; --i){
                    idx[i] = temp_id/strides[i];
                    temp_id -= idx[i] * strides[i];
                }
                return idx;
            };

            static MemoryIdType map(const IndexType& idx, const RangeType& range) {
                assert(range.contains(idx));
                MemoryIdType memory_id = 0;
                IndexType strides = get_stride(range);
                for (int i = 0; i < rank_; ++i) {
                    //Get the positive index
                    int id_non_neg = (range[i] + idx[i])%range[i];
                    memory_id += strides[i] * id_non_neg;
                }
                return memory_id;
            };

            static IndexType get_stride(const RangeType& range) {
                IndexType strides;
                for (int idx = rank_ - 1; idx >= 0; --idx) {
                    strides[idx] = 1;
                    for (int idx_inner = idx - 1; idx_inner >= 0; --idx_inner) {
                        strides[idx] *= range[idx_inner];
                    }
                }

                return strides;
            }
        };

        template<int rank_>
        class StorageOrderArranger<rank_, StorageOrder::ROW_MAJOR> {
        public:
            static const int kRank = rank_;
            static const StorageOrder kFormat = StorageOrder::COLUMN_MAJOR;
            using IndexType = Index<rank_>;
            using RangeType = Range<rank_>;
            using MemoryIdType = typename RangeType::value_type;

            static IndexType map(const MemoryIdType& id, const RangeType& range) {
                assert(range.size() > id);
                IndexType idx;
                MemoryIdType temp_id = id;
                IndexType strides = get_stride(range);
                for(int i=0; i<rank_; ++i){
                    idx[i] = temp_id/strides[i];
                    temp_id -= idx[i] * strides[i];
                }
                return idx;
            };

            static MemoryIdType map(const IndexType& idx, const RangeType& range) {
                assert(range.contains(idx));
                MemoryIdType memory_id = 0;
                IndexType strides = get_stride(range);
                for (int i = 0; i < rank_; ++i) {
                    //Get the positive index
                    int id_non_neg = (range[i] + idx[i])%range[i];
                    memory_id += strides[i] * id_non_neg;
                }
                return memory_id;
            };

            static IndexType get_stride(const RangeType& range) {
                IndexType strides;

                for (int idx = 0; idx < rank_; ++idx) {
                    strides[idx] = 1;
                    for (int idx_inner = idx + 1; idx_inner < rank_; ++idx_inner) {
                        strides[idx] *= range[idx_inner];
                    }
                }

                return strides;
            }
        };
    }
}

#endif 

