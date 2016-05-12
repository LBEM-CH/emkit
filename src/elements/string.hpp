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

#ifndef STRING_HPP
#define STRING_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>

namespace em {
    
    namespace element {

        /**
         * A class with added functions on string.
         */
        class String {
        public:

            /**
             * Initiate the string from std::string
             * @param (std::string) string
             */
            String(const std::string& string)
            : _value(string) {
            }

            /**
             * Returns the std::string
             * @return std::string
             */
            const std::string& std_string() const {
                return _value;
            };

            std::string& std_string() {
                return _value;
            }

            /**
             * Trims the leading/trailing/extra spaces in the string
             * @param string
             * @return std::string
             */
            static std::string trim(const std::string& str) {
                //Remove extra spaces
                std::string trimmed = "";
                bool spaced = false;

                for (const char& curr : str) {
                    if (spaced && curr != ' ') spaced = false;
                    if (!spaced) trimmed += curr;
                    if (curr == ' ') spaced = true;
                }

                //Remove beginning and end white space if exists
                if (trimmed.back() == ' ') trimmed = trimmed.substr(0, trimmed.length() - 1);
                if (trimmed[0] == ' ') trimmed = trimmed.substr(1, trimmed.length() - 1);

                return trimmed;

            }

            /**
             * Trims the leading/trailing/extra spaces in the string
             * @return 
             */
            String trim() const {
                return String(trim(std_string()));
            };

            /**
             * Splits the string with the given delimiter
             * @param s
             * @param delim
             * @return 
             */
            static std::vector<std::string> split(const std::string &s, char delim) {
                std::vector<std::string> elems;
                std::stringstream ss(s);
                std::string item;
                while (std::getline(ss, item, delim)) {
                    elems.push_back(item);
                }
                return elems;
            }

            /**
             * Splits the string with the given delimiter
             * @param delim
             * @return 
             */
            std::vector<String> split(char delim) const {
                std::vector<String> elems;
                std::stringstream ss(std_string());
                std::string item;
                while (std::getline(ss, item, delim)) {
                    elems.push_back(String(item));
                }
                return elems;
            }

            /**
             * Returns a string of precision and width from double
             * @param (double) value
             * @param (int) width
             * @param (int) precision
             * @return (string)
             */
            static std::string string_of(double value, int width, int precision) {
                std::stringstream stream;
                stream << std::fixed << std::setw(width) << std::setprecision(precision) << value;
                std::string s = stream.str();

                return s;
            }

            /**
             * Returns a string of width from int
             * @param (int) value
             * @param (int) width
             * @return (string)
             */
            static std::string string_of(int value, int width) {
                std::stringstream stream;
                stream << std::fixed << std::setw(width) << value;
                std::string s = stream.str();

                return s;
            }

            /**
             * Returns a string of width from string
             * @param (string) value
             * @param (int) width
             * @return (string)
             */
            static std::string string_of(std::string value, int width) {
                std::string s = value;
                int length_input = value.length();
                if (length_input > width) s = s.substr(0, width);
                else {
                    while (s.length() != width) s += ' ';
                }

                return s;
            }

        private:
            std::string _value;
        };
    }
}

#endif /* STRING_HPP */

