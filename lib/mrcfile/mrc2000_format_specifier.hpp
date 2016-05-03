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

#ifndef MRC_FORMAT_SPECIFIER_HPP
#define MRC_FORMAT_SPECIFIER_HPP

#include "format_specifier.hpp"

namespace mrc {

    class MRC2000FormatSpecifier : public FormatSpecifier {
    public:

        virtual std::vector<HeaderProperty> header_properties() const override {
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
            
            for (int i = 25; i <= 49; i++) fields.push_back(HeaderProperty("extra", new HeaderValue<float>(0.0)));
            fields.push_back(HeaderProperty("originx", new HeaderValue<int>(0)));
            fields.push_back(HeaderProperty("originy", new HeaderValue<int>(0)));
            fields.push_back(HeaderProperty("originz", new HeaderValue<int>(0)));
            fields.push_back(HeaderProperty("map", new HeaderValue<std::string>("    ")));
            fields.push_back(HeaderProperty("stamp", new HeaderValue<std::string>("    ")));
            fields.push_back(HeaderProperty("rms", new HeaderValue<float>(0.0)));
            return fields;
        };
    };
}

#endif /* MRC_FORMAT_SPECIFIER_HPP */

