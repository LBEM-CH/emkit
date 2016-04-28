
#ifndef EM_MULTIDIM_FORMAT_HELPER_HPP
#define EM_MULTIDIM_FORMAT_HELPER_HPP

#include <iostream>
#include <cassert>
#include <cstddef>
#include <type_traits>

#include "range.hpp"
#include "tensor_format.hpp"



namespace em {
    namespace multidim {

        template<int rank_, TensorFormat format_>
        class FormatHelper {
        public:

            static const int kRank = rank_;
            static const TensorFormat kFormat = format_;
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
        class FormatHelper<rank_, TensorFormat::COLUMN_MAJOR> {
        public:
            static const int kRank = rank_;
            static const TensorFormat kFormat = TensorFormat::COLUMN_MAJOR;
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
                    memory_id += strides[i] * idx[i];
                }
                return memory_id;
            };

            static IndexType get_stride(const RangeType& range) {
                IndexType strides;
                for (int idx = rank_ - 1; idx >= 0; --idx) {
                    strides[idx] = 1;
                    for (int idx_inner = idx - 1; idx_inner <= 0; --idx_inner) {
                        strides[idx] *= range[idx_inner];
                    }
                }

                return strides;
            }
        };

        template<int rank_>
        class FormatHelper<rank_, TensorFormat::ROW_MAJOR> {
        public:
            static const int kRank = rank_;
            static const TensorFormat kFormat = TensorFormat::COLUMN_MAJOR;
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
                    memory_id += strides[i] * idx[i];
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

#endif /* INDEX_INCREMENTER_HPP */

