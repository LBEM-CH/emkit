/* 
 * Author: Nikhil Biyani - nikhil(dot)biyani(at)gmail(dot)com
 *
 * This file is a part of 2dx.
 * 
 * 2dx is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or any 
 * later version.
 * 
 * 2dx is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public 
 * License for more details <http://www.gnu.org/licenses/>.
 */

#include "binary_file.hpp"

using namespace em;

BinaryFile::BinaryFile(const std::string& file_name, openmode mode) 
    : File(file_name, mode | std::ios_base::binary)
{

}

std::string BinaryFile::read_string(const int& read_size)
{
    char* temp = new char[read_size];
    this->read(temp, read_size*sizeof(char));
    
    std::string value = std::string(temp);
    return value;
}

int BinaryFile::read_int()
{
    int value;
    this->read((char*)&value, sizeof(int));
    
    return value;
}

float BinaryFile::read_float()
{
    float value;
    this->read((char*)&value, sizeof(float));
    
    return value;
}