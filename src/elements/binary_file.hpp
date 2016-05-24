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

#ifndef BINARY_FILE_HPP
#define BINARY_FILE_HPP

#include <iostream>
#include <string>
#include <fstream>

#include "file.hpp"

namespace em {
    namespace element {

        /**
         * @brief           A wrapper class to Read and write the binary files
         * @description     
         * @author          Nikhil Biyani: nikhil(dot)biyani(at)gmail(dot)com
         */
        
        
        class BinaryInputFile : public InputFile {
        public:

            /**
             * Creates the object with filename and mode
             * @param file_name
             * @param mode
             */
            BinaryInputFile(const std::string& file_name)
            : InputFile(file_name, std::ios::binary) {
            };

            /**
             * @brief           Reads an string of character size read_size 
             *                  from current position
             * @param   read_size       number of characters to be read
             * @return 
             */
            std::string read_string(const int& read_size) {
                char* temp = new char[read_size];
                this->get_stream().read(temp, read_size * sizeof (char));

                std::string value = std::string(temp);
                return value;
            };

            /**
             * @brief           Reads in an integer from the current position 
             *                  of the file
             * @return int
             */
            int read_int() {
                int value;
                this->get_stream().read((char*) &value, sizeof (int));

                return value;
            };

            /**
             * @brief           Reads in a float from the current position of 
             *                  the file
             * @return float
             */
            float read_float() {
                float value;
                this->get_stream().read((char*) &value, sizeof (float));

                return value;
            };
        };
        
    }
}


#endif /* BINARY_FILE_HPP */

