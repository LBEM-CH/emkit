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
#include <vector>

#include "property.hpp"


namespace em {

    class PropertiesMap {
    public:

        void register_property(std::string name, std::string value) {
            Property property;
            property.set_value(value);
            register_property(name, property);
        };

        void register_property(std::string name, Property property) {
            _properties.push_back(std::make_pair(name, property));
        };

        Property get_property(std::string name) const {
            for(const auto& prop : _properties) {
                if(prop.first == name) return prop.second;
            }
            
            std::cerr << "The property requested was not found. Returning something.\n";
            return Property();
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
        std::vector<std::pair<std::string, Property>> _properties;
    };
}

#endif /* OBJECT_PROPERTIES_HPP */

