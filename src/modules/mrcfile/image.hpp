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
#include <memory>

#include "header.hpp"
#include "data.hpp"

#include "format_specifier.hpp"

namespace em {
    namespace mrc {

        class Image {
        public:

            Image() = default;

            Image(std::string file_name, std::string format)
            : file_name_(file_name) {
                format_ = select_specifier(format);
                header_ = Header(format_);
                data_ = Data(format_);

            };

            void load() {
                std::ifstream is(file_name_, std::ios::binary);
                if (!is.is_open()) {
                    throw std::runtime_error("Unable to open file: '" + file_name_ + "' Are you sure the file exists?\n");
                }
                if (!header_.load(is)) {
                    throw std::runtime_error("Unable to load header from file: " + file_name_);
                }
                if (!data_.load(is, header_.data_points(), header_.mode(), header_.should_swap_endianness())) {
                    throw std::runtime_error("Unable to load data from file: " + file_name_);
                }
            }

            void save() {
                if (header_.data_points() != data_.data_points()) {
                    throw std::runtime_error("The data points to be written in header and the actual present do not match");
                }

                //Overwrite the mode from data
                header_.set("mode", std::to_string(data_.mode()));

                //Overwrite the min and max values
                if (data_.mode() < 3) {
                    std::vector<float> v = data().get<float>();
                    float max = *std::max_element(v.begin(), v.end());
                    float min = *std::min_element(v.begin(), v.end());
                    float sum = std::accumulate(v.begin(), v.end(), 0.0);
                    float mean = sum / v.size();
                    float sq_sum = std::inner_product(v.begin(), v.end(), v.begin(), 0.0);
                    float stdev = std::sqrt(sq_sum / v.size() - mean * mean);
                    if (header().exists("min")) header().set("min", std::to_string(min));
                    if (header().exists("max")) header().set("max", std::to_string(max));
                    if (header().exists("mean")) header().set("mean", std::to_string(mean));
                    if (header().exists("rms")) header().set("rms", std::to_string(stdev));
                }

                //Overwrite the cell lengths
                if (header().exists("cella") && std::stod(header().get("cella")) < 1) header().set("cella", header().get("columns"));
                if (header().exists("cellb") && std::stod(header().get("cellb")) < 1) header().set("cellb", header().get("rows"));
                if (header().exists("cellc") && std::stod(header().get("cellc")) < 1) header().set("cellc", header().get("sections"));

                //Overwrite the mx, my, mz
                if (header().exists("mx") && std::stod(header().get("mx")) < 1) header().set("mx", header().get("columns"));
                if (header().exists("my") && std::stod(header().get("my")) < 1) header().set("my", header().get("rows"));
                if (header().exists("mz") && std::stod(header().get("mz")) < 1) header().set("mz", header().get("sections"));

                // TODO: Overwrite the stamp

                std::ofstream os(file_name_, std::ios::binary);
                if (!header_.save(os)) {
                    throw std::runtime_error("Unable to save header to file: " + file_name_);
                }
                if (!data_.save(os)) {
                    throw std::runtime_error("Unable to save data to file: " + file_name_);
                }
            }
            
            void clear() {
                header_.clear();
                data_.clear();
            }

            const Header& header() const {
                return header_;
            };

            Header& header() {
                return header_;
            };

            const Data& data() const {
                return data_;
            }

            Data& data() {
                return data_;
            }


        private:

            std::shared_ptr<FormatSpecifier> select_specifier(std::string format) {
                if (format == "mrc") return std::shared_ptr<FormatSpecifier>(new MRCFormatSpecifier());
                else if (format == "map") return std::shared_ptr<FormatSpecifier>(new MAPFormatSpecifier());
                else {
                    std::cerr << "WARNING: The specified format (" << format << ") is not a"
                            << " standard MRC format. Please choose from:"
                            << "\n\t mrc, map\n"
                            << "Trying to use the classical old MRC format.";
                    return std::shared_ptr<FormatSpecifier>(new FormatSpecifier());
                }

            };

            std::string file_name_;
            std::shared_ptr<FormatSpecifier> format_;
            Header header_;
            Data data_;
        };
    }
}

#endif /* IMAGE_HPP */

