#ifndef MRC_FILE_HPP
#define MRC_FILE_HPP

#include "file_handler.hpp"
#include "../helper/binary_file.hpp"
#include "../components/volume.hpp"
#include "../components/volume_data.hpp"

namespace em
{
    class MRCFile : public FileHandler
    {
    public:
        MRCFile(std::string fileName)
            : FileHandler(fileName)
        {
        }
        
        /**
         * Loads the MRC Header
         * @return 
         */
        bool loadHeader() const;
        
        
        bool loadData() const
        {
            if(_mode != 2)
            {
                    std::cerr << "ERROR while reading MRC file:\n"
                              << "\t" << _fileName << "\n"
                              << "The data format (MRC mode:" << _mode << ") not supported!\n"
                              << "HINT:\n Only MRC mode - 2 is supported.\n"
                              << "If you don't have mode 2 please open the image in Chimera and store it as MRC.\n";
                    exit(1);
            }

            em::BinaryFile infile(_fileName, em::File::INPUT);

            //Check for the presence of file
            if (!infile.exists()){
                std::cerr << "File not found: " << _fileName << std::endl;
                exit(1);
            }

            size_t input_size = _rows*_columns*_sections;
            size_t file_size = infile.file_size();
            size_t memory_size = (int)(file_size - 1024)/4;

            if(memory_size < input_size)
            {
                std::cerr << "ERROR while reading file:\n"
                          << "\t" << _fileName << "\n"
                          << "Error reading data, input data is less than expected.\n"
                          << "\tExpected size: (" << _columns << "X" << _rows << "X" << _sections << ") " << input_size <<"\n"
                          << "\tData size in file: " << memory_size << "\n";       
                exit(1);
            }

            infile.seekg (1024, std::ios::beg);

            char* rawData = new char[input_size*sizeof(int)];
            infile.read(rawData, input_size*sizeof(int));

            Array3D<double> array(_columns, _rows, _sections);
            array.set_data((double*)(float*) rawData);
            _data = VolumeData(_columns, _rows, _sections, VolumeData::VolumeType::REAL);
            _data.setData(array, VolumeData::VolumeType::REAL);
            
            delete[] rawData;

        };

        
        
    private:
        
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
        
        VolumeData _data;
        
    };
}

#endif