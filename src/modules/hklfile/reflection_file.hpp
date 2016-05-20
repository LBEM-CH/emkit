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

#ifndef HKLFILE_HPP
#define HKLFILE_HPP

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "reflection_data.hpp"

namespace em {

    namespace hklfile {

        class ReflectionFile {
        public:

            ReflectionFile(std::string file_name, std::string format)
            : file_name_(file_name) {
                if(format == ""){
                    std::cerr << "The format was not understood, using standard hkl\n";
                    format = "hkl";
                }
                rank_ = format.length();
                max_index_ = std::vector<int>(rank_, 0);
                min_index_ = std::vector<int>(rank_, 0);
            }
            
            bool load() {
                initialize();
                int data_cols = number_columns_ - rank_;
                if(data_cols < 1 ) {
                    std::cerr << "No data columns found!!\n";
                    return false;
                }
                
                data_.clear();
                max_index_ = std::vector<int>(rank_, 0);
                min_index_ = std::vector<int>(rank_, 0);
                
                std::ifstream ifs(file_name_);
                if (!ifs.is_open()) {
                    throw std::runtime_error("Unable to open file: '" + file_name_ + "' Are you sure the file exists?\n");
                }
                
                std::string sLine;
                for(int header_line=0; header_line<header_lines_; header_line++) std::getline(ifs, sLine);
                
                while(!ifs.eof()) {
                    std::vector<int> index;
                    for(int i=0; i< rank_; ++i) {
                        int val;
                        if(ifs >> val) {
                            index.push_back(val);
                            if(max_index_[i] < val) max_index_[i] = val;
                            if(min_index_[i] > val) min_index_[i] = val;
                        }
                        else break;
                    }
                    
                    std::vector<double> data;
                    for(int i=0; i< data_cols; ++i) {
                        double val;
                        if(ifs >> val) data.push_back(val);
                        else break;
                    }
                    
                    if(index.size() == rank_ && data.size() == data_cols) {
                        //std::cout << "Read: " << ReflectionData(index, data);
                        data_.push_back(ReflectionData(index, data));
                    }
                }
                       
                ifs.close();
                
                return true;
            }
            
            bool save() const {
                const int INT_WIDTH = 5;
                const int FLOAT_WIDTH = 13;
                const int FLOAT_PRECISION = 7;
                
                std::ofstream hklFile(file_name_);
                
                for(const auto& d: data_) {
                    if(d.index().size() != rank_) continue;
                    for(int i=0; i< rank_; ++i) hklFile << std::setw(INT_WIDTH) << d.index()[i] << " ";
                    for(const auto& c : d.data()) hklFile << std::setw(FLOAT_WIDTH) << std::setprecision(FLOAT_PRECISION) << c << " ";
                    hklFile << std::endl;
                }
                
                hklFile.close();
                
            }
            
            std::vector<ReflectionData>& data() {
                return data_;
            }

            const std::vector<ReflectionData>& data() const {
                return data_;
            }
            
            size_t rank() const {
                return rank_;
            }
            
            std::vector<int> max_index() const {
                return max_index_;
            }
            
            std::vector<int> min_index() const {
                return min_index_;
            }
            
            void clear() {
                data_.clear();
                min_index_.clear();
                max_index_.clear();
            }
            

        private:

            bool initialize() {
                std::ifstream infile(file_name_);

                std::string sLine;
                number_columns_ = 0;
                header_lines_ = -1;
                int cols_current = 0;
                int cols_previous = 0;

                if (infile.good()) {
                    //Check for the same number of columns in two consecutive rows
                    while (!infile.eof()) {
                        std::getline(infile, sLine);
                        std::stringstream is(sLine);

                        float temp;
                        cols_previous = cols_current;
                        cols_current = 0;
                        while (is >> temp) {
                            cols_current++;
                        }
                        if (cols_current == cols_previous) {
                            number_columns_ = cols_current;
                            break;
                        }
                        header_lines_++;
                    }
                }
                infile.close();
            }

            std::string file_name_;
            size_t rank_;
            int number_columns_ = 0;
            int header_lines_ = 0;
            std::vector<ReflectionData> data_;
            std::vector<int> max_index_;
            std::vector<int> min_index_;

        };
    }
}

#endif /* HKLFILE_HPP */

