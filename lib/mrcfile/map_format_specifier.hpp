
#ifndef MAP_FORMAT_SPECIFIER_HPP
#define MAP_FORMAT_SPECIFIER_HPP

#include "format_specifier.hpp"


namespace mrc {

    class MAPFormatSpecifier : public FormatSpecifier {
    public:

        std::vector<std::string> header_fields() const override {
            std::vector<std::string> fields = {
                    "nx", "ny", "nz", "mode", "nxstart", "nystart", 
                    "nzstart", "mx", "my", "mz", "cella", "cellb", "cellc", 
                    "alpha", "beta", "gamma", "mapc", "mapr", "maps", 
                    "min", "max", "mean", "ispg", "nsymbt", "lskflg",
                    "skwmat11", "skwmat21", "skwmat31", "skwmat12", "skwmat22",
                    "skwmat32", "skwmat13", "skwmat23", "skwmat33",
                    "skwtrn1", "skwtrn2", "skwtrn3" };
            for (int i = 38; i <= 52; i++) fields.push_back("extra");
            fields.push_back("map");
            fields.push_back("stamp");
            fields.push_back("rms");
            return fields;
        };
    };
}


#endif /* MAP_FORMAT_SPECIFIER_HPP */

