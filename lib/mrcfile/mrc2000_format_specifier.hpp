/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   mrc_format_specifier.hpp
 * Author: biyanin
 *
 * Created on April 18, 2016, 4:04 PM
 */

#ifndef MRC_FORMAT_SPECIFIER_HPP
#define MRC_FORMAT_SPECIFIER_HPP

#include "format_specifier.hpp"

namespace mrc {

    class MRC2000FormatSpecifier : public FormatSpecifier {
    public:

        virtual std::vector<std::string> header_fields() const override {
            std::vector<std::string> fields = {
                    "nx", "ny", "nz", "mode", "nxstart", "nystart", 
                    "nzstart", "mx", "my", "mz", "cella", "cellb", "cellc", 
                    "alpha", "beta", "gamma", "mapc", "mapr", "maps", 
                    "min", "max", "mean", "ispg", "nsymbt"};
            for (int i = 25; i <= 49; i++) fields.push_back("extra");
            fields.push_back("originx");
            fields.push_back("originy");
            fields.push_back("originz");
            fields.push_back("map");
            fields.push_back("stamp");
            fields.push_back("rms");
            return fields;
        };
    };
}

#endif /* MRC_FORMAT_SPECIFIER_HPP */

