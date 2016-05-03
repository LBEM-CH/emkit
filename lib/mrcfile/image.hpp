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

#ifndef IMAGE_HPP_PNDT63
#define IMAGE_HPP_PNDT63

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>
#include <omp.h>

#include "header.hpp"
#include "data.hpp"

#include "format_specifier.hpp"
#include "mrc2000_format_specifier.hpp"
#include "map_format_specifier.hpp"

namespace mrc {

    class Image {
    public:

        Image(std::string file_name, std::string format) {
            _file_name = file_name;

            if (format == "mrc") _format = FormatSpecifier();
            else if(format == "mrc2000") _format = MRC2000FormatSpecifier();
            else if (format == "map") _format = MAPFormatSpecifier();
            else {
                std::cerr << "ERROR: The specified format (" << format << ") is not a"
                        << " standard MRC format. Please choose from:"
                        << "\n\t mrc, mrc2000, map\n";
                std::exit(1);
            }

            _header = Header(_format);
            _data = Data(_format);

        };

        void load() {
            std::ifstream is(_file_name, std::ios::binary);
            if (!_header.load(is)) {
                throw std::runtime_error("Unable to load header from file: " + _file_name);
            }
            if (!_data.load(is, _header.data_points(), _header.mode(), _header.should_swap_endianness())) {
                throw std::runtime_error("Unable to load data from file: " + _file_name);
            }
        }

        void save() {
            if (_header.data_points() != _data.data_points()) {
                throw std::runtime_error("The data points to be written in header and the actual present do not match");
            }

            if (_header.mode() != _data.mode()) {
                throw std::runtime_error("The mode of data to be written in header and the actual do not match");
            }

            std::ofstream os(_file_name, std::ios::binary);
            if (!_header.save(os)) {
                throw std::runtime_error("Unable to save header to file: " + _file_name);
            }
            if (!_data.save(os)) {
                throw std::runtime_error("Unable to save data to file: " + _file_name);
            }
        }

        const Header& header() const {
            return _header;
        };

        Header& header() {
            return _header;
        };

        const Data& data() const {
            return _data;
        }

        Data& data() {
            return _data;
        }


    private:
        std::string _file_name;
        FormatSpecifier _format;
        Header _header;
        Data _data;
    };
}

#endif /* IMAGE_HPP */

