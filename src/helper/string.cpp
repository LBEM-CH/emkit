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

#include "string.hpp"

using namespace em;

String::String(const std::string& string) 
{
    from_std_string(string);
}

void String::from_std_string(const std::string& string)
{
    _value = string;
}

std::string String::to_std_string() const
{
    return _value;
}

std::string String::trim(const std::string& str)
{
    //Remove extra spaces
    std::string trimmed = "";
    bool spaced = false;

    for(const char& curr : str)
    {
        if(spaced && curr != ' ') spaced = false;
        if(!spaced) trimmed += curr;
        if(curr == ' ') spaced = true;
    }

    //Remove beginning and end white space if exists
    if(trimmed.back() == ' ') trimmed = trimmed.substr(0, trimmed.length()-1);
    if(trimmed[0] == ' ') trimmed = trimmed.substr(1, trimmed.length()-1);

    return trimmed;

}

String String::trim() const
{
    return String(trim(to_std_string()));
}

std::vector<std::string> String::split(const std::string &s, char delim) 
{
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) 
    {
        elems.push_back(item);
    }
    return elems;
}

std::vector<String> String::split(char delim) const
{
    std::vector<String> elems;
    std::stringstream ss(to_std_string());
    std::string item;
    while (std::getline(ss, item, delim)) 
    {
        elems.push_back(String(item));
    }
    return elems;
}

std::string String::string_of(double value, int width, int precision)
{
    std::stringstream stream;
    stream << std::fixed << std::setw(width) << std::setprecision(precision) << value;
    std::string s = stream.str();
    
    return s;
}

std::string String::string_of(int value, int width)
{
    std::stringstream stream;
    stream << std::fixed << std::setw(width) << value;
    std::string s = stream.str();
    
    return s;
}

std::string String::string_of(std::string value, int width)
{
    std::string s = value;
    int length_input = value.length();
    if(length_input > width) s = s.substr(0, width);
    else
    {
        while(s.length() != width) s += ' ';
    }
    
    return s;
}