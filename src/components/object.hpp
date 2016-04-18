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

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <iostream>
#include <memory>

#include "properties_map.hpp"
#include "container.hpp"


namespace em {

    class Object {
    public:

        const PropertiesMap& properties() const {
            return _properties;
        };

        PropertiesMap& properties() {
            return _properties;
        };

        virtual const Container& container() const {
            return _container;
        };

        virtual Container& container() {
            return _container;
        };

        virtual void set_container(const Container& c) {
            _container = c;
        };

    protected:
        Container _container;
        PropertiesMap _properties;
    };
}

#endif /* OBJECT_HPP */

