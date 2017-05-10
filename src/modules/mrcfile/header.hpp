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
#include <cstdlib>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include "format_specifier.hpp"
#include "byte_swapper.hpp"
#include "header_value.hpp"
#include "header_property.hpp"

namespace em {
    namespace mrc {

        /**
         * MRC Header class
         */
        class Header {
        public:

            Header(std::shared_ptr<FormatSpecifier> format = std::shared_ptr<FormatSpecifier>(new FormatSpecifier()))
            : format_(format), swap_byte_order_(false) {
                values_ = format_->header_properties();
                if (std::find(values_.begin(), values_.end(), "mode") == values_.end()
                        || std::find(values_.begin(), values_.end(), "columns") == values_.end()
                        || std::find(values_.begin(), values_.end(), "rows") == values_.end()
                        || std::find(values_.begin(), values_.end(), "sections") == values_.end()) {
                    std::cerr << "ERROR: MRC Format specifier error:\n"
                            << "mode/columns/rows/sections fields are required in format specification.\n";
                    exit(1);
                }

                //Set the default mode to 2
                set("mode", "2");
            };

            bool load(std::ifstream& is) {
                is.seekg(format_->header_offset(), is.beg);

                for (auto& v : values_) {
                    if (!v.set_value(is)) {
                        std::cerr << "Error in reading '" << v.identifier() << "' from the file..\n";
                        return false;
                    }
                }

                int bitMask = std::stoi(get("mapc")) | std::stoi(get("mapr")) | std::stoi(get("maps"));
                if (bitMask == 3 || bitMask == 0) swap_byte_order_ = false;
                else {
                    std::cout << "NOTE: Swap Byte Order is marked true.\n";
                    swap_byte_order_ = true;
                }

                if (swap_byte_order_) {
                    for (int i = 0; i < 256; i++) ByteSwapper::byte_swap(&values_[i], sizeof (float));
                }

                if (is) {
                    return true;
                } else return false;
            }

            bool save(std::ofstream& os) {
                os.seekp(format_->header_offset(), os.beg);

                for (const auto& v : values_) {
                    v.write_value(os);
                }

                if (os) return true;
                else return false;
            }
            
            void clear() {
                values_.clear();
            }

            int mode() {
                return std::stoi(get("mode"));
            };

            size_t data_points() {
                return (size_t) (std::stoul(get("columns"))
                        * std::stoul(get("rows"))
                        * std::stoul(get("sections")));
            }

            bool should_swap_endianness() {
                return swap_byte_order_;
            }

            std::string get(std::string field) const {
                const auto& found_field = std::find(values_.begin(), values_.end(), field);
                if (found_field == values_.end()) {
                    std::cerr << "Warning: The requested field (" << field << ") not found in header specification returning 0\n";
                    std::cerr << "Following are the allowed values:\n";
                    for (const auto& f : values_) std::cerr << f.identifier() << std::endl;
                    return "";
                } else {
                    return values_[std::distance(values_.begin(), found_field)].value();
                }
            };

            bool exists(std::string field) {
                const auto& found_field = std::find(values_.begin(), values_.end(), field);
                if (found_field == values_.end()) return false;
                else return true;
            }

            std::map<std::string, std::string> get_all() const {
                std::map<std::string, std::string> values_map;
                for (const auto& f : values_) {
                    values_map[f.identifier()] = f.value();
                }
                return values_map;
            }

            void set(const std::string& field, const std::string& value) {
                const auto& found_field = std::find(values_.begin(), values_.end(), field);
                if (found_field == values_.end()) {
                    std::cerr << "Warning: The requested field (" << field << ") not found in header specification returning 0\n";
                    std::cerr << "Following are the allowed values:\n";
                    for (const auto& f : values_) std::cerr << f.identifier() << std::endl;
                } else {
                    values_[std::distance(values_.begin(), found_field)].set_value(value);
                }
            };

        private:

            std::shared_ptr<FormatSpecifier> format_;
            bool swap_byte_order_;
            std::vector<HeaderProperty> values_;
        };
    }
}
#endif /* MRC_HEADER_HPP */

