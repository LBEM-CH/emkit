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

#ifndef ARRAY3D_HPP
#define	ARRAY3D_HPP

#include <iostream>
#include <stdexcept>
#include <cassert>
#include <vector>
#include <algorithm>
#include <functional>

namespace em
{
    /**
     * @brief:          A class to store data 3D data in array format.
     * @description:    The data can either be stored in a column major or
     *                  a row major format. Internally uses the std::vector
     *                  class to store the data.
     * @author:         Nikhil Biyani: nikhil(dot)biyani(at)gmail(dot)com
     */
    template <typename T>
    class Array3D
    {
        
    public:

        /**
         * @brief:          ENUM to define format in which the array is stored.
         * @description:    Defines how the data is stored in the memory. For
         *                  COLUMN_MAJOR the columns are fastest changing and 
         *                  rows are slowest changing while in the
         *                  ROW_MAJOR order the rows are fastest 
         *                  changing and the columns are slowest changing.
         * @author          Nikhil Biyani: nikhil(dot)biyani(at)gmail(dot)com
         */
        enum class ArrayFormat
        {
            COLUMN_MAJOR, ROW_MAJOR
        };
        
        /**
         * @brief:          Default constructor. 
         * @description:    Initializes a new empty ColumnMajor array
         */
        Array3D()
            : _data(), _format(ArrayFormat::COLUMN_MAJOR), _columns(0), _rows(0), _sections(0)
        {
        };
        
        /**
         * @brief:          Constructor with format
         * @description:    Initializes a new empty array with given format
         * @param[in]:  format      format required from the array
         */
        Array3D(ArrayFormat format)
            : _data(), _columns(0), _rows(0), _sections(0), _format(format)
        {
        };

        /**
         * @brief:          Constructor with size and format provided
         * @description:    Initialize empty array of the size provided with
         *                  data stored in the format provided.
         * @param[in]   columns     number of columns
         * @param[in]   rows        number of rows
         * @param[in]   sections    number of sections
         * @param[in]   format      format for the storage of data
         */
        Array3D(const size_t& columns, 
                const size_t& rows, 
                const size_t& sections,
                ArrayFormat format=ArrayFormat::COLUMN_MAJOR)
            :   _format(format), 
                _columns(columns), 
                _rows(rows), _sections(sections)
        {
            _data = std::vector<T>(columns*rows*sections, T());
        };

        /**
         * @brief:          Defines addition operator. 
         * @description:    Adds two arrays of same size.
         *                  Note that the addition should also be
         *                  defined in the templated class.
         * @param[in]   rhs         another instance with the same size
         * @return added array
         */
        virtual Array3D operator+(const Array3D& rhs) const
        {
            assert(columns() == rhs.columns());
            assert(rows() == rhs.rows());
            assert(sections() == rhs.sections());
            assert(format() == rhs.format());

            Array3D result(columns(), rows(), sections(), format());

            std::transform(_data.begin(), _data.end(), rhs._data.begin(), std::back_inserter(result._data), std::plus<T>());
            return result;
        }
        
        /**
         * @brief:          Defines multiplication operator with a factor. 
         * @description:    Multiplies each element of array with the factor
         *                  Note that the multiplication should also be
         *                  defined in the templated class.
         * @param[in]   rhs         another instance with the same size
         * @return modified array
         */
        virtual Array3D operator*(const double& factor)
        {
            std::for_each(_data.begin(), _data.end(), [&](T val){return val*factor;});
        }

        /**
         * @brief:          Empties the array
         */
        void clear()
        {
            _data.clear();
            _data = std::vector<T>(columns()*rows()*sections(), T());
        };

        /**
         * @brief:              Returns the total size of the memory allocated 
         *                      to the array
         * @return (size_t)
         */
        virtual size_t size() const
        {
            return _data.size();
        };

        /**
         * @brief:              Returns the total number of columns
         * @return (size_t)
         */
        size_t columns() const
        {
            return _columns;
        };

        /**
         * @brief:              Returns the total number of rows
         * @return (size_t)
         */
        size_t rows() const
        {
            return _rows;
        };
        
        /**
         * @brief:              Returns the total number of sections
         * @return (size_t)
         */
        size_t sections() const
        {
            return _sections;
        };
        
        /**
         * @brief:              Returns the format of the storage
         * @return ArrayFormat
         */
        ArrayFormat format() const
        {
            return _format;
        }

        /**
         * @brief:              Get the const pointer to the data
         * @return const raw pointer to data
         */
        const T* data() const
        {
            return _data.data();
        };
        
        /**
         * @brief:          Get the pointer to the data
         * @description:    User is responsible for modification of the data
         * @return raw data pointer
         */
        T* data()
        {
            return _data.data();
        }

        /**
         * @brief:          Sets the new data with the new sizes provided. 
         * @description:    Note that the user is responsible for memory leak 
         *                  that can occur due to wrongly allocated data pointer.
         * @param[in]   data    raw data pointer
         */
        void set_data(const T* data)
        {
            assert(data != nullptr);
            clear();
            _data = std::vector<T>(data, data+size());
        };

        /**
         * @brief           Fetches the data at the indices provided
         * @param[in]   column  column index
         * @param[in]   row     row index
         * @param[in]   section section index
         * @return data value at the index
         */
        T data_at(size_t column, size_t row, size_t section) const
        {
            assert(indices_in_limit(column, row, section) == true);
            return _data[memory_id(column, row, section)];
        };

        /**
         * @brief:          Fetches the data stored at the memory location provided
         * @param[in]   id  index of the memory location
         * @return data value
         */
        T data_at(size_t id) const
        {
            assert(id < size());
            return _data[id];
        };

        /**
         * @brief:          Sets the data at the indices provided
         * @param   column  column index
         * @param   row     row index
         * @param   section section index
         * @param   value   value at the index
         */
        void set_data_at(size_t column, size_t row, size_t section, T value)
        {
            assert(indices_in_limit(column, row, section));
            _data[memory_id(column, row, section)] = value;
        };

        /**
         * @brief:          Sets the value at the memory location provided
         * @param   id      index of the memory location
         * @param   value
         */
        void set_data_at(int id, T value)
        {
            assert(id < size());
            _data[id] = value;
        };

    protected:

        /**
         * @brief           Returns the memory location of the indices provided
         * @param   column  column index
         * @param   row     row index
         * @param   section section index
         * @return 
         */
        size_t memory_id(size_t column, size_t row, size_t section) const
        {
            if(_format == ArrayFormat::COLUMN_MAJOR) 
                return ( column + (row*columns()) + (section*rows()*columns()) );
            else if(_format == ArrayFormat::ROW_MAJOR) 
                return ( section + (row*sections()) + column*sections()*rows() );
            else
            {
                std::cerr << "ERROR! Unrecognized array format";
                return 0;
            }
        };

        /**
         * @brief           Checks if the provided indices are in limit.
         * @param   column  column index
         * @param   row     row index
         * @param   section section index
         * @return 
         */
        bool indices_in_limit(size_t column, size_t row, size_t section) const
        {
            bool result = true;
            if(column >= columns() || row >= rows())
            {
                result = false;
            }

            return result;
        };

        size_t _columns, _rows, _sections;
        ArrayFormat _format;
        std::vector<T> _data;
    };
}


#endif	/* ARRAY3D_HPP */