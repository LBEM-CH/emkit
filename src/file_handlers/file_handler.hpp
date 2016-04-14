/* 
 * File:   EMFile.hpp
 * Author: biyanin
 *
 * Created on March 23, 2016, 3:31 PM
 */

#ifndef EMFILE_BASE_HPP
#define	EMFILE_BASE_HPP

#include <iostream>
#include <string>
#include <memory>

#include "../components/object_type.hpp"

namespace em
{
    class ObjectBase;
    
    /**
     * A generalized (abstract) EMFile class which has factory methods to load
     * and save file which occur in Electron Microscopy. The subclasses should
     * specialize the behavior of the protected member object
     * and should override the following methods:
     * 
     * loadHeader, loadData, saveHeader, saveData
     */
    class FileHandler
    {
    public:
        
        FileHandler(std::string fileName) : _fileName (fileName){};
        
        /***********************************
         * FACTORY METHODS
         ***********************************/
        
        /**
         * FACTORY CREATOR of the correct EMFile using the format.
         * @param[in]   file_name   name with path of the file
         * @param[in]   format      (optional) format of the file, if not provided
         *                          the extension of the file will be used to
         *                          detect the format.
         * @return  The correct EMFile instance 
         */
        static std::unique_ptr<FileHandler> create(std::string file_name, std::string format="");
        
        /***********************************
         * TEMPLATE METHODS
         ***********************************/
        
        /**
         * Loads an object from the file of the specified type. Internally first
         * loads an object and then converts it to the type specified, produces
         * an error if the conversion is not possible
         * @return  The object of specified type
         */
        virtual std::unique_ptr<ObjectBase> loadObject() = 0;
        
        /**
         * Saves the given object to the file. Internally first converts the 
         * given object to the compatible type and then saves it. It will produce
         * run time error if the conversion is not possible.
         * @param[in]   object  the object which is to be saved.
         */
        virtual void saveObject(const std::unique_ptr<ObjectBase>& object) = 0;
        
        
    protected:
        
        std::string _fileName;
        
    };
}

#endif	/* EMFILE_HPP */

