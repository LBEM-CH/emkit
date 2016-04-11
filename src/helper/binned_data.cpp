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

#include "binned_data.hpp"

#include "file.hpp"

using namespace em;

BinnedData::BinnedData(double min_range, double max_range, int bins)
{
    _min_range = min_range;
    _max_range = max_range;
    _bins = bins;
    _data = (double*) calloc(_bins, sizeof(double));
    _counts = (int*) calloc(_bins, sizeof(int));
}

void BinnedData::add_data_at(double data_point, double data_value)
{
    //Find the appropriate bin
    if ( data_point <= max_range() && data_point >= min_range() ) 
    {
        int bin = get_bin_number(data_point);
        if(bin != -1)
        {   
            _data[bin] += data_value;
            _counts[bin]++;
        }
    }
} 

void BinnedData::set_bin_sum(int bin, double sum)
{
    if(bin >=0 && bin < bins()) _data[bin] = sum;
    else std::cerr << "Warning: The bin provided exceeds limits.";
}

void BinnedData::set_bin_count(int bin, int count)
{
    if(bin >=0 && bin < bins()) _counts[bin] = count;
    else std::cerr << "Warning: The bin provided exceeds limits.";
}

void BinnedData::write_sum(std::string file) const
{
    write(file, false);
}

void BinnedData::write_average(std::string file) const
{
    write(file, true);
}

void BinnedData::write(std::string file, bool average) const
{
    em::File writeFile(file, em::File::OUTPUT);
    
    //Check for the existence of the file
    if(writeFile.exists())
    {
        std::cout << "WARNING: File.. " << file << " already exists. Overwriting!\n";
    }
    
    std::string output = "";
    output += "\n";
    if(average) output += "#Averaged ";
    else output += "#Summed ";
    output += "data in range (" + 
                std::to_string(min_range()) +
                ", " + std::to_string(max_range()) +
                ") spaced by " + std::to_string(spacing())+ ":\n\n";
    
    for(int bin=0; bin<bins(); bin++)
    {
        double data_point = min_range() + (bin)*spacing();
        double data;
        if(average) data = average_in(bin);
        else data = sum_in(bin);
        output += std::to_string(data_point) + "\t" + std::to_string(data) + "\n";
    }
    
    writeFile << output;
    writeFile.close();
}

std::string BinnedData::plot_sum() const
{
    return plot_profile(false);
}

std::string BinnedData::plot_average() const
{
    return plot_profile(true);
}

std::string BinnedData::plot_profile(bool average) const
{
    std::string output = "";
    output += "\n";
    if(average) output += "#Averaged ";
    else output += "#Summed ";
    output += "data in range (" + 
                std::to_string(min_range()) +
                ", " + std::to_string(max_range()) +
                ") spaced by " + std::to_string(spacing())+ ":\n\n";
    
    double max_value;
    if(average) 
    {
        max_value = max_averaged_value();
    }
    else 
    {
        max_value = max_summed_value();
    }
    
    double max_points = 100;
    double step_value = max_value/max_points;
    
    for(int bin=0; bin<bins(); bin++)
    {
        double data_point = min_range() + (bin)*spacing();
        
        double data;
        if(average) data = average_in(bin);
        else data = sum_in(bin);
        
        int steps = int(data/step_value);
        output += std::to_string(data_point) + "\t|";
        for(int s=0; s<steps; s++) output += '+';
        output += " (" + std::to_string(data) + ")";
        output +="\n";
    }
    
    return output;
}

double BinnedData::sum_at(double data_point) const
{   
    return sum_in(get_bin_number(data_point));      
}

double BinnedData::average_at(double data_point) const
{   
    return average_in(get_bin_number(data_point));       
}

double BinnedData::sum_in(int bin) const
{
    if(bin >= 0 && bin < bins())
        return _data[bin];
    else
        return -1.0;
}

double BinnedData::average_in(int bin) const
{
    if(bin >= 0 && bin < bins())
        if(_counts[bin] == 0) return 0.0;
        else return _data[bin]/_counts[bin];
    else
        return -1.0;
}

double BinnedData::min_range() const
{
    return _min_range;
}

double BinnedData::max_range() const
{
    return _max_range;
}

int BinnedData::bins() const
{
    return _bins;
}

double BinnedData::spacing() const
{
    return (max_range() - min_range())/(bins());
}

double BinnedData::max_summed_value() const
{
    double max = 0.0;
    for(int bin=0; bin<bins(); bin++)
    {
        if(sum_in(bin) >  max) max = sum_in(bin);
    }
    
    return max;  
}

double BinnedData::max_averaged_value() const
{
    double max = 0.0;
   for(int bin=0; bin<bins(); bin++)
    {
        if(average_in(bin) >  max) max = average_in(bin);
    }
    
    return max;
}

int BinnedData::get_bin_number(double data_point) const
{
    int bin = floor((data_point-min_range())/spacing());
    if( bin>=0 && bin<bins() )
        return bin;
    else
        return -1;
}