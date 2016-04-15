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

#ifndef TABULARCONATINER_HPP
#define TABULARCONATINER_HPP

#include "container.hpp"
#include "../helper/tripet.hpp"

namespace em {
    
    template<class value_t>
    class TabularContainer : public Container, std::map<Triplet<int>, std::vector<value_t>> {
        
    public:
        
        typedef std::map<Triplet<int>, std::vector<value_t> SortedTable;
        
        TabularContainer();
        
        Iterator<value_t> begin() override {
            return SortedTable::begin();
        };
        
        Iterator<value_t> cbegin() const override {
            return SortedTable::cbegin();
        };

        Iterator<value_t> end() override {
            return SortedTable::end();
        };
        
        Iterator<value_t> cend() const override {
            return SortedTable::cend();
        };

        Container::Type container_type() const override {
            return Container::Type::TABULAR;
        };



    };
}

#endif /* TABULARCONATINER_HPP */

