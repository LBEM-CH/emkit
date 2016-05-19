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

#ifndef FILE_IO_HPP
#define FILE_IO_HPP

#include "../elements/file.hpp"
#include "mrc_file.hpp"

namespace em {

    namespace fileio {

        template<typename ObjectType_>
        bool read(std::string file, std::string format, ObjectType_& object) {
            if (!element::File(file).exists()) {
                std::cerr << "ERROR! File not found: " << file << std::endl;
                return false;
            }

            if (format == "") {
                format = element::File(file).extension();
            }

            if (format == "mrc" || format == "MRC") return fileio::MRCFile(file, "mrc").load(object);
            else if (format == "map" || format == "MAP") return fileio::MRCFile(file, "map").load(object);
            else {
                std::cerr << "ERROR: The format: " << format << " was not recognized format.\n\n";
                return false;
            }
        }

        template<typename ObjectType_>
        bool read(std::string file, ObjectType_& object) {
            return read(file, "", object);
        }

        template<typename ObjectType_>
        bool write(std::string file, std::string format, const ObjectType_& object) {
            //Check for the existence of the file
            if (element::File(file).exists()) {
                std::cout << "WARNING: File.. " << file << " already exists. Overwriting!\n";
            }

            if (format == "") {
                format = element::File(file).extension();
            }

            if (format == "mrc" || format == "MRC") return fileio::MRCFile(file, "mrc").save(object);
            else if (format == "map" || format == "MAP") return fileio::MRCFile(file, "map)").save(object);
            else {
                std::cerr << "ERROR: The format: " << format << " was not recognized format.\n\n";
                return false;
            }
        }

        template<typename ObjectType_>
        bool write(std::string file, ObjectType_& object) {
            return write(file, "", object);
        }

    }
}

#endif /* FILE_IO_HPP */

