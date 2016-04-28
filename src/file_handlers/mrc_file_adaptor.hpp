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
            load_image();
            int mode = _mrc_image.data().mode();
            int columns = (int) _mrc_image.header().get("columns");
            int rows = (int) _mrc_image.header().get("rows");
            int sections = (int) _mrc_image.header().get("sections");
            
            object.properties().clear();
            if(mode == 0 || mode == 1 || mode == 2) {
                object.container() == RealObject<ValueType_, 3>::tensor_type({columns, rows, sections}, _mrc_image.data().get<ValueType_>());
            }
            else {
                std::cerr << "ERROR while reading MRC file:\n"
                        << "The data format (MRC mode:" << mode << ") not supported!\n"
                        << "HINT:\n Only MRC mode 0-4 is supported.\n";
                exit(1);
            }
            
            //TODO implement mode 3 and 4
            // Specifically first create a complex object and fft to real object
            
            
            for(auto prop : _mrc_image.header().get_all()) {
                object.properties().register_property(prop.first, std::to_string(prop.second));
            }
        };
        
        template<typename ValueType_>
        void save_object(const RealObject<ValueType_, 3>& object) {
            
        };

    private:
        
        void load_image() {
            if(!_loaded) {
                _mrc_image.load();
                _loaded = true;
            }
        }
        
        mrc::Image _mrc_image;
        bool _loaded = false;

    };
}

#endif