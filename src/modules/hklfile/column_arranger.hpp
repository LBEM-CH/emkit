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

#ifndef COLUMN_ARRANGER_HPP
#define COLUMN_ARRANGER_HPP

#include <memory>

namespace em {
    
    namespace hklfile {
        
        class ColumnArrangement {
        public:    
            virtual int amplitude_col_id() const { return -1; };
            virtual int phase_col_id() const { return -1; };
            virtual int fom_col_id() const { return -1; };
        };
        
        struct TwoColumnArrangement : public ColumnArrangement {
        public:
            int amplitude_col_id() const override { return 0; };
            int phase_col_id() const override { return 1; };
        };
        
        struct ThreeColumnArrangement : public ColumnArrangement {
        public:
            int amplitude_col_id() const override { return 0; };
            int phase_col_id() const override { return 1; };
            int fom_col_id() const override { return 2; };
        };
        
        class ColumnArranger {
        public:
            static std::unique_ptr<ColumnArrangement> get_arrangement(int number_of_data_cols){
                if(number_of_data_cols == 2) return std::unique_ptr<ColumnArrangement>(new TwoColumnArrangement);
                else if(number_of_data_cols == 3) return std::unique_ptr<ColumnArrangement>(new ThreeColumnArrangement);
                else return std::unique_ptr<ColumnArrangement>(new ColumnArrangement);
            }
        };
        
        
    }
}


#endif /* COLUMN_ARRANGER_HPP */

