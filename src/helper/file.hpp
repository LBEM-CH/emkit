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

#ifndef FILE_HPP
#define	FILE_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <sys/stat.h>
#include <sstream>

namespace em
{
    /**
     * A wrapper class to Read and write the files
     */
    class File : public std::fstream
    {
    public:
        
        typedef std::ios_base::openmode openmode;
        
        static const openmode INPUT = std::ios::in;
        static const openmode OUTPUT = std::ios::out;
        
        /**
         * Default constructor
         */
        File();
        
        /**
         * Creates the object with filename and mode
         * @param file_name
         * @param mode
         */
        File(const std::string& file_name, openmode mode = INPUT | OUTPUT);
        
        /**
         * Name with path of the file under consideration
         * @return (string) file name
         */
        std::string file_name() const;
        
        /**
         * Checks if the file exists on the system
         * @return (bool) if file exists
         */
        bool exists() const;
        
        /**
         * Returns the file extension.
         * Evaluates the letters after the final '.' appearing in the string.
         * If '.' is absent returns ''
         * @return (string) the extension of the file
         */
        std::string extension() const;
        
        /**
         * Returns the size of the input file
         * @return size
         */
        size_t file_size() const;
        
        /**
         * Reads a line from the current position
         * @return (string) line
         */
        std::string read_line();
        
    private:
        std::string _filename;
    };
}


#endif	/* FILE_HPP */

