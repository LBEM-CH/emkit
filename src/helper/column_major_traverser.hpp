/* 
 * Author: Nikhil Biyani - nikhil(dot)biyani(at)gmail(dot)com
 *
 * This file is a part of 2dx.
 * 
 * 2dx is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or any 
 * later version.
 * 
 * 2dx is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public 
 * License for more details <http://www.gnu.org/licenses/>.
 */

#ifndef COLUMN_MAJOR_TRAVERSER_HPP
#define	COLUMN_MAJOR_TRAVERSER_HPP

#include "traverser.hpp"
#include "array3D.hpp"

namespace em
{
    /**
     * The column major array traverser. The fastest changing index in
     * the traverse would be the columns (first dimension) followed by
     * rows (second dimension) and the sections (third dimension). Note
     * that the traverser only allows to get the values. It does not
     * change them.
     */
    template <class T>
    class ColumnMajorTraverser : public ArrayTraverser3D<T>
    {
        
    public:
        
        /**
         * Associates the traverser with an array.
         * @param array
         */
        ColumnMajorTraverser(ColumnMajorArray3D<T>* array) 
            : _array(array), _index(0)
        {
        };
        
        void begin() override
        {
            _index(0);
        };
        
        bool finished() override
        {
            return(_index >= _array.size());
        };
        
        virtual void next() override
        {
            _index++;
        };
        
        Voxel current_index() override
        {
            size_t nx_ny = _array.nx()*_array.ny();
            size_t z = _index/(nx_ny);
            size_t y = (_index%nx_ny)/_array.nx();
            size_t x = (_index%nx_ny)%_array.nx();
            
            return Voxel(x, y, z); 
        };
        
        T current_value() override
        {
            return _array->data_at(_index);
        };
    
    private:
        
        /**
         * The array associated with the traverser.
         */
        ColumnMajorArray3D<T>* _array;
        
        size_t _index;
        
    };
}

#endif	/* COLUMN_MAJOR_TRAVERSER_HPP */

