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

#ifndef HKLDATA_HPP
#define HKLDATA_HPP

#include <iostream>
#include <vector>
#include <cmath>

#include "column_arranger.hpp"


namespace em {
    
    namespace hklfile {

        class ReflectionData {
        public:
            ReflectionData() = default;
            
            ReflectionData(int rank, int number_of_data_cols)
            : index_(std::vector<int>(rank, 0)), data_(std::vector<double>(number_of_data_cols, 0.0)) {
            };
            
            ReflectionData(std::vector<int> index, std::vector<double> data) 
            : index_(index), data_(data){
            }
            
            friend inline std::ostream& operator<<(std::ostream& os, const ReflectionData& obj){
                os << "(" ;
                for(auto d: obj.index_) os << d << " ";
                os << ") -> ";
                for(auto d: obj.data_) os << d << " ";
                os << std::endl;
                return os;
            }
            
            std::vector<int> index() const {
                return index_;
            }
            
            void set_index(std::vector<int> index) {
                index_ = index;
            }
            
            std::vector<double> data() const {
                return data_;
            }
            
            double amplitude(int id = -1) const {
                int data_cols_ = data_.size();
                if(id < 0) id = ColumnArranger::get_arrangement(data_cols_)->amplitude_col_id();
                if(id >=0 && id < data_cols_) return data_[id];
                else return 0.0;
            }
            
            void set_amplitude(double value, int id = -1) {
                int data_cols_ = data_.size();
                if(id < 0) id = ColumnArranger::get_arrangement(data_cols_)->amplitude_col_id();
                if(id >=0 && id < data_cols_) data_[id] = value;
            }
            
            double phase(int id = -1) const {
                int data_cols_ = data_.size();
                if(id < 0) id = ColumnArranger::get_arrangement(data_cols_)->phase_col_id();
                if(id >=0 && id < data_cols_) return data_[id] * (M_PI/180);
                else return 0.0;
            }
            
            void set_phase(double value, int id = -1) {
                int data_cols_ = data_.size();
                if(id < 0) id = ColumnArranger::get_arrangement(data_cols_)->phase_col_id();
                if(id >=0 && id < data_cols_) data_[id] = value * (180/M_PI);
            }
            
            double fom(int id = -1) const {
                int data_cols_ = data_.size();
                if(id < 0) id = ColumnArranger::get_arrangement(data_cols_)->fom_col_id();
                if(id >=0 && id < data_cols_) {
                    if(data_[id] > 1.0) return data_[id]/100;
                    else return data_[id];
                }
                else return 1.0;
            }
            
            void set_fom(double value, int id = -1) {
                int data_cols_ = data_.size();
                if(id < 0) id = ColumnArranger::get_arrangement(data_cols_)->fom_col_id();
                if(id >=0 && id < data_cols_) data_[id] = value;
            }
            
        private:
            std::vector<int> index_;
            std::vector<double> data_;
        };
                
        
    }
}

#endif /* HKLDATA_HPP */

