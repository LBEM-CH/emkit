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

#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "iterator.hpp"


namespace em
{
    template<class value_t>
    class Container
    {
    public:
        
        enum class Type
        {
            NONE, ARRAY, TABULAR, STACK
        };
        
        virtual Iterator<value_t> begin();
        virtual Iterator<value_t> end();
        virtual Iterator<value_t> cbegin() const;
        virtual Iterator<value_t> cend() const;
        
        virtual Type container_type() const;
            
        
    };
}

#endif /* CONTAINER_HPP */

