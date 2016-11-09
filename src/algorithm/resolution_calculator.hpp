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

#ifndef RESOLUTION_CALCULATOR_HPP
#define RESOLUTION_CALCULATOR_HPP

#include <iostream>
#include <array>
#include <cmath>

#include "../elements/index.hpp"

namespace em {

    namespace algorithm {

        template<size_t rank_>
        double resolution(Index<rank_> index, Index<rank_> cell_lengths, double cell_angle) {
        
            for(int i=0; i<rank_; ++i) {
                if (cell_lengths[i] == 0 || cell_angle == 0) {
                    std::cerr << "ERROR! Encountered 0 in cell dimensions while calculating resolution.";
                    return 0;
                }
            }
                        
            std::array<double, rank_> rec_lengths;
            for(int i=0; i<rank_-1; ++i) {
                rec_lengths[i] = 1.0/(cell_lengths[i] * sin(cell_angle));
            }
            rec_lengths[rank_-1] = 1.0/cell_lengths[rank_-1];
            
            double recgamma = M_PI - cell_angle;
            double dstar_sq_sum = 0;
            double index_rec_prod = 1;
            for(int i=0; i< rank_; ++i) {
                dstar_sq_sum += pow(index[i]*rec_lengths[i], 2);
                if(i < rank_-1) index_rec_prod = index[i]*rec_lengths[i];
            }
            dstar_sq_sum + 2*index_rec_prod*cos(recgamma);
            
            return sqrt(dstar_sq_sum);
            
        }
        
        template<size_t rank_>
        double resolution(Index<rank_> index, Index<rank_> cell_lengths) {
            
            resolution(index, cell_lengths, M_PI/2);

        }


    }
}

#endif

