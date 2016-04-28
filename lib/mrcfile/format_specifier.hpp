
#ifndef FORMAT_SPECIFIER_HPP
#define FORMAT_SPECIFIER_HPP

#include <string>
#include <vector>

namespace mrc {

    class FormatSpecifier {
    public:

        virtual int header_offset() const {
            return 0;
        };
        
        virtual int header_length() const {
            return 1024;
        };

        virtual int data_offset() const {
            return 1024;
        };
        
        virtual int data_byte_size(int mode) const {
            int byte_size;
            if (mode == 0) byte_size = 1;
            else if (mode == 1) byte_size = 2;
            else if (mode == 2) byte_size = 4;
            else if (mode == 3) byte_size = 2;
            else if (mode == 4) byte_size = 4;
            else {
                std::cerr << "The MRC mode of image  (mode = " << mode << ") is not a supported MRC mode.\n";
                exit(1);
            }
            
            return byte_size;
        };

        virtual std::vector<std::string> header_fields() const {
            return {
                    "nx", "ny", "nz", "mode", "nxstart", "nystart", 
                    "nzstart", "mx", "my", "mz", "cella", "cellb", "cellc", 
                    "alpha", "beta", "gamma", "mapc", "mapr", "maps", 
                    "min", "max", "mean", "ispg", "nsymbt"};
        };
    };
}

#endif /* FORMAT_SPECIFIER_HPP */

