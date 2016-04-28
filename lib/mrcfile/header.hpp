
#ifndef HEADER_HPP_HC343PL
#define HEADER_HPP_HC343PL

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include "format_specifier.hpp"
#include "byte_swapper.hpp"

namespace mrc {

    /**
     * MRC Header class
     */
    class Header {
    public:

        Header()
        : _format(FormatSpecifier()),  _swap_byte_order(false) {
        };
        
        Header(FormatSpecifier format)
        : _format(format), _swap_byte_order(false) {
            _fields = _format.header_fields();
            if (std::find(_fields.begin(), _fields.end(), "mode") == _fields.end()
                    || std::find(_fields.begin(), _fields.end(), "columns") == _fields.end()
                    || std::find(_fields.begin(), _fields.end(), "rows") == _fields.end()
                    || std::find(_fields.begin(), _fields.end(), "sections") == _fields.end()) {
                std::cerr << "ERROR: MRC Format specifier error:\n"
                        << "mode/columns/rows/sections fields are required in format specification.\n";
                exit(1);
            }
            
            //Set the default mode to 2
            set("mode", 2);
        };

        bool load(std::ifstream& is) {
            is.seekg(_format.header_offset(), is.beg);
            _values.clear();
            _values.resize(_fields.size());
            is.read((char*) _values.data(), _values.size() * sizeof (float));

            int bitMask = (int) get("mapc") | (int) get("mapr") | (int) get("maps");
            if (bitMask == 3 || bitMask == 0) _swap_byte_order = false;
            else _swap_byte_order = true;

            if (_swap_byte_order) {
                for (int i = 0; i < 256; i++) ByteSwapper::byte_swap(&_values[i], sizeof(float));
            }

            if (is) {
                return true;
            }
            else return false;
        }

        bool save(std::ofstream& os) {
            os.seekp(_format.header_offset(), os.beg);
            os.write((char*) _values.data(), _values.size() * sizeof (float));

            if (os) return true;
            else return false;
        }

        int mode() {
            return (int) get("mode");
        };
        
        size_t data_points() {
            return (size_t) (get("columns") * get("rows") * get("sections"));
        }

        bool should_swap_endianness() {
            return _swap_byte_order;
        }

        float get(std::string field) const {
            auto found_field = std::find(_fields.begin(), _fields.end(), field);
            if (found_field == _fields.end()) {
                std::cerr << "Warning: The requested field (" << field << ") not found in header specification returning 0\n";
                std::cerr << "Following are the allowed values:\n";
                for (auto f : _fields) std::cerr << f << std::endl;
                return 0.0;
            } else {
                return _values[std::distance(_fields.begin(), found_field)];
            }
        };
        
        std::map<std::string, float> get_all() const {
            std::map<std::string, float> values_map;
            for(auto f : _fields) {
                values_map.emplace(f, get(f));
            }
            return values_map;
        }

        void set(std::string field, float value) {
            auto found_field = std::find(_fields.begin(), _fields.end(), field);
            if (found_field == _fields.end()) {
                std::cerr << "Warning: The requested field (" << field << ") not found in header specification, doing nothing\n";
                std::cerr << "Following are the allowed values:\n";
                for (auto f : _fields) std::cerr << f << std::endl;
            } else {
                _values[std::distance(_fields.begin(), found_field)] = value;
            }
        };

    private:

        FormatSpecifier _format;
        bool _swap_byte_order;
        std::vector<std::string> _fields;
        std::vector<float> _values;
    };
}

#endif /* MRC_HEADER_HPP */

