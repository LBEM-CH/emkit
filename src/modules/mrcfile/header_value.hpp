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

#ifndef HEADER_VALUE_HPP
#define HEADER_VALUE_HPP

#include <iostream>
#include <string>
#include <cassert>
#include <type_traits>
#include <typeinfo>

namespace em {
    namespace mrc {

        class HeaderValueInterface {
        public:

            virtual std::string to_string() const = 0;

            virtual bool to_ofstream(std::ofstream& ofs) const = 0;

            virtual bool from_string(const std::string& str) = 0;

            virtual bool from_ifstream(std::ifstream& ifs) = 0;

        };

        template<typename FundamentalType_>
        class HeaderValue : public HeaderValueInterface {
        public:
            FundamentalType_ value;

            HeaderValue(const FundamentalType_& val) : value(val) {
            };

            std::string to_string() const {
                std::ostringstream convert;
                convert << value;
                return convert.str();
            };

            bool to_ofstream(std::ofstream& ofs) const {
                ofs.write((const char*) &value, sizeof (FundamentalType_));
                if (ofs) return true;
                else return false;
            }

            bool from_string(const std::string& str) {
                std::istringstream ss(str);
                if (ss >> value) return true;
                else return false;
            };

            bool from_ifstream(std::ifstream& ifs) {
                ifs.read((char*) &value, sizeof (FundamentalType_));
                if (ifs) return true;
                else return false;
            }

        };

        template<>
        class HeaderValue<std::string> : public HeaderValueInterface {
        public:
            std::string value;
            size_t length;

            HeaderValue(const size_t& len)
            : length(len) {
                from_string("");
            }

            HeaderValue(const size_t& len, const std::string& str)
            : length(len) {
                from_string(str);
            }

            std::string to_string() const {
                return value;
            };

            bool to_ofstream(std::ofstream& ofs) const {
                ofs.write(value.c_str(), sizeof (char)*length);
                if (ofs) return true;
                else return false;
            }

            bool from_string(const std::string& str) {
                if (str.size() > length) {
                    std::cerr << "The length of string '" << str << "' was expected to be: " << length
                            << " but found: " << str.size() << " Trimming to appropriate length\n";
                    value = str.substr(0, 4);
                } else if (str.size() < length) {
                    value = str;
                    for (int i = str.size(); i < length; ++i) value += " ";
                } else {
                    value = str;
                }
                return true;
            };

            bool from_ifstream(std::ifstream& ifs) {
                char* temp = new char[length];
                ifs.read(temp, sizeof (char)*length);
                std::string str = std::string(temp, length);
                delete[] temp;
                from_string(str);
                if (ifs) return true;
                else return false;
            }

        };


    }
}

#endif 