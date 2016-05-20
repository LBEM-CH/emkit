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

#include <iostream>
#include <string>

#include "../elements/file.hpp"
#include "mrc_file.hpp"
#include "reflection_file.hpp"

namespace em {

    namespace fileio {

        enum class FileFormat {
            MRC,
            MAP,
            HKL
        };
        
        template<typename ObjectType_, FileFormat format_>
        struct io_impl {
            static bool read(std::string file, ObjectType_& obj) {
                std::cerr << "ERROR: The format could not be recognized from file: "<< file << "\n\n";
                return false;
            }
            
            static bool write(std::string file, const ObjectType_& obj) {
                std::cerr << "ERROR: The format could not be recognized from file: "<< file << "\n\n";
                return false;
            }
        };
        
        template<typename ObjectType_>
        struct io_impl<ObjectType_, FileFormat::MRC> {
            static bool read(std::string file, ObjectType_& obj) {
                element::PropertiesMap temp;
                return fileio::MRCFile(file, "mrc").load(obj, temp);
            }
            
            static bool write(std::string file, const ObjectType_& obj) {
                element::PropertiesMap temp;
                return fileio::MRCFile(file, "mrc").save(obj, temp);
            }
        };
        
        template<typename ObjectType_>
        struct io_impl<ObjectType_, FileFormat::MAP> {
            static bool read(std::string file, ObjectType_& obj) {
                element::PropertiesMap temp;
                return fileio::MRCFile(file, "map").load(obj, temp);
            }
            
            static bool write(std::string file, const ObjectType_& obj) {
                element::PropertiesMap temp;
                return fileio::MRCFile(file, "map").save(obj, temp);
            }
        };
        
        template<typename ObjectType_>
        struct io_impl<ObjectType_, FileFormat::HKL> {
            static bool read(std::string file, ObjectType_& obj) {
                return fileio::ReflectionFile(file).load(obj);
            }
            
            static bool write(std::string file, const ObjectType_& obj) {
                return fileio::ReflectionFile(file).save(obj);
            }
        };
        
        template<typename ObjectType_>
        bool read(std::string file, std::string format, ObjectType_& obj) {
            if (!element::File(file).exists()) {
                std::cerr << "ERROR! File not found: " << file << std::endl;
                return false;
            }

            if (format == "") {
                format = element::File(file).extension();
            }

            if (format == "mrc" || format == "MRC") return io_impl<ObjectType_, FileFormat::MRC>::read(file, obj);
            else if (format == "map" || format == "MAP") return io_impl<ObjectType_, FileFormat::MAP>::read(file, obj);
            else if (format == "hkl" || format == "hk") return io_impl<ObjectType_, FileFormat::HKL>::read(file, obj);
            else {
                std::cerr << "ERROR: The format: " << format << " was not recognized format.\n\n";
                return false;
            }
        }
        
        template<typename ObjectType_>
        bool read(std::string file, ObjectType_& obj) {
            return read(file, "", obj);
        }

        template<typename ObjectType_>
        bool write(std::string file, std::string format, const ObjectType_& obj) {
            //Check for the existence of the file
            if (element::File(file).exists()) {
                std::cout << "WARNING: File.. " << file << " already exists. Overwriting!\n";
            }

            if (format == "") {
                format = element::File(file).extension();
            }

            if (format == "mrc" || format == "MRC") return io_impl<ObjectType_, FileFormat::MRC>::write(file, obj);
            else if (format == "map" || format == "MAP") return io_impl<ObjectType_, FileFormat::MAP>::write(file, obj);
            else if (format == "hkl" || format == "hk") return io_impl<ObjectType_, FileFormat::HKL>::write(file, obj);
            else {
                std::cerr << "ERROR: The format: " << format << " was not recognized format.\n\n";
                return false;
            }
        }

        template<typename ObjectType_>
        bool write(std::string file, ObjectType_& obj) {
            return write(file, "", obj);
        }

    }
}

#endif /* FILE_IO_HPP */

