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

#ifndef PROPERTIES_MAP_HPP
#define PROPERTIES_MAP_HPP

#include <iostream>
#include <string>
#include <map>


namespace em {

    class PropertiesMap {
    public:

        typedef std::map<std::string, std::string> map_type;
        typedef typename map_type::iterator iterator;
        typedef typename map_type::const_iterator const_iterator;
        
        iterator begin() {
            return _properties.begin();
        }
        
        const_iterator begin() const {
            return _properties.cbegin();
        }
        
        iterator end() {
            return _properties.end();
        }
        
        const_iterator end() const {
            return _properties.cend();
        }
        
        void register_property(std::string name, std::string value) {
            _properties[name] = value;
        };
        
        bool exists(std::string name) const {
            if(_properties.find(name) == _properties.end()) {
                return false;
            }
            else return true;
        }
        
        std::string get_property(std::string name) const {
            if(!exists(name)) {
                std::cerr << "The property requested was not found. Returning something.\n";
                return "";
            }
            return _properties.at(name);
        };
        
        void clear() {
            _properties.clear();
        };
        
        friend inline std::ostream& operator<<(std::ostream& os, const PropertiesMap& properties) {
            for(const auto& prop : properties._properties) {
                std::cout << prop.first << " -> " << prop.second << std::endl;
            }
            return os;
        };

    private:
        map_type _properties;
    };
}

#endif /* OBJECT_PROPERTIES_HPP */

