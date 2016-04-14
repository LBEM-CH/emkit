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

#ifndef ARRAYCONTAINER_HPP
#define ARRAYCONTAINER_HPP

#include "container.hpp"
#include "../helper/array3D.hpp"

namespace em {

    template<class T>
    class ArrayContainer : public Container<T>, Array3D<T> {
    public:
        ArrayContainer();

        Iterator<T> begin() override {
            return _data.begin();
        };
        
        Iterator<T> cbegin() const override {
            return _data.cbegin();
        };
        
        Iterator<T> end() override {
            return _data.end();
        };
        
        Iterator<T> cend() const override {
            return _data.cend();
        };

        Container::Type container_type() const override {
            return Container::Type::ARRAY;
        };



    };
}

#endif /* ARRAYCONTAINER_HPP */
