/* 
 * This file is a part of emkit.
 * 
 * emkit is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or any 
 * later version.
 * 
 * emkit is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public 
 * License for more details <http://www.gnu.org/licenses/>
 * 
 * Author:
 * Nikhil Biyani: nikhil(dot)biyani(at)gmail(dot)com
 * 
 */

#ifndef HKL_FILE_ADAPTOR_HPP
#define HKL_FILE_ADAPTOR_HPP

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <type_traits>

#include "../elements/complex.hpp"
#include "../elements/tensor.hpp"
#include "../elements/string.hpp"
#include "../elements/table.hpp"

namespace em {

    namespace fileio {

        class ReflectionFile {
        public:

            ReflectionFile(std::string file_name = "", std::string format = "", int rank = 0)
            : file_name_(file_name), format_(format), rank_(rank) {
                if (format == "") {
                    format_ = element::File(file_name).extension();
                }

                if (rank == 0) rank_ = format_.length();
            };

            template<typename ObjectType_>
            typename std::enable_if<!object::is_complex_valued<typename object::object_traits<ObjectType_>::data_type>::value, bool>::type
            load(ObjectType_& obj) {
                std::cerr << "Error! HKL type files can read only complex valued objects\n";
                return false;
            }

            template<typename ObjectType_>
            typename std::enable_if<object::is_complex_valued<typename object::object_traits<ObjectType_>::data_type>::value, bool>::type
            load(ObjectType_& obj) {

                using object_type = ObjectType_;
                static const size_t rank = object::object_traits<object_type>::rank;
                using index_type = typename object::object_traits<object_type>::index_type;
                using data_type = typename object::object_traits<object_type>::data_type;
                using value_type = typename data_type::value_type;

                static_assert((rank == 2 || rank == 3), "To read a HKL type file, the rank of the object should be either 2 or 3 ");

                if (rank_ != rank) {
                    std::cerr << "ERROR! Rank deduced from file format and the requested rank do not match!\n";
                    return false;
                }

                //Reassign the size if not set
                if (obj.range() == index_type(0)) {
                    std::cerr << "ERROR! The size of the target object to be read from file '" << file_name_ << "'is not set\n";
                    exit(1);
                }
                
                index_type origin = obj.range()*0.5;
                origin[0] = 0;
                obj.transform_origin(origin);

                int number_cols = guess_number_of_cols();
                element::Table table = element::Table::read_table(file_name_, number_cols);
                
                for (int row_num = 0; row_num < table.rows(); row_num++) {
                    value_type amp = table.get<value_type>(row_num, ColumnArranger::get_arrangement(number_cols-rank)->amplitude_col_id()+rank);
                    value_type phase = table.get<value_type>(row_num, ColumnArranger::get_arrangement(number_cols-rank)->phase_col_id()+rank);
                    
                    //convert phase to radians
                    phase = phase*(M_PI/180);
                    
                    index_type idx;
                    for (int i = 0; i < rank; ++i) idx[i] = table.get<typename index_type::value_type>(row_num, i);

                    //We store only positive half of the complex space, 
                    //for negative h, get Friedel spot
                    if (idx[0] < 0) {
                        idx = idx * -1;
                        phase = phase * -1;
                    }

                    element::Complex<value_type> value;
                    value.set_amplitude(amp);
                    value.set_phase(phase);
                    //std::cout << "Setting: " << idx << " -> " << value << std::endl;
                    obj[idx] = value;
                }

                return true;
            }

            template<typename ObjectType_>
            typename std::enable_if<!object::is_complex_valued<typename object::object_traits<ObjectType_>::data_type>::value, bool>::type
            save(const ObjectType_& obj) {
                std::cerr << "Error! HKL type files can be used to write only complex valued objects\n";
                return false;
            }

            template<typename ObjectType_>
            typename std::enable_if<object::is_complex_valued<typename object::object_traits<ObjectType_>::data_type>::value, bool>::type
            save(const ObjectType_& obj) {

                using object_type = ObjectType_;
                static const size_t rank = object::object_traits<object_type>::rank;
                using index_type = typename object::object_traits<object_type>::index_type;
                using data_type = typename object::object_traits<object_type>::data_type;

                static_assert((rank == 2 || rank == 3), "To read a HKL type file, the rank of the object should be either 2 or 3 ");

                if (rank != rank_) {
                    std::cerr << "ERROR! Rank deduced from file format and the requested rank do not match!\n";
                    return false;
                }

                index_type origin = obj.range()*0.5;
                origin[0] = 0;
                element::Tensor<data_type, rank, element::StorageOrder::ROW_MAJOR> to_be_written(obj.range(), origin);
                convert(obj, to_be_written);

                element::Table table(rank+3);
                
                //Dump the data to table
                for (const auto& d : to_be_written) {
                    if (d.value().amplitude() > 0.000001) {
                        std::vector<std::string> row_str = std::vector<std::string>(table.columns());
                        for (int i = 0; i < rank; ++i) row_str[i] = std::to_string(d.index()[i]);
                        
                        row_str[rank] = std::to_string(d.value().amplitude());
                        row_str[rank+1] = std::to_string(d.value().phase()*(180/M_PI));
                        row_str[rank+2] = "100";

                        table.append_row(row_str);
                    }
                }

                table.write_table(file_name_);

            }

            const std::string& file_name() const {
                return file_name_;
            }

            std::string& file_name() {
                return file_name_;
            }

        private:

            class ColumnArrangement {
            public:

                virtual int amplitude_col_id() const {
                    return -1;
                };

                virtual int phase_col_id() const {
                    return -1;
                };

                virtual int fom_col_id() const {
                    return -1;
                };
            };

            struct TwoColumnArrangement : public ColumnArrangement {
            public:

                int amplitude_col_id() const override {
                    return 0;
                };

                int phase_col_id() const override {
                    return 1;
                };
            };

            struct ThreeColumnArrangement : public ColumnArrangement {
            public:

                int amplitude_col_id() const override {
                    return 0;
                };

                int phase_col_id() const override {
                    return 1;
                };

                int fom_col_id() const override {
                    return 2;
                };
            };

            class ColumnArranger {
            public:

                static std::unique_ptr<ColumnArrangement> get_arrangement(int number_of_data_cols) {
                    if (number_of_data_cols == 2) return std::unique_ptr<ColumnArrangement>(new TwoColumnArrangement);
                    else if (number_of_data_cols == 3) return std::unique_ptr<ColumnArrangement>(new ThreeColumnArrangement);
                    else return std::unique_ptr<ColumnArrangement>(new ColumnArrangement);
                }
            };

            int guess_number_of_cols() {
                InputFile file(file_name_);
                int cols_curr = element::String(file.read_line()).trim().split(' ').size();
                int cols_pre = 0;
                while (cols_curr != cols_pre && !file.eof()) {
                    cols_pre = cols_curr;
                    cols_curr = element::String(file.read_line()).trim().split(' ').size();
                }

                return cols_curr;
            }

            std::string file_name_;
            std::string format_;
            int rank_;

        };
    }
}

#endif