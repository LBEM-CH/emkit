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

#ifndef HEADER_HPP_HC343PL
#define HEADER_HPP_HC343PL

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include "format_specifier.hpp"
#include "byte_swapper.hpp"
#include "header_value.hpp"
#include "header_property.hpp"

namespace mrc {

    /**
     * MRC Header class
     */
    class Header {
    public:
        
        Header(FormatSpecifier format = FormatSpecifier())
        : _format(format), _swap_byte_order(false) {
            _values = _format.header_properties();
            if (std::find(_values.begin(), _values.end(), "mode") == _values.end()
                    || std::find(_values.begin(), _values.end(), "columns") == _values.end()
                    || std::find(_values.begin(), _values.end(), "rows") == _values.end()
                    || std::find(_values.begin(), _values.end(), "sections") == _values.end()) {
                std::cerr << "ERROR: MRC Format specifier error:\n"
                        << "mode/columns/rows/sections fields are required in format specification.\n";
                exit(1);
            }
            
            //Set the default mode to 2
            set("mode", "2");
        };

        bool load(std::ifstream& is) {
            is.seekg(_format.header_offset(), is.beg);

            for(auto& v : _values) {
                v.set_value(is);
            }
            
            int bitMask = std::stoi(get("mapc")) | std::stoi(get("mapr")) | std::stoi(get("maps"));
            if (bitMask == 3 || bitMask == 0) _swap_byte_order = false;
            else {
                std::cout << "NOTE: Swap Byte Order is marked true.\n";
                _swap_byte_order = true;
            }

            if (_swap_byte_order) {
                for (int i = 0; i < 256; i++) ByteSwapper::byte_swap(&_values[i], sizeof(float));
            }

            if (is) {
                return true;
            }
            else return false;
        }

        bool save(std::ofstream& os) {
            os.seekp(_format.header_offset(), os.beg);
            
            for(const auto& v : _values) {
                v.write_value(os);
            }

            if (os) return true;
            else return false;
        }

        int mode() {
            return std::stoi(get("mode"));
        };
        
        size_t data_points() {
            return (size_t) (std::stoi(get("columns")) 
                    * std::stoi(get("rows")) 
                    * std::stoi(get("sections")) );
        }

        bool should_swap_endianness() {
            return _swap_byte_order;
        }

        std::string get(std::string field) const {
            const auto& found_field = std::find(_values.begin(), _values.end(), field);
            if (found_field == _values.end()) {
                std::cerr << "Warning: The requested field (" << field << ") not found in header specification returning 0\n";
                std::cerr << "Following are the allowed values:\n";
                for (const auto& f : _values) std::cerr << f.identifier() << std::endl;
                return "";
            } else {
                return _values[std::distance(_values.begin(), found_field)].value();
            }
        };
        
        std::map<std::string, std::string> get_all() const {
            std::map<std::string, std::string> values_map;
            for(const auto& f : _values) {
                values_map[f.identifier()] = f.value();
            }
            return values_map;
        }

        void set(const std::string& field, const std::string& value) {
            const auto& found_field = std::find(_values.begin(), _values.end(), field);
            if (found_field == _values.end()) {
                std::cerr << "Warning: The requested field (" << field << ") not found in header specification returning 0\n";
                std::cerr << "Following are the allowed values:\n";
                for (const auto& f : _values) std::cerr << f.identifier() << std::endl;
            } else {
                _values[std::distance(_values.begin(), found_field)].set_value(value);
            }
        };

    private:

        FormatSpecifier _format;
        bool _swap_byte_order;
        std::vector<HeaderProperty> _values;
    };
}

#endif /* MRC_HEADER_HPP */

