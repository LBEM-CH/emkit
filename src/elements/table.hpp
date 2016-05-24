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

#ifndef TABLE_HPP
#define TABLE_HPP

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>
#include <algorithm>

#include "string.hpp"
#include "file.hpp"

namespace em {

    namespace element {

        class Table {
        public:

            typedef typename std::vector<std::vector<std::string>> table_type;
            typedef typename std::vector<std::string> row_type;

            Table()
            : data_(table_type()), column_max_widths_(std::vector<int>()) {
            }

            Table(int columns)
            : columns_(columns), data_(table_type()), 
              column_max_widths_(std::vector<int>(columns, 0)) {
            }

            Table(int rows, int columns)
            : columns_(columns), data_(table_type(rows, row_type(columns))), 
              column_max_widths_(std::vector<int>(columns, 0)) {
            }

            Table(const Table& other) = default;

            Table& operator=(const Table& right) = default;

            static Table read_table(std::string file_name, int number_of_cols, 
                                    char delimiter = ' ', char line_comment_char='#') {
                InputFile ifs(file_name);
                Table table(number_of_cols);
    
                while(!ifs.eof()) {
                    std::string line = ifs.read_line();
                    std::string line_trimmed = String::trim(line);
                    if(line[0] != line_comment_char) {
                        table.append_row(String::split(line_trimmed, delimiter));
                    }
                }
                ifs.close();
                return table;
            }
            
            void write_table(std::string file_name, std::vector<int> column_widths = std::vector<int>()) {
                if(column_widths.size() != 0) set_column_widths(column_widths);
                std::ofstream ofs(file_name);
                ofs << *this;
                ofs.close();
            } 
            
            template<typename ValueType_>
            typename std::enable_if<std::is_fundamental<ValueType_>::value, ValueType_>::type
            get(int row_number, int column_number) const {
                assert(row_number < data_.size());
                assert(column_number < columns_);
                return String(data_[row_number][column_number]).number<ValueType_>();
            }
            
            std::string get(int row_number, int column_number) const {
                assert(row_number < data_.size());
                assert(column_number < columns_);
                return data_[row_number][column_number];
            }
            
            template<typename ValueType_>
            typename std::enable_if<std::is_fundamental<ValueType_>::value, void>::type
            set(int row_number, int column_number, ValueType_ value) {
                assert(row_number < data_.size());
                assert(column_number < columns_);
                std::string str = std::to_string(value);
                data_[row_number][column_number] = str;
                reset_column_max_width(column_number, str);
            }
            
            void set(int row_number, int column_number, std::string value) {
                assert(row_number < data_.size());
                assert(column_number < columns_);
                data_[row_number][column_number] = value;
                reset_column_max_width(column_number, value);
            }

            template<typename ValueType_>
            typename std::enable_if<std::is_fundamental<ValueType_>::value, std::vector<ValueType_>>::type
            get_row(int row_number) const {
                std::vector<ValueType_> row;
                for (const auto& str : data_.at(row_number)) row.push_back(String(str).number<ValueType_>());
                return row;
            }

            row_type get_row(int row_number) const {
                return data_.at(row_number);
            }

            template<typename ValueType_>
            typename std::enable_if<std::is_fundamental<ValueType_>::value, void>::type
            set_row(int row_number, const std::vector<ValueType_>& row) {
                if (row.size() != columns_) {
                    std::cerr << "WARNING: Setting a row in Table, encountered " << row.size()
                            << " columns (expected: " << columns_ << "). Will trim/extend!!\n";
                }

                int column_number = 0;
                for (auto& str : data_.at(row_number)) {
                    if (column_number < row.size()) str = std::to_string(row.at(column_number));
                    reset_column_max_width(column_number, str);
                    ++column_number;
                }
            }

            void set_row(int row_number, std::vector<std::string> row) {
                if (row.size() != columns_) {
                    std::cerr << "WARNING: Setting a row in Table, encountered " << row.size()
                            << " columns (expected: " << columns_ << "). Will trim/extend!!\n";
                }

                int column_number = 0;
                for (auto& str : data_.at(row_number)) {
                    if (column_number < row.size()) {
                        str = row.at(column_number);
                        reset_column_max_width(column_number, str);
                        column_number++;
                    }
                }
            }

            template<typename ValueType_>
            typename std::enable_if<std::is_fundamental<ValueType_>::value, std::vector<ValueType_>>::type
            get_column(int column_number) const {
                std::vector<ValueType_> col;
                for (const auto& row : data_) {
                    col.push_back(String(row.at(column_number)).number<ValueType_>());
                }
                return col;
            }

