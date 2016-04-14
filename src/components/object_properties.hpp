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
 */

#ifndef OBJECT_PROPERTIES_HPP
#define OBJECT_PROPERTIES_HPP

#include "object_property.hpp"


namespace em
{
    class ObjectProperties
    {
    public:
        
        register_property(std::string name, std::string value)
        {
            ObjectProperty property();
            property.set_value(value);
            _properties.insert(std::pair<std::string, ObjectProperty>(name, property) );
        };
        
        register_property(std::string name, ObjectProperty property)
        {
            _properties.insert(std::pair<std::string, ObjectProperty>(name, property) );
        };
        
        ObjectProperty get_property(std::string name)
        {
            return _properties[name];
        };
        
    private:
        std::map<std::string, ObjectProperty> _properties;
    };
}

#endif /* OBJECT_PROPERTIES_HPP */

