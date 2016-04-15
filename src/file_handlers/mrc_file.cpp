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

#include <cassert>

#include "mrc_file.hpp"

#include "../helper/binary_file.hpp"
#include "../components/density.hpp"

using namespace em;

std::unique_ptr<Object> MRCFile::loadObject() {

    std::unique_ptr<Object> object;

    em::BinaryFile infile(_fileName, em::File::INPUT);

    if (!infile.exists()) {
        std::cerr << "File not found: " << _fileName << std::endl;
        exit(1);
    }

    if(!loadHeader(infile)) {std::cerr << "Error in loading header from file: " + _fileName; exit(1);}

    if (_mode == 2) {
        object = std::unique_ptr<DensityObject<float>>(new DensityObject<float>(_columns, _rows, _sections));
        object->properties().register_property("rows", std::to_string(_rows));
        object->properties().register_property("columns", std::to_string(_columns));
        object->properties().register_property("sections", std::to_string(_sections));
        object->properties().register_property("mx", std::to_string(_mx));
        object->properties().register_property("my", std::to_string(_my));
        object->properties().register_property("mz", std::to_string(_mz));
        object->properties().register_property("nxstart", std::to_string(_nxstart));
        object->properties().register_property("nystart", std::to_string(_nystart));
        object->properties().register_property("nzstart", std::to_string(_nzstart));
        object->properties().register_property("xlen", std::to_string(_xlen));
        object->properties().register_property("ylen", std::to_string(_ylen));
        object->properties().register_property("zlen", std::to_string(_zlen));
        object->properties().register_property("alpha", std::to_string(_alpha));
        object->properties().register_property("beta", std::to_string(_beta));
        object->properties().register_property("gamma", std::to_string(_gamma));
        object->properties().register_property("mapc", std::to_string(_mapc));
        object->properties().register_property("mapr", std::to_string(_mapr));
        object->properties().register_property("maps", std::to_string(_maps));

        auto c = ArrayContainer<float>(_columns, _rows, _sections);

        char* raw_data;

        if(!loadData(infile, raw_data)){ std::cerr << "Error in loading data from file: " + _fileName; exit(1);}

        c.set_data((float*) raw_data);

        delete[] raw_data;

        object->set_container(c);
    } else {
        std::cerr << "ERROR while reading MRC file:\n"
                << "\t" << _fileName << "\n"
                << "The data format (MRC mode:" << _mode << ") not supported!\n"
                << "HINT:\n Only MRC mode - 2 is supported.\n"
                << "If you don't have mode 2 please open the image in Chimera and store it as MRC.\n";
        exit(1);
    }

    infile.close();

    return object;


}

void MRCFile::saveObject(const std::unique_ptr<Object>& object) {

}


bool MRCFile::loadHeader(BinaryFile& infile) {
    std::cout << "Reading header..\n";

    infile.seekg(0, std::ios::beg);

    try {

        //Read the header
        _rows = infile.read_int();
        _columns = infile.read_int();
        _sections = infile.read_int();

        _mode = infile.read_int();

        _nxstart = infile.read_int();
        _nystart = infile.read_int();
        _nzstart = infile.read_int();

        _mx = infile.read_int();
        _my = infile.read_int();
        _mz = infile.read_int();

        _xlen = infile.read_float();
        _ylen = infile.read_float();
        _zlen = infile.read_float();

        _alpha = infile.read_float();
        _beta = infile.read_float();
        _gamma = infile.read_float();

        _mapc = infile.read_int();
        _mapr = infile.read_int();
        _maps = infile.read_int();

    } catch (const std::exception& e) {
        std::cerr << "Error in reading file header from:\n\t" << _fileName << std::endl;
        return false;
    }

    return true;

}

bool MRCFile::loadData(BinaryFile& infile, char* rawData) {

    size_t input_size = _rows * _columns*_sections;
    size_t file_size = infile.file_size();
    size_t memory_size = (int) (file_size - 1024) / 4;

    if (memory_size < input_size) {
        std::cerr << "ERROR while reading file:\n"
                << "\t" << _fileName << "\n"
                << "Error reading data, input data is less than expected.\n"
                << "\tExpected size: (" << _columns << "X" << _rows << "X" << _sections << ") " << input_size << "\n"
                << "\tData size in file: " << memory_size << "\n";
        return false;
    }

    infile.seekg(1024, std::ios::beg);

    if (rawData) delete[] rawData;
    rawData = new char[input_size * sizeof (int)];

    try {
        infile.read(rawData, input_size * sizeof (int));
    } catch (const std::exception& e) {
        std::cerr << "Error in reading file data from:\n\t" << _fileName << std::endl;
        return false;
    }

    return true;

}

bool MRCFile::saveHeader(BinaryFile& outfile) {

    int spcgrp = 1;
    int zero = 0;

    try {
        outfile.write((char*) &_columns, sizeof (int));
        outfile.write((char*) &_rows, sizeof (int));
        outfile.write((char*) &_sections, sizeof (int));
        outfile.write((char*) &_mode, sizeof (int));
        outfile.write((char*) &_nxstart, sizeof (int)); //nxstart
        outfile.write((char*) &_nystart, sizeof (int)); //nystart
        outfile.write((char*) &_nzstart, sizeof (int)); //nzstart
        outfile.write((char*) &_mx, sizeof (int)); //mx
        outfile.write((char*) &_my, sizeof (int)); //my
        outfile.write((char*) &_mz, sizeof (int)); //mz
        outfile.write((char*) &_xlen, sizeof (float));
        outfile.write((char*) &_ylen, sizeof (float));
        outfile.write((char*) &_zlen, sizeof (float));
        outfile.write((char*) &_alpha, sizeof (float));
        outfile.write((char*) &_beta, sizeof (float));
        outfile.write((char*) &_gamma, sizeof (float));
        outfile.write((char*) &_mapc, sizeof (int));
        outfile.write((char*) &_mapr, sizeof (int));
        outfile.write((char*) &_maps, sizeof (int));

        /*
         * TODO:
         * Write out correct min/ max/mean
         */
        outfile.write((char*) &zero, sizeof (float)); //min
        outfile.write((char*) &zero, sizeof (float)); //max
        outfile.write((char*) &zero, sizeof (float)); //mean
        outfile.write((char*) &spcgrp, sizeof (int));
        for (int i = 0; i < 233; i++) outfile.write((char*) &zero, sizeof (float));
        
    } catch (const std::exception& e) {
        std::cerr << "Error in writing file header to:\n\t" << _fileName << std::endl;
        return false;
    }

    return true;

}

bool MRCFile::saveData(BinaryFile& outfile, const char* data) {
    
    outfile.seekp(1024);
    try {
        outfile.write(data, _columns*_rows*_sections*sizeof(float));
    } catch (const std::exception& e) {
        std::cerr << "Error in writing file data to:\n\t" << _fileName << std::endl;
        return false;
    }

    return true;
    
}

