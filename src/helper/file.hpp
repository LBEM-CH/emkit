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
     * @brief           A wrapper class to Read and write the files
     * @description     A derived class from std::fstream with added functions
     *                  to help in read/write
     * @author          Nikhil Biyani: nikhil(dot)biyani(at)gmail(dot)com     
     */
    class File : public std::fstream
    {
    public:
        
        typedef std::ios_base::openmode openmode;
        
        static const openmode INPUT = std::ios::in;
        static const openmode OUTPUT = std::ios::out;
        
        /**
         * @brief           Default constructor
         */
        File()
            : std::fstream(), _filename()
        {
        };
        
        /**
         * @brief           Creates the object with filename and mode
         * @param   file_name   origin name with path of file
         * @param   mode        Mode of access (INPUT/OUTPUT)
         */
        File(const std::string& file_name, openmode mode = INPUT | OUTPUT)
            : std::fstream(file_name.c_str(), mode)
        {
            _filename = file_name;
        };
        
        /**
         * @brief           Name with path of the file under consideration
         * @return (string) file name
         */
        std::string file_name() const
        {
            return _filename;
        };
        
        /**
         * @brief           Checks if the file exists on the system
         * @return (bool) if file exists
         */
        bool exists() const
        {
            struct stat buffer;   
            bool result = (stat (file_name().c_str(), &buffer) == 0);
            return result;
        };
        
        /**
         * @brief           Returns the file extension.
         * @description     Evaluates the letters after the final '.' appearing 
         *                  in the string.
         *                  If '.' is absent returns ''
         * @return (string) the extension of the file
         */
        std::string extension() const
        {
            std::string extension = "";
    
            if(file_name().find('.') != std::string::npos){
                extension = file_name().substr(file_name().find_last_of(".") + 1);
            }
    
            return extension; 
        };
        
        /**
         * @brief           Returns the size of the input file
         * @return size
         */
        size_t file_size() const
        {
            struct stat stat_buf;
            int rc = stat(file_name().c_str(), &stat_buf);
            return rc == 0 ? stat_buf.st_size : -1;  
        };
        
        /**
         * @brief           Reads a line from the current position
         * @return (string) line
         */
        std::string read_line()
        {
            std::string line;
            std::getline(*this, line);
            return line;
        };
        
    protected:
        std::string _filename;
    };
}


#endif	/* FILE_HPP */

