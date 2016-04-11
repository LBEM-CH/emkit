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

#ifndef BINARY_FILE_HPP
#define	BINARY_FILE_HPP

#include <iostream>
#include <string>
#include <fstream>

#include "file.hpp"

namespace em
{
    /**
     * A wrapper class to Read and write the binary files
     */
    class BinaryFile : public File
    {
    public:
        
        /**
         * Creates the object with filename and mode
         * @param file_name
         * @param mode
         */
        BinaryFile(const std::string& file_name, openmode mode = INPUT | OUTPUT);
        
        /**
         * Reads an string of character size read_size from current position
         * @param read_size (number of characters to be read)
         * @return 
         */
        std::string read_string(const int& read_size);

        /**
         * Reads in an integer from the current position of the file
         * @return int
         */
        int read_int();

        /**
         * Reads in a float from the current position of the file
         * @return float
         */
        float read_float();
        
    private:
        std::string _filename;
    };
}


#endif	/* BINARY_FILE_HPP */

