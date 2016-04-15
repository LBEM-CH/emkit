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
 */

#ifndef MRC_FILE_HPP
#define MRC_FILE_HPP

#include <iostream>
#include <fstream>

#include "../helper/binary_file.hpp"

#include "file_handler.hpp"

namespace em {

    class MRCFile : public FileHandler {
    public:

        MRCFile(std::string fileName);


        std::unique_ptr<Object> loadObject() override;
        
        void saveObject(const std::unique_ptr<Object>& object) override;


    private:

        bool loadHeader(BinaryFile& infile);

        bool loadData(BinaryFile& infile, char* raw_data);
        
        bool saveHeader(BinaryFile& outfile);
        
        bool saveData(BinaryFile& outfile, const char* data);

        /**
         * Mode for the storage of data
         */
        int _mode;

        /**
         * Start locations
         */
        int _nxstart, _nystart, _nzstart;

        /**
         * Number of rows, columns and sections of the volume
         */
        int _rows, _columns, _sections;

        /**
         * Grid size.
         * The pixel/A is defined as xlen/mx, ylen/my, zlen/mz
         */
        int _mx, _my, _mz;

        /**
         * Axis corresponding to columns, rows, sections
         */
        int _mapc, _mapr, _maps;

        /**
         * Cell size
         * Example: 131.0, 131.0, 400.0
         */
        double _xlen, _ylen, _zlen;

        /**
         * Cell angle beta in degrees;
         * NOTE: alpha, beta are = 90 for 2d crystals
         * Example: 30
         * Default: 90
         */
        double _alpha, _beta, _gamma;

    };
}

#endif