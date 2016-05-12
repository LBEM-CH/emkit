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

#ifndef MRC_DATA_HPP_PQVG38
#define MRC_DATA_HPP_PQVG38

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "format_specifier.hpp"
#include "byte_swapper.hpp"

namespace em {
    namespace mrc {

        /**
         * @brief           A class to handle the data of MRC type files.
         * @description     The raw data is stored as a vector of char. The
         *                  data can be requested/set in any of the C++ standard 
         *                  types and internally the char vector is first 
         *                  reinterpreted to the correct type using which was
         *                  expected from file and then finally to the requested 
         *                  type.
         * @author          Nikhil Biyani - nikhil(dot)biyani(at)gmail(dot)com
         */
        class Data {
        public:

            /**
             * @brief       Initialize the data specifying some underlying variables
             * @param   
             */
            Data(std::shared_ptr<FormatSpecifier> format = std::shared_ptr<FormatSpecifier>(new FormatSpecifier()))
            : format_(format), mode_(2) {
            };

            /**
             * @brief           Load the data from the file and swap endianness
             * @description     The file is 
             *                  read and the data is internally set. The read data
             *                  can be requested in any format using the other
             *                  functions. If it is marked that the endianess is to
             *                  be swapped then also does that!
             * @return          Success of read from the file
             */
            bool load(std::ifstream& is, size_t data_points, int mode, bool swap_endianness) {
                mode_ = mode;
                is.seekg(format_->data_offset(), is.beg);
                data_.clear();
                data_ = std::vector<char>(data_points * cell_size());
                is.read(data_.data(), data_points * cell_size());

                if (swap_endianness) {
                    for (int i = 0; i < data_points*block_size(); ++i) {
                        ByteSwapper::byte_swap(&data_[i * byte_size()], byte_size());
                    }
                }

                if (is) {
                    return true;
                } else {
                    return false;
                }
            };

            /**
             * @brief           Saves the data to the file
             * @description     The file (filename provided in the constructor) is 
             *                  saved with the data is internally set. The data
             *                  can be changed using other functions.
             * @return          Success of save to the file
             */
            bool save(std::ofstream& os) {

                os.seekp(format_->data_offset(), os.beg);
                os.write(data_.data(), data_.size());

                if (os) return true;
                else {
                    return false;
                }
            };
            
            void clear() {
                data_.clear();
            }

            /**
             * @brief           Request the vector of stored data
             * @description     The data is first converted to the inherit type 
             *                  using the size of byte for each data point indicated
             *                  in the constructor and then converted to the
             *                  requested type
             * @return          vector of the stored data
             */
            template<typename value_type>
            std::vector<value_type> get() const {
                size_t _points = data_points();
                if (byte_size() == 1) {
                    const int8_t* data = reinterpret_cast<const int8_t*> (data_.data());
                    return std::vector<value_type>(data, data + _points*block_size());
                } else if (byte_size() == 2) {
                    const int16_t* data = reinterpret_cast<const int16_t*> (data_.data());
                    return std::vector<value_type>(data, data + _points*block_size());
                } else if (byte_size() == 4) {
                    const float* data = reinterpret_cast<const float*> (data_.data());
                    return std::vector<value_type>(data, data + _points*block_size());
                } else {
                    std::cerr << "Unidentified byte size " << byte_size() << " encountered. Possible (1, 2, 4)\n";
                }
            }

            /**
             * @brief           Set the data from a vector.
             * @description     The vector is first converted to the inherent type
             *                  using the byte size provided in the constructor
             *                  and then finally stored as a char vector
             * @param       data    vector of the data
             */
            template<typename value_type>
            void set(const std::vector<value_type>& data, int mode) {
                mode_ = mode;
                data_.clear();
                size_t _points = data.size()/block_size();
                if (byte_size() == 1) {
                    auto corrected = std::vector<int8_t>(data.begin(), data.end());
                    data_ = std::vector<char>((char*) corrected.data(), (char*) corrected.data() + _points * cell_size());
                } else if (byte_size() == 2) {
                    auto corrected = std::vector<int16_t>(data.begin(), data.end());
                    data_ = std::vector<char>((char*) corrected.data(), (char*) corrected.data() + _points * cell_size());
                } else if (byte_size() == 4) {
                    auto corrected = std::vector<float>(data.begin(), data.end());
                    data_ = std::vector<char>((char*) corrected.data(), (char*) corrected.data() + _points * cell_size());
                } else {
                    std::cerr << "Unidentified byte size " << byte_size() << " encountered. Possible (1, 2, 4)\n";
                }
            }

            size_t data_points() const {
                return data_.size() / cell_size();
            }

            int mode() {
                return mode_;
            }

        private:

            int byte_size() const {
                return format_->data_byte_size(mode_);
            };
            
            int block_size() const {
                return format_->block_size(mode_);
            };
            
            int cell_size() const {
                return byte_size()*block_size();
            }

            int mode_;
            std::shared_ptr<FormatSpecifier> format_;
            std::vector<char> data_;
        };


    }
}

#endif