            row_type get_column(int column_number) const {
                std::vector<std::string> col;
                for (const auto& row : data_) {
                    col.push_back(row.at(column_number));
                }
                return col;
            }

            template<typename ValueType_>
            typename std::enable_if<std::is_fundamental<ValueType_>::value, void>::type
            set_column(int column_number, std::vector<ValueType_> column) {
                if (column.size() != data_.size()) {
                    std::cerr << "WARNING: Setting a column in Table, encountered " << column.size()
                            << " rows (expected: " << data_.size() << "). Will trim/extend!!\n";
                }

                int row_number = 0;
                for (auto& row_str : data_) {
                    if (row_number < column.size())  {
                        std::string str = std::to_string(column.at(row_number));
                        row_str.at(column_number) = str;
                        reset_column_max_width(column_number, str);
                        row_number++;
                    }
                }
            }

            void set_column(int column_number, std::vector<std::string> column) {
                if (column.size() != data_.size()) {
                    std::cerr << "WARNING: Setting a column in Table, encountered " << column.size()
                            << " rows (expected: " << data_.size() << "). Will trim/extend!!\n";
                }

                int i = 0;
                for (auto& row_str : data_) {
                    if (i < column.size()) {
                        std::string str = column.at(i++);
                        row_str.at(column_number) = str;
                        reset_column_max_width(column_number, str);
                    }
                }
            }

            template<typename ValueType_>
            typename std::enable_if<std::is_fundamental<ValueType_>::value, void>::type
            append_row(const std::vector<ValueType_>& row) {
                if(row.size() == 0) return;
                if (row.size() != columns_) {
                    std::cerr << "WARNING: Setting a row in Table, encountered " << row.size()
                            << " columns (expected: " << columns_ << "). Will trim/extend!!\n";
                }

                int column_number = 0;
                std::vector<std::string> row_to_be_set = std::vector<std::string>(columns_, "");
                for (auto& str : row_to_be_set) {
                    if (column_number < row.size()) {
                        str = std::to_string(row.at(column_number));
                        reset_column_max_width(column_number, str);
                        column_number++;
                    }
                }
                data_.push_back(row_to_be_set);
            }

            void append_row(std::vector<std::string> row) {
                if(row.size() == 0) return;
                if (row.size() != columns_) {
                    std::cerr << "WARNING: Setting a row in Table, encountered " << row.size()
                            << " columns (expected: " << columns_ << "). Will trim/extend!!\n";
                }

                int column_number = 0;
                std::vector<std::string> row_to_be_set = std::vector<std::string>(columns_, "");
                for (auto& str : row_to_be_set) {
                    if (column_number < row.size()) {
                        str = row.at(column_number);
                        reset_column_max_width(column_number, str);
                        column_number++;
                    }
                }
                data_.push_back(row_to_be_set);
            }

            template<typename ValueType_>
            typename std::enable_if<std::is_fundamental<ValueType_>::value, void>::type
            sort(int column_number) {
                if (column_number >= columns_) {
                    std::cerr << "WARNING: The column number " << column_number
                            << " for sorting exceeds the total number of columns ("
                            << columns_ << ") NOT SORTING!!\n";
                    return;
                }

                std::sort(data_.begin(), data_.end(),
                        [&column_number] (row_type i1, row_type i2) {
                            return String(i1[column_number]).number<ValueType_>() < String(i2[column_number]).number<ValueType_>();
                        });

            }
            
            template<typename ValueType_>
            typename std::enable_if<!std::is_fundamental<ValueType_>::value, void>::type
            sort(int column_number) {
                std::cerr << "WARNING: Sorting would not work, as the type used is not fundamental\n";
            }

            int rows() const {
                return data_.size();
            }

            int columns() const {
                return columns_;
            }

            void clear() {
                data_.clear();
            }

            friend inline std::ostream& operator<<(std::ostream& os, const Table& obj) {
                for (const auto& row : obj.data_) {
                    int col_number = 0;
                    for (const auto& col : row) {
                        os << std::setw(obj.column_max_widths_[col_number]) << col << " ";
                        col_number++;
                    }
                    os << "\n";
                }
                return os;
            }
            
            void set_column_widths(std::vector<int> widths) {
                if(widths.size() != columns_) {
                    std::cerr << "WARNING: Setting column widths of table, encountered " << widths.size()
                            << " columns (expected: " << columns_ << "). NOT CHANGING\n";
                    return;
                }
                
                column_max_widths_ = widths;
            }


        private:
            
            void reset_column_max_width(int col, std::string str){
                column_max_widths_[col] = std::max(column_max_widths_[col], (int) str.length());
            }
            
            table_type data_;
            int columns_;
            std::vector<int> column_max_widths_;

        };



    }
}


#endif /* TABLE_HPP */

