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
#include <fstream>

#include "mrcfile/image.hpp"
#include "../components/real_object.hpp"

namespace em {

    class MRCFileAdaptor{
    public:

        MRCFileAdaptor(std::string fileName)
        : _mrc_image(fileName, "mrc"){
        };
        
        template<typename ValueType_>
        void load_object(RealObject<ValueType_, 3>& object) {
            
            typedef typename RealObject<ValueType_, 3>::base_type::container_type container_type;
            
            _mrc_image.load();
            int mode = _mrc_image.data().mode();
            int columns = std::stoi(_mrc_image.header().get("columns"));
            int rows = std::stoi(_mrc_image.header().get("rows"));
            int sections = std::stoi(_mrc_image.header().get("sections"));
            
            if(mode == 0 || mode == 1 || mode == 2) {
                object.container() = container_type({columns, rows, sections}, _mrc_image.data().get<ValueType_>());
            }
            else {
                std::cerr << "ERROR while reading MRC file:\n"
                        << "The data format (MRC mode:" << mode << ") not supported!\n"
                        << "HINT:\n Only MRC mode 0-4 is supported.\n";
                exit(1);
            }
            
            //TODO implement mode 3 and 4
            // Specifically first create a complex object and fft to real object
            
            object.properties().clear();
            for(auto prop : _mrc_image.header().get_all()) {
                object.properties().register_property(prop.first, prop.second);
            }
        };
        
        template<typename ValueType_>
        void save_object(const RealObject<ValueType_, 3>& object) {
            
            //Copy the current properties
            for(const auto& prop : object.properties()) {
                _mrc_image.header().set(prop.first, prop.second);
            }
            
            //Change to the current sizes
            _mrc_image.header().set("columns", std::to_string(object.container().range()[0]));
            _mrc_image.header().set("rows", std::to_string(object.container().range()[1]));
            _mrc_image.header().set("sections", std::to_string(object.container().range()[2]));
            _mrc_image.header().set("mode", "2");
            
            //Copy the data
            _mrc_image.data().set(object.container().vectorize(), 2);
            
            //Save to the file
            _mrc_image.save();
            
        };

    private:
        
        mrc::Image _mrc_image;

    };
}

#endif