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

#ifndef OBJECTPROPERTY_HPP
#define OBJECTPROPERTY_HPP

#include <iostream>
#include <string>

namespace em
{
    
    class ObjectProperty
    {
    public:
        ObjectProperty()
        {
            _value_set = false;
            _has_default = false;
        };
        
        ObjectProperty(std::string value, std::string default_value)
        {
            set_value(value);
            set_default(default_value);
        };
        
        std::string value() const
        {
            if(_value_set) return _value;
            else if(_has_default) return _default;
            else
            {
                std::cerr << "WARNING: No property value or it's default set, still returning something!!\n";
                return "";
            }
        };
        
        int int_value() const
        {
            return std::stoi(value());
        };
        
        double double_value() const
        {
            return std::stod(value());
        };
        
        float float_value() const
        {
            return std::stof(value());
        };
        
        bool has_value_set()
        {
            return _value_set;
        }
        
        void set_default(std::string value)
        {
            _has_default = true;
            _default = value;
        };
        
        void set_value(std::string value)
        {
            _value_set = true;
            _value = value;
        };
            
    private:
            
        std::string _value;
        std::string _default;
        bool _value_set;
        bool _has_default;
    };
}


#endif /* OBJECTPROPERTY_HPP */

