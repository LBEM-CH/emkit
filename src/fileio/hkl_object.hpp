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

#ifndef HKL_FILE_ADAPTOR_HPP
#define HKL_FILE_ADAPTOR_HPP

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "../elements/complex.hpp"


namespace em {

    namespace object {

        class HKLTypeFile {
        public:

            enum class HKLFileColumnType {
                INDEX,
                RECIPROCAL_DISTANCE,
                AMPLITUDE,
                PHASE,
                FOM,
                SIG_PHASE,
                SIG_AMPLITUDE
            };

            HKLTypeFile(std::string file_name, std::vector<HKLFileColumnType> columns)
            : file_name_(file_name) {
                initiliaze();

            }

            


        private:

            void initiliaze() {
                std::ifstream infile(file_name_);

                std::string sLine;
                number_of_columns_ = 0;
                header_lines_ = -1;

                int cols_current = 0;
                int cols_previous = 0;

                if (infile.good()) {
                    //Check for the same number of columns in two consecutive rows
                    while (!infile.eof()) {
                        std::getline(infile, sLine);
                        std::stringstream is(sLine);

                        float temp;
                        cols_previous = cols_current;
                        cols_current = 0;
                        while (is >> temp) {
                            cols_current++;
                        }
                        if (cols_current == cols_previous) {
                            number_of_columns_ = cols_current;
                            break;
                        }
                        header_lines_++;
                    }
                }

                infile.close();
            }

            std::string file_name_;
            int header_lines_;
            int number_of_columns_;
            std::vector<HKLFileColumnType> columns_;

        };
    }
}

#endif