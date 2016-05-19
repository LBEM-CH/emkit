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
            
            ReflectionData(std::vector<int> index, std::vector<double> data) 
            : index_(index), data_(data){
                rank_ = index.size();
                data_cols_ = data.size();
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
            
            std::vector<double> data() const {
                return data_;
            }
            
            double amplitude(int id = -1) const {
                if(id < 0) id = ColumnArranger::get_arrangement(data_cols_)->amplitude_col_id();
                if(id >=0 && id < data_cols_) return data_[id];
                else return 0.0;
            }
            
            double phase(int id = -1) const {
                if(id < 0) id = ColumnArranger::get_arrangement(data_cols_)->phase_col_id();
                if(id >=0 && id < data_cols_) return data_[id] * (M_PI/180);
                else return 0.0;
            }
            
            double fom(int id = -1) const {
                if(id < 0) id = ColumnArranger::get_arrangement(data_cols_)->fom_col_id();
                if(id >=0 && id < data_cols_) {
                    if(data_[id] > 1.0) return data_[id]/100;
                    else return data_[id];
                }
                else return 1.0;
            }
            
        private:
            std::vector<int> index_;
            std::vector<double> data_;
            size_t rank_;
            size_t data_cols_;
        };
                
        
    }
}

#endif /* HKLDATA_HPP */

