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
 * 
 */

#ifndef FILE_HPP
#define FILE_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <sys/stat.h>
#include <sstream>

namespace em {
    
    namespace element {

        /**
         * @brief           A wrapper class to Read and write the files
         * @description     A derived class from std::fstream with added functions
         *                  to help in read/write
         * @author          Nikhil Biyani: nikhil(dot)biyani(at)gmail(dot)com     
         */
        class File {
        public:

            /**
             * @brief           Default constructor
             */
            File()
            : _filename() {
            }

            /**
             * @brief           Creates the object with filename and mode
             * @param   file_name   origin name with path of file
             */
            File(const std::string& file_name)
            : _filename(file_name) {
            }

            /**
             * @brief           Name with path of the file under consideration
             * @return (string) file name
             */
            std::string file_name() const {
                return _filename;
            }

            /**
             * @brief           Checks if the file exists on the system
             * @return (bool) if file exists
             */
            bool exists() const {
                return exists(file_name());
            }
            
            /**
             * @brief       Checks if a file exists on the system
             * @param       file_name
             * @return 
             */
            static bool exists(std::string file_name) {
                struct stat buffer;
                bool result = (stat(file_name.c_str(), &buffer) == 0);
                return result;
            }

            /**
             * @brief           Returns the file extension.
             * @description     Evaluates the letters after the final '.' appearing 
             *                  in the string.
             *                  If '.' is absent returns ''
             * @return (string) the extension of the file
             */
            std::string extension() const {
                return extension(file_name());
            }
            
            /**
             * @brief           Returns the file extension.
             * @description     Evaluates the letters after the final '.' appearing 
             *                  in the string.
             *                  If '.' is absent returns ''
             * @param           file_name
             * @return (string) the extension of the file
             */
            static std::string extension(std::string file_name) {
                std::string extension = "";

                if (file_name.find('.') != std::string::npos) {
                    extension = file_name.substr(file_name.find_last_of(".") + 1);
                }

                return extension;
            }

        protected:
            std::string _filename;
        };
        
        
        /**
         * An extension of the File class, which has added method to use the 
         * file in input mode.
         */
        class InputFile : public File {
        
        public:
            InputFile() 
            : File() {
            }
            
            InputFile(std::string file_name) 
            : File(file_name) {
                if(exists()) stream_ .open(file_name);
                else  {
                    std::cerr << "ERROR: File does not exist: \n\t" << file_name; 
                    exit(1);
                }
            }
            
            /**
             * @brief       Moves the file to the beginning position
             */
            void goto_beginning() {
                stream_.clear();
                stream_.seekg(0, std::ios::beg);
            }
            
            /**
             * @brief       Moves the cursor of the file to the @p line_number
             *              provided from the beginning
             * @param       line_number
             */
            void goto_line(int line_number) {
                goto_beginning();
                for(int i=0; i< line_number; ++i) read_line();
            }
            
            /**
             * @brief           Reads a line from the current position
             * @return (string) line
             */
            std::string read_line() {
                std::string line;
                std::getline(stream_, line);
                return line;
            }
            
            /**
             * @brief           Counts number of lines in the file and brings
             *                  the cursor back to the position where it was
             * @return          number of lines in the file
             */
            int number_of_lines() {
                auto pos = stream_.tellg();
                goto_beginning();
                int number_lines =  0;
                while(!stream_.eof()) {
                    read_line();
                    number_lines++;
                }
                stream_.clear();
                stream_.seekg(pos, std::ios::beg);
                return number_lines;
            }
            
            /**
             * @brief           Returns the size of the input file
             * @return size
             */
            size_t file_size() const {
                struct stat stat_buf;
                int rc = stat(file_name().c_str(), &stat_buf);
                return rc == 0 ? stat_buf.st_size : -1;
            }
            
            void clear() {
                stream_.clear();
            }
            
            void close() {
                stream_.close();
            }
            
            bool eof() {
                return stream_.eof();
            }
            
        private:
            
            std::ifstream stream_;
            
            
        };
    }
}


#endif /* FILE_HPP */

