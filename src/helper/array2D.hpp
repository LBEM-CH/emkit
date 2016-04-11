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

#ifndef COLUMN_MAJOR_ARRAY2D_HPP
#define	COLUMN_MAJOR_ARRAY2D_HPP

#include <iostream>
#include <stdexcept>

namespace em
{
    /**
     * A class to store data 2D data in column major format.
     * (like Fortran/MATLAB). The data can be traversed in either
     * coordinates (x, y) or using the ids in which the data
     * is stored in the memory.
     */
    template <class T>
    class ColumnMajorArray2D
    {
        public:

        /**
         * Default constructor. Data pointer will point to NULL and the 
         * sizes will be initialized by 0 
         */
        ColumnMajorArray2D()
            : _data(NULL), _nx(0), _ny(0)
        {
        };

        /**
         * Initialize the size of the array and set data to 0
         * @param nx
         * @param ny
         */
        ColumnMajorArray2D(const size_t& nx, const size_t& ny)
            : _nx(nx), _ny(ny)
        {
            _data = new T[size()]();
        };

        /**
         * Copy constructor. Deletes the old pointer and copies the new data to
         * the newly created pointer.
         * @param other
         */
        ColumnMajorArray2D(const ColumnMajorArray2D& other)
        {
            reset(other);
        };

        /**
         * Destructor
         */
        ~ ColumnMajorArray2D()
        {
            if(_data != NULL) delete[] _data;
        };

        /*
         * Defines equal to operator
         */
        virtual ColumnMajorArray2D& operator=(const ColumnMajorArray2D& rhs)
        {
            reset(rhs);
            return *this;
        };

        /**
         * Defines addition operator. Note that the addition should also be
         * defined in the templated class.
         * @param rhs : another instance with the same size
         * @return added array
         */
        virtual ColumnMajorArray2D operator+(const ColumnMajorArray2D& rhs) const
        {
            ColumnMajorArray2D<T> added(nx(), ny());
            if( (rhs.nx() != nx()) || (rhs.ny() != ny()) )
            {
                std::cerr << "WARNING: Can't add arrays with different sizes\n\n";
                std::cerr << "RHS Size(" << rhs.nx() << ", " << rhs.ny() << ", " 
                          << ") does not match (" << nx() << ", " << ny() << ")\n";
                return *this;
            }

            for(int id=0; id < size(); id++)
            {
                added.set_data_at(id, data_at(id)+rhs.data_at(id));
            }

            return added;
        }

        /**
         * Defines multiplication with a double factor. Note that this
         * operation should also be defined in the templated class
         * @param factor
         * @return array multiplied by the factor
         */
        virtual ColumnMajorArray2D operator*(double factor) const
        {
            ColumnMajorArray2D new_data(nx(), ny());
            for(int id=0; id < size(); id++)
            {
                new_data.set_data_at(id, data_at(id)*factor);
            }

            return new_data;
        }

        /**
         * Resets the data from the array provided
         * @param other
         */
        virtual void reset(const ColumnMajorArray2D& other)
        {
            //Check if the object is different
            if(this != &other)
            {
                set_data(other.nx(), other.ny(), other.data());
            }
        }

        /**
         * Deletes the current data
         */
        void clear()
        {
            if(_data != NULL) delete _data;
            _data = new T[size()]();
        };

        /**
         * Returns the total size of the memory allocated to the array
         * @return (size_t)
         */
        virtual size_t size() const
        {
            return nx()*ny();
        };

        /**
         * Returns the number of rows, aka, size of the first dimension x
         * @return (size_t)
         */
        size_t nx() const
        {
            return _nx;
        };

        /**
         * Returns the total number of columns, aka, size of the second dimension y
         * @return (size_t)
         */
        size_t ny() const
        {
            return _ny;
        };

        /**
         * Get the pointer to the data. User is responsible 
         * for the safety of the data
         * @return (pointer to data)
         */
        T* data() const
        {
            return _data;
        };

        /**
         * Sets the new data with the new sizes provided. Note that the
         * user is responsible for memory leak that can occur due to 
         * wrongly allocated data pointer.
         * @param nx
         * @param ny
         * @param data
         */
        void set_data(size_t nx, size_t ny, T* data)
        {
            _nx = nx;
            _ny = ny;
            if( _data != NULL ) delete[] _data;
            _data = new T[size()]();
            if(data != NULL) std::copy(data, data+size(), _data);
        };


        /**
         * Fetches the data at the indices provided
         * @param x : index in first dimension
         * @param y : index in second dimension
         * @return data
         */
        T data_at(size_t x, size_t y) const
        {
            if(indices_in_limit(x, y))
            {
               return _data[memory_id(x, y)];
            }
            else
            {
                throw std::out_of_range
                        ("ERROR! Fetching value got out of bound indices: " + std::to_string(x) + ", " + 
                          std::to_string(y) + "\n");
            }
        };

        /**
         * Fetches the data stored at the memory location provided
         * @param id : index of the memory location
         * @return data
         */
        T data_at(size_t id) const
        {
            if(id < size())
            {
               return _data[id];
            }
            else
            {
                throw std::out_of_range
                        ("ERROR! Fetching value got out of bound indices: " + std::to_string(id) + "\n");
            }
        };

        /**
         * sets the data at the indices provided
         * @param x : index in first dimension
         * @param y : index in second dimension
         * @param value
         */
        void set_data_at(int x, int y, T value)
        {
            if(indices_in_limit(x, y))
            {
                _data[memory_id(x, y)] = value;
            }
            else
            {
                throw std::out_of_range
                        ("ERROR! Setting value got out of bound indices: " + std::to_string(x) + ", " + 
                          std::to_string(y) + "\n");
            }
        };

        /**
         * Sets the value at the memory location provided
         * @param id : index of the memory location
         * @param value
         */
        void set_data_at(int id, T value)
        {
            if(id < size())
            {
               _data[id] = value;
            }
            else
            {
                throw std::out_of_range
                        ("ERROR! Setting value got out of bound indices: " + std::to_string(id) + "\n");
            }
        };

    protected:

        /**
         * Returns the memory location of the indices provided
         * @param x
         * @param y
         * @return 
         */
        size_t memory_id(size_t x, size_t y) const
        {
            return ( x + (y*nx()) );
        };

        /**
         * Checks if the provided indices are in limit.
         * @param x
         * @param y
         * @return 
         */
        bool indices_in_limit(size_t x, size_t y) const
        {
            bool result = true;
            if(x >= nx() || y >= ny())
            {
                result = false;
            }

            return result;
        };


        size_t _nx;
        size_t _ny;

        T* _data;
    };
}


#endif	/* ARRAY2D_HPP */

