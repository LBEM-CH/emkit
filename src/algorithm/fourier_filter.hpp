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

#ifndef FOURIER_FILTER_HPP
#define FOURIER_FILTER_HPP

#include <type_traits>

#include "../objects/object_base_types.hpp"
#include "resolution_calculator.hpp"

namespace em {
    
    namespace algorithm {
        
        enum class FilterAlgorithm {
            TOP_HAT,
            GAUSSIAN,
            BUTTERWORTH
        };
        
        template<typename ObjectType_, FilterAlgorithm algo_>
        struct filtering_impl {
            static void low_pass(ObjectType_& obj, double cut_off_freq) {
                std::cerr << "ERROR: The filtering algorithm could not be recognized.\n\n";
            }
            
            static void high_pass(ObjectType_& obj, double cut_off_freq) {
                std::cerr << "ERROR: The filtering algorithm could not be recognized.\n\n";
            }
            
            static void band_pass(ObjectType_& obj, double low_pass_freq, double high_pass_freq) {
                std::cerr << "ERROR: The filtering algorithm could not be recognized.\n\n";
            }
        };
        
        template<typename ObjectType_>
        struct filtering_impl<ObjectType_, FilterAlgorithm::TOP_HAT> {
            
            static void low_pass(ObjectType_& obj, double cut_off_freq) {
                for(auto& data : obj) {
                    if(resolution(data.index(), obj.cell_lengths()) >= cut_off_freq) {
                        using data_type = typename object::object_traits<ObjectType_>::data_type;
                        obj[data.index()] = data_type();
                    }
                }
            }
            
            static void high_pass(ObjectType_& obj, double cut_off_freq) {
                for(auto& data : obj) {
                    if(resolution(data.index(), obj.cell_lengths()) <= cut_off_freq) {
                        using data_type = typename object::object_traits<ObjectType_>::data_type;
                        obj[data.index()] = data_type();
                    }
                }
            }
            
            static void band_pass(ObjectType_& obj, double low_pass_freq, double high_pass_freq) {
                for(auto& data : obj) {
                    if(resolution(data.index(), obj.cell_lengths()) >= low_pass_freq || 
                       resolution(data.index(), obj.cell_lengths()) <= high_pass_freq) {
                        using data_type = typename object::object_traits<ObjectType_>::data_type;
                        obj[data.index()] = data_type();
                    }
                }
            }
            
        };
        
        template<typename ObjectType_>
        struct filtering_impl<ObjectType_, FilterAlgorithm::BUTTERWORTH> {
            static void low_pass(ObjectType_& obj, double cut_off_freq) {
                //TODO
            }
            
            static void high_pass(ObjectType_& obj, double cut_off_freq) {
                //TODO
            }
            
            static void band_pass(ObjectType_& obj, double low_pass_freq, double high_pass_freq) {
                //TODO
            }
            
        };
        
        template<typename ObjectType_>
        struct filtering_impl<ObjectType_, FilterAlgorithm::GAUSSIAN> {
            static void low_pass(ObjectType_& obj, double cut_off_freq) {
                //TODO
            }
            
            static void high_pass(ObjectType_& obj, double cut_off_freq) {
                //TODO
            }
            
            static void band_pass(ObjectType_& obj, double low_pass_freq, double high_pass_freq) {
                //TODO
            }
            
        };
        
        template<typename ObjectType_>
        typename std::enable_if<object::is_complex_valued<typename object::object_traits<ObjectType_>::data_type>::value, void>::type
        low_pass(ObjectType_& obj, double cut_off_freq) {
            return filtering_impl<ObjectType_, FilterAlgorithm::TOP_HAT>::low_pass(obj, cut_off_freq);
        }
        
        
        
    }
}

#endif /* FOURIER_FILTER_HPP */

