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

#ifndef ROW_MAJOR_TRAVERSER_HPP
#define	ROW_MAJOR_TRAVERSER_HPP

#include "traverser.hpp"
#include "array3D.hpp"
#include "voxel.hpp"

namespace em
{
    /**
     * The row major array traverser. The fastest changing index in
     * the traverse would be the sections (last dimension) followed by
     * rows (second dimension) and the columns (first dimension). Note
     * that the traverser only allows to get the values. It does not
     * change them.
     */
    template <class T>
    class RowMajorTraverser : public ArrayTraverser3D<T>
    {
        
    public:
        
        /**
         * Associates the traverser with an array.
         * @param array
         */
        RowMajorTraverser(ColumnMajorArray3D<T>* array) 
            : _array(array), _index(0, 0, 0)
        {
        };
        
        void begin() override
        {
            _index(0, 0, 0);
        };
        
        bool finished() override
        {
            return(_index.x() >= _array->nx()
                    && _index.y() >= _array->ny()
                    && _index.z() >= _array->nz());
        };
        
        virtual void next() override
        {
            if(_index.z()+1 != _array->nz())
            {
                int z = _index.z() + 1;
                _index.reset(_index.x(), _index.y(), z);
            }
            else if(_index.y() != _array->ny())
            {
                int y = _index.y() + 1;
                _index.reset(_index.x(), y, _index.z());
            }
            else if(_index.x()+1 != _array->nx())
            {
                int x = _index.x() + 1;
                _index.reset(x, _index.y(), _index.z());
            }
            else
            {
                std::cerr << "WARNING: The array already finished traversing\n";
            }
        };
        
        TripletIndex current_index() override
        {   
            return _index; 
        };
        
        T current_value() override
        {
            return _array->data_at(_index.x(), _index.y(), _index.z());
        };
    
    private:
        
        /**
         * The array associated with the traverser.
         */
        ColumnMajorArray3D<T>* _array;
        TripletIndex _index;
        
    };
}

#endif	/* ROW_MAJOR_TRAVERSER_HPP */

