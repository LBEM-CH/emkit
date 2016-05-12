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

#ifndef MRC_FILE_ADAPTOR_HPP
#define MRC_FILE_ADAPTOR_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

#include "../modules/mrcfile/image.hpp"
#include "../elements/file.hpp"
#include "../algorithm/fourier_transform.hpp"
#include "object.hpp"
#include "complex_object.hpp"
#include "real_object.hpp"

namespace em {

    namespace object {

        template<typename ValueType_>
        class MRCObject : public Object<ValueType_, 3> {
        
        public:
            
            using base_type = Object<ValueType_, 3>;
            using container_type = typename base_type::container_type;
            using range_type = typename container_type::range_type;
            
            
            MRCObject(std::string file_name="", std::string format = "") 
            : base_type(), file_name_(file_name), format_(format) {
            };
            
            MRCObject(const range_type& range, std::string file_name="", std::string format = "") 
            : file_name_(file_name), format_(format), base_type(range) {
            }
            
            MRCObject(const container_type& container, std::string file_name="", std::string format = "")
            : file_name_(file_name), format_(format), base_type(container){
            }
            
            MRCObject(const base_type& object, std::string file_name="", std::string format="")
            : file_name_(file_name), format_(format), base_type(object){
            }

            void load() {
                
                if(!element::File(file_name()).exists()) {
                    std::cerr << "ERROR! File not found: " << file_name() << std::endl;
                    exit(1);
                }
                
                mrc::Image mrc_image_ = get_mrc_image();
                mrc_image_.load();
                int mode = mrc_image_.data().mode();
                int columns = std::stoi(mrc_image_.header().get("columns"));
                int rows = std::stoi(mrc_image_.header().get("rows"));
                int sections = std::stoi(mrc_image_.header().get("sections"));

                if (mode == 0 || mode == 1 || mode == 2) {
                    this->container() = container_type({columns, rows, sections}, mrc_image_.data().get<ValueType_>());
                    
                } else if(mode == 3 || mode == 4 ) {
                    /*
                     * THIS PART IS NOT TESTED
                     */
                    std::cerr << "\n\n\nWARNING: Reading mode 3/4 from MRC files is not tested. Use at your own risk.\n\n\n";
                    std::vector<ValueType_> raw_data = mrc_image_.data().get<ValueType_>();
                    
                    //Develop a complex container
                    ComplexObject<ValueType_, 3> complex_object({columns*2, rows, sections}, {0, rows/2, sections/2});
                    int i=0;
                    for(auto& pair:complex_object) {
                        complex_object[pair.index()] = typename ComplexObject<ValueType_, 3>::value_type(raw_data[i++], raw_data[i++]);
                    }
                    complex_object.container().transform_origin({0,0,0});
                    
                    //FFT
                    RealObject<ValueType_, 3> real_object;
                    algorithm::fourier_transform(complex_object, real_object);
                    this->container() = container_type(real_object.container());

                } else {
                    std::cerr << "ERROR while reading MRC file:\n"
                            << "The data format (MRC mode:" << mode << ") not supported!\n"
                            << "HINT:\n Only MRC mode 0-4 is supported.\n";
                    exit(1);
                }

                this->properties().clear();
                for (auto prop : mrc_image_.header().get_all()) {
                    this->properties().register_property(prop.first, prop.second);
                }
                mrc_image_.clear();
            };

            void save() const {
                
                //Check for the existence of the file
                if(element::File(file_name()).exists())
                {
                    std::cout << "WARNING: File.. " << file_name() << " already exists. Overwriting!\n";
                }
                
                mrc::Image mrc_image_ = get_mrc_image();
                
                //Copy the current properties
                for (const auto& prop : this->properties()) {
                    mrc_image_.header().set(prop.first, prop.second);
                }

                //Change to the current sizes
                mrc_image_.header().set("columns", std::to_string(this->container().range()[0]));
                mrc_image_.header().set("rows", std::to_string(this->container().range()[1]));
                mrc_image_.header().set("sections", std::to_string(this->container().range()[2]));

                //Copy the data
                mrc_image_.data().set(this->container().vectorize(), 2);

                //Save to the file
                mrc_image_.save();

            };
            
            const std::string& file_name() const {
                return file_name_;
            }
            
            std::string& file_name() {
                return file_name_;
            }

        protected:
            
            virtual mrc::Image get_mrc_image() const {
                if(file_name() == "") {
                    std::cerr << "ERROR: File name is not set.\n";
                    exit(1);
                }
                std::string format = format_;
                if(format == "") {
                    format = element::File(file_name()).extension();
                }
                return mrc::Image(file_name(), format);
            }

            std::string file_name_;
            std::string format_;
        };
    }
}

#endif