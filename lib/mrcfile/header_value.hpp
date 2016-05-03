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

#ifndef HEADER_VALUE_HPP
#define HEADER_VALUE_HPP

#include <iostream>
#include <string>
#include <cassert>
#include <type_traits>
#include <typeinfo>

namespace mrc {
    
    class HeaderValueInterface {
    
    public:
        
        virtual std::string to_string() const = 0;
        
        virtual bool to_ofstream(std::ofstream& ofs) const = 0;
        
        virtual bool from_string(const std::string& str) = 0;
        
        virtual bool from_ifstream(std::ifstream& ifs) = 0;
        
    };
    
    template<typename FundamentalType_>
    class HeaderValue : public HeaderValueInterface {
    public:
        FundamentalType_ value;
        
        HeaderValue(const FundamentalType_& val) : value(val) {
        };
        
        std::string to_string() const {
            std::ostringstream convert;
            convert << value;
            return convert.str();
        };
        
        bool to_ofstream(std::ofstream& ofs) const {
            ofs.write((const char*) &value, sizeof(value));
            if (ofs) return true;
            else return false;
        }
        
        bool from_string(const std::string& str) {
            std::istringstream ss(str);
            if(ss >> value) return true;
            else return false;
        };
        
        bool from_ifstream(std::ifstream& ifs) {
            ifs.read((char*) &value, sizeof(value));
            if(ifs) return true;
            else return false;
        }
        
    };
    
    
    
}

#endif 