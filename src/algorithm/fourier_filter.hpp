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

namespace em {
    
    namespace algorithm {
        
        enum class FilterAlgorithm {
            TOP_HAT,
            GAUSSIAN,
            BUTTERWORTH
        };
        
        template<typename ObjectType_, FilterAlgorithm algo_>
        struct FilteringImplementaiton {
            
        };
        
        template<typename ObjectType_>
        struct FilteringImplementation<ObjectType_, FilterAlgorithm::TOP_HAT> {
            
            void low_pass(ObjectType_& obj, double cut_off_freq) {
                for(auto& data : obj) {
                    if(resolution(data.index(), obj.cell_lengths(), obj.cell_angle()) >= cut_off_freq) {
                        obj[data.index()] = obj::object_traits<ObjectType_>::data_type();
                    }
                }
            }
            
            void high_pass(ObjectType_& obj, double cut_off_freq) {
                for(auto& data : obj) {
                    if(resolution(data.index(), obj.cell_lengths(), obj.cell_angle()) <= cut_off_freq) {
                        obj[data.index()] = obj::object_traits<ObjectType_>::data_type();
                    }
                }
            }
            
            void band_pass(ObjectType_& obj, double low_pass_freq, double high_pass_freq) {
                for(auto& data : obj) {
                    if(resolution(data.index(), obj.cell_lengths(), obj.cell_angle()) >= low_pass_freq || 
                       resolution(data.index(), obj.cell_lengths(), obj.cell_angle()) <= high_pass_freq) {
                        obj[data.index()] = obj::object_traits<ObjectType_>::data_type();
                    }
                }
            }
            
        };
        
        template<typename ObjectType_>
        struct FilteringImplementation<ObjectType_, FilterAlgorithm::BUTTERWORTH> {
            
            //TODO
            
        };
        
        template<typename ObjectType_>
        struct FilteringImplementation<ObjectType_, FilterAlgorithm::GAUSSIAN> {
            
            //TODO
            
        };
        
        
        
        
        template<typename ObjectType_>
        std::enable_if<object::is_complex_valued<typename object::object_traits<ObjectType_>::data_type>::value, void>::type
        low_pass(ObjectType_& object) {
            
        }
        
        
        
    }
}

#endif /* FOURIER_FILTER_HPP */

