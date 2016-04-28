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


#include <string>
#include <map>

#include "property.hpp"


namespace em {

    class PropertiesMap {
    public:

        void register_property(std::string name, std::string value) {
            Property property;
            property.set_value(value);
            _properties[name] = property;
        };

        void register_property(std::string name, Property property) {
            _properties[name] = property;
        };

        Property get_property(std::string name) {
            return _properties[name];
        };
        
        void clear() {
            _properties.clear();
        }

    private:
        std::map<std::string, Property> _properties;
    };
}

#endif /* OBJECT_PROPERTIES_HPP */

