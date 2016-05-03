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

#ifndef MAP_FORMAT_SPECIFIER_HPP
#define MAP_FORMAT_SPECIFIER_HPP

#include "format_specifier.hpp"
#include "header_property.hpp"

namespace mrc {

    class MAPFormatSpecifier : public FormatSpecifier {
    public:

        std::vector<HeaderProperty> header_properties() const override {
            std::vector<HeaderProperty> fields;
            fields.push_back(HeaderProperty("columns", new HeaderValue<int>(0)));
            fields.push_back(HeaderProperty("rows", new HeaderValue<int>(0)));
            fields.push_back(HeaderProperty("sections", new HeaderValue<int>(0)));
            fields.push_back(HeaderProperty("mode", new HeaderValue<int>(0)));
            fields.push_back(HeaderProperty("nxstart", new HeaderValue<int>(0)));
            fields.push_back(HeaderProperty("nystart", new HeaderValue<int>(0)));
            fields.push_back(HeaderProperty("nzstart", new HeaderValue<int>(0)));
            fields.push_back(HeaderProperty("mx", new HeaderValue<int>(0)));
            fields.push_back(HeaderProperty("my", new HeaderValue<int>(0)));
            fields.push_back(HeaderProperty("mz", new HeaderValue<int>(0)));
            fields.push_back(HeaderProperty("cella", new HeaderValue<float>(0.0)));
            fields.push_back(HeaderProperty("cellb", new HeaderValue<float>(0.0)));
            fields.push_back(HeaderProperty("cellc", new HeaderValue<float>(0.0)));
            fields.push_back(HeaderProperty("alpha", new HeaderValue<float>(90.0)));
            fields.push_back(HeaderProperty("beta", new HeaderValue<float>(90.0)));
            fields.push_back(HeaderProperty("gamma", new HeaderValue<float>(90.0)));
            fields.push_back(HeaderProperty("mapc", new HeaderValue<int>(1)));
            fields.push_back(HeaderProperty("mapr", new HeaderValue<int>(2)));
            fields.push_back(HeaderProperty("maps", new HeaderValue<int>(3)));
            fields.push_back(HeaderProperty("min", new HeaderValue<float>(0.0)));
            fields.push_back(HeaderProperty("max", new HeaderValue<float>(0.0)));
            fields.push_back(HeaderProperty("mean", new HeaderValue<float>(0.0)));
            fields.push_back(HeaderProperty("ispg", new HeaderValue<int>(0)));
            fields.push_back(HeaderProperty("nsymbt", new HeaderValue<int>(0)));
            fields.push_back(HeaderProperty("lskflg", new HeaderValue<float>(0.0)));
            fields.push_back(HeaderProperty("skwmat11", new HeaderValue<float>(0.0)));
            fields.push_back(HeaderProperty("skwmat21", new HeaderValue<float>(0.0)));
            fields.push_back(HeaderProperty("skwmat31", new HeaderValue<float>(0.0)));
            fields.push_back(HeaderProperty("skwmat12", new HeaderValue<float>(0.0)));
            fields.push_back(HeaderProperty("skwmat22", new HeaderValue<float>(0.0)));
            fields.push_back(HeaderProperty("skwmat32", new HeaderValue<float>(0.0)));
            fields.push_back(HeaderProperty("skwmat13", new HeaderValue<float>(0.0)));
            fields.push_back(HeaderProperty("skwmat23", new HeaderValue<float>(0.0)));
            fields.push_back(HeaderProperty("skwmat33", new HeaderValue<float>(0.0)));
            fields.push_back(HeaderProperty("skwtrn1", new HeaderValue<float>(0.0)));
            fields.push_back(HeaderProperty("skwtrn2", new HeaderValue<float>(0.0)));
            fields.push_back(HeaderProperty("skwtrn3", new HeaderValue<float>(0.0)));

            for (int i = 38; i <= 52; i++) fields.push_back(HeaderProperty("extra", new HeaderValue<float>(0.0)));
            fields.push_back(HeaderProperty("map", new HeaderValue<std::string>("    ")));
            fields.push_back(HeaderProperty("stamp", new HeaderValue<std::string>("    ")));
            fields.push_back(HeaderProperty("rms", new HeaderValue<float>(0.0)));
            return fields;

        };
    };
}


#endif /* MAP_FORMAT_SPECIFIER_HPP */

