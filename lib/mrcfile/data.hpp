
#ifndef MRC_DATA_HPP_PQVG38
#define MRC_DATA_HPP_PQVG38

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "format_specifier.hpp"
#include "byte_swapper.hpp"


namespace mrc {

    /**
     * @brief           A class to handle the data of MRC type files.
     * @description     The raw data is stored as a vector of char. The
     *                  data can be requested/set in any of the C++ standard 
     *                  types and internally the char vector is first 
     *                  reinterpreted to the correct type using which was
     *                  expected from file and then finally to the requested 
     *                  type.
     * @author          Nikhil Biyani - nikhil(dot)biyani(at)gmail(dot)com
     */
    class Data {
    public:

        Data()
        : _format(FormatSpecifier()), _mode(2){
        };
        
        /**
         * @brief       Initialize the data specifying some underlying variables
         * @param   
         */
        Data(FormatSpecifier format)
        : _format(format), _mode(2){
        };

        /**
         * @brief           Load the data from the file and swap endianness
         * @description     The file is 
         *                  read and the data is internally set. The read data
         *                  can be requested in any format using the other
         *                  functions. If it is marked that the endianess is to
         *                  be swapped then also does that!
         * @return          Success of read from the file
         */
        bool load(std::ifstream& is, size_t data_points, int mode, bool swap_endianness) {
            _mode = mode;
            is.seekg(_format.data_offset(), is.beg);
            _data.clear();
            _data = std::vector<char>(data_points * byte_size());
            is.read(_data.data(), data_points * byte_size());
            
            if(swap_endianness) {
                for(int i=0; i<data_points; ++i) {
                    ByteSwapper::byte_swap(&_data[i* byte_size()], byte_size());
                }
            }
            
            if (is) {
                return true;
            }
            else {
                return false;
            }
        };

        /**
         * @brief           Saves the data to the file
         * @description     The file (filename provided in the constructor) is 
         *                  saved with the data is internally set. The data
         *                  can be changed using other functions.
         * @return          Success of save to the file
         */
        bool save(std::ofstream& os) {
            
            os.seekp(_format.data_offset(), os.beg);
            os.write(_data.data(), _data.size());
            
            if(os) return true;
            else {
                return false;
            }
        };

        /**
         * @brief           Request the vector of stored data
         * @description     The data is first converted to the inherit type 
         *                  using the size of byte for each data point indicated
         *                  in the constructor and then converted to the
         *                  requested type
         * @return          vector of the stored data
         */
        template<typename value_type>
        std::vector<value_type> get() const {
            size_t _points = data_points();
            if (byte_size() == 1) {
                const int8_t* data = reinterpret_cast<const int8_t*> (_data.data());
                return std::vector<value_type>(data, data + _points);
            } else if (byte_size() == 2) {
                const int16_t* data = reinterpret_cast<const int16_t*> (_data.data());
                return std::vector<value_type>(data, data + _points);
            } else if (byte_size() == 4) {
                const float* data = reinterpret_cast<const float*> (_data.data());
                return std::vector<value_type>(data, data + _points);
            } else {
                std::cerr << "Unidentified byte size " << byte_size() << " encountered. Possible (1, 2, 4)\n";
            }
        }

        /**
         * @brief           Set the data from a vector.
         * @description     The vector is first converted to the inherent type
         *                  using the byte size provided in the constructor
         *                  and then finally stored as a char vector
         * @param       data    vector of the data
         */
        template<typename value_type>
        void set(const std::vector<value_type>& data, int mode) {
            _mode = mode;
            _data.clear();
            size_t _points = data.size();
            int _byte_size = byte_size(); 
            if (byte_size() == 1) {
                auto corrected = std::vector<int8_t>(data.begin(), data.end());
                _data = std::vector<char>((char*) corrected.data(), (char*) corrected.data() + _points * _byte_size);
            } else if (byte_size() == 2) {
                auto corrected = std::vector<int16_t>(data.begin(), data.end());
                _data = std::vector<char>((char*) corrected.data(), (char*) corrected.data() + _points * _byte_size);
            } else if (byte_size() == 4) {
                auto corrected = std::vector<float>(data.begin(), data.end());
                _data = std::vector<char>((char*) corrected.data(), (char*) corrected.data() + _points * _byte_size);
            } else {
                std::cerr << "Unidentified byte size " << byte_size() << " encountered. Possible (1, 2, 4)\n";
            }
        }
        
        size_t data_points() const {
            return _data.size()/byte_size();
        }
        
        int mode() {
            return _mode;
        }

    private:
        
        int byte_size() const {
            return _format.data_byte_size(_mode);
        };
        
        int _mode;
        FormatSpecifier _format;
        std::vector<char> _data;
    };


}


#endif
