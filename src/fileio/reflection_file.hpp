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

#include "../elements/complex.hpp"
#include "../modules/hklfile/reflection_file.hpp"

namespace em {

    namespace fileio {

        class ReflectionFile {
        public:

            ReflectionFile(std::string file_name = "", std::string format = "")
            : file_name_(file_name), format_(format) {
            };
            
            template<typename ObjectType_>
            bool load(ObjectType_& object) {
                
                using object_type = ObjectType_;
                static const size_t rank_ = object::object_traits<ObjectType_>::rank;
                using index_type = typename object::object_traits<ObjectType_>::index_type;
                using data_type = typename object::object_traits<ObjectType_>::data_type;
                
                static_assert(object::is_complex_valued<data_type>::value, "HKL type files can read only complex valued objects\n");
                static_assert((rank_ == 2 || rank_ == 3), "To read a HKL type file, the rank of the object should be either 2 or 3 ");
                
                hklfile::ReflectionFile file = get_file_impl();
                if(file.rank() != rank_) {
                    std::cerr <<  "ERROR! Rank deduced from file format and the requested rank do not match!\n";
                    return false;
                }
                
                if(!file.load()) {
                    std::cerr <<  "ERROR! There were errors in reading the file!\n";
                    return false;
                }
                
                //Reassign the size if not set
                if(object.range() == index_type(0)) {
                    index_type rnge;
                    std::vector<int> max_idx = file.max_index();
                    std::vector<int> min_idx = file.min_index();
                    for(int i=0; i<rank_; ++i) rnge[i] = 2*std::max(std::abs(max_idx[i]), std::abs(min_idx[i]))+1;
                    std::cout << "WARNING: Resizing object to: " << rnge << std::endl;
                    object.resize(rnge);
                }
                
                for(const auto& d : file.data()) {
                    double amp = d.amplitude();
                    double phase = d.phase();
                    std::vector<int> index = d.index();
                    index_type idx;
                    for(int i=0; i< rank_; ++i)  idx[i] = index[i];
                    
                    //We store only positive half of the complex space, 
                    //for negative h, get Friedel spot
                    if(idx[0] < 0) {
                        idx = idx * -1;
                        phase = phase * -1;
                    }
                    
                    element::Complex<typename data_type::value_type> value;
                    value.set_amplitude((typename data_type::value_type) amp);
                    value.set_phase((typename data_type::value_type) phase);
                    //std::cout << "Setting: " << idx << " -> " << value << std::endl;
                    object[idx] = value;
                }
                
                file.clear();
                return true;
            }
            
            const std::string& file_name() const {
                return file_name_;
            }

            std::string& file_name() {
                return file_name_;
            }
            
        private:
            
            hklfile::ReflectionFile get_file_impl() const {
                if (file_name() == "") {
                    std::cerr << "ERROR: File name is not set.\n";
                    exit(1);
                }
                std::string format = format_;
                if (format == "") {
                    format = element::File(file_name()).extension();
                }
                return hklfile::ReflectionFile(file_name(), format);
            }
            
            std::string file_name_;
            std::string format_;

        };
    }
}

#endif