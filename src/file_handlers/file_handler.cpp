#include <stdexcept>
#include <string>

#include "file_handler.hpp"

#include "../helper/file.hpp"
#include "../components/object_base.hpp"
#include "../components/object_type.hpp"
#include "../components/object_factory.hpp"

#include "mrc_file_handler.hpp"

using namespace em;

std::unique_ptr<FileHandler> FileHandler::create(std::string file_name, std::string format)
{
    if(format == "")
    {
        File file(file_name);
        format = file.extension();
    }
    
    if(format == "MRC" || format == "mrc") return std::unique_ptr<MRCFile>(new MRCFile(file_name));
    //else if(format == "MAP" || format == "map") return new MAPFile(file_name);
    //else if(format == "HKL" || format == "hkl") return new HKLFile(file_name);
    //else if(format == "HKZ" || format == "hkz") return new HKZFile(file_name);
    //else if(format == "MTZ" || format == "mtz") return new MTZFile(file_name);
    else
    {
        std::cerr << "The file format not recognized.\n";
    }
}

std::unique_ptr<ObjectBase> FileHandler::loadObject(ObjectType type)
{
    if(!loadHeader() || !loadData())
    {
        throw std::runtime_error("Was not able to load the file.");
    }
    
    return _object->convertTo(type);
}


void FileHandler::saveObject(const std::unique_ptr<ObjectBase>& object) 
{
    _object = object->convertTo(_object->objectType());
    
    if(!saveHeader() || !saveData())
    {
        throw std::runtime_error("Was not able to save the file.");
    }
}


