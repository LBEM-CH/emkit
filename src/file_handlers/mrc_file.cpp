/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "mrc_file.hpp"

using namespace em;

VolumeHeader MRCFile::loadHeader() const 
{
    std::cout << "Reading header..\n";

    em::BinaryFile infile(_fileName, em::File::INPUT);

    //Check for the presence of file
    if (!infile.exists()) {
        std::cerr << "File not found: " << _fileName << std::endl;
        return false;
    }

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

        _alpha = infile.read_float();<
        _beta = infile.read_float();
        _gamma = infile.read_float();

        _mapc = infile.read_int();
        _mapr = infile.read_int();
        _maps = infile.read_int();

    } catch (const std::exception& e) {
        std::cerr << "Error in reading file header from:\n\t" << _fileName << std::endl;
        return false;
    }

    infile.close();
    
    

}