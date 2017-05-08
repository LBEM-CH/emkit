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
 * 
 */

#ifndef MRC_FILE_ADAPTOR_HPP
#define MRC_FILE_ADAPTOR_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

#include "../modules/mrcfile/image.hpp"
#include "../elements/file.hpp"
#include "../algorithm/fourier_transform.hpp"
#include "../objects/object_base_types.hpp"

namespace em {

    namespace fileio {

        class MRCFile {
        public:

            MRCFile(std::string file_name = "", std::string format = "")
            : file_name_(file_name), format_(format) {
            };
            
            template<typename ObjectType_>
            typename std::enable_if<!object::is_real_valued<typename object::object_traits<ObjectType_>::data_type>::value, bool>::type
            load(ObjectType_& object, element::PropertiesMap& header_values) {
                std::cerr << "MRC type files can read only real valued objects\n";
                return false;
            }
            
            template<typename ObjectType_>
            typename std::enable_if<object::is_real_valued<typename object::object_traits<ObjectType_>::data_type>::value, bool>::type
            load(ObjectType_& object, element::PropertiesMap& header_values) {
                
                using object_type = ObjectType_;
                static const size_t rank_ = object::object_traits<ObjectType_>::rank;
                using index_type = typename object::object_traits<ObjectType_>::index_type;
                using data_type = typename object::object_traits<ObjectType_>::data_type;
                
                static_assert((rank_ == 2 || rank_ == 3), "To read a MRC type file, the rank of the object should be either 2 or 3 ");
                
                mrc::Image mrc_image_ = get_mrc_image();
                mrc_image_.load();
                int mode = mrc_image_.data().mode();
                int columns = std::stoi(mrc_image_.header().get("columns"));
                int rows = std::stoi(mrc_image_.header().get("rows"));
                int sections = std::stoi(mrc_image_.header().get("sections"));

                if (mode == 0 || mode == 1 || mode == 2) {
                    index_type range;
                    range[0] = columns;
                    range[1] = rows;
                    if (rank_ == 3) range[2] = sections;
                    object = object_type(range, mrc_image_.data().get<data_type>());

                } else if (mode == 3 || mode == 4) {
                    //THIS PART IS NOT TESTED
                    std::cerr << "\n\n\nWARNING: Reading mode 3/4 from MRC files is not tested. Use at your own risk.\n\n\n";
                    std::vector<data_type> raw_data = mrc_image_.data().get<data_type>();

                    //Develop a complex container
                    index_type range;
                    range[0] = 2*columns;
                    range[1] = rows;
                    if (rank_ == 3) range[2] = sections;

                    index_type center;
                    if (rank_ == 2) center = {0, rows / 2};
                    else center = {0, rows / 2, sections / 2};

                    ComplexHalfObject<data_type, rank_> complex_object(range, center);

                    int i = 0;
                    for (auto& pair : complex_object) {
                        complex_object[pair.index()] = element::Complex<data_type>(raw_data[i++], raw_data[i++]);
                    }
                    complex_object.transform_origin({0, 0, 0});

                    //FFT
                    fourier_transform(range, complex_object, object);

                } else {
                    std::cerr << "ERROR while reading MRC file:\n"
                            << "The data format (MRC mode:" << mode << ") not supported!\n"
                            << "HINT:\n Only MRC mode 0-4 is supported.\n";
                    return false;
                }

                //Copy the current properties
                for (const auto& prop : mrc_image_.header().get_all()) {
                    header_values.register_property(prop.first, prop.second);
                }
                
                mrc_image_.clear();

                return true;
            }
            

            template<typename ObjectType_>
            typename std::enable_if<!object::is_real_valued<typename object::object_traits<ObjectType_>::data_type>::value, bool>::type
            save(const ObjectType_& obj, const element::PropertiesMap& properties) {
                std::cerr << "Error! MRC type files can be used to write only real valued objects\n";
                return false;
            }
            
            template<typename ObjectType_>
            typename std::enable_if<object::is_real_valued<typename object::object_traits<ObjectType_>::data_type>::value, bool>::type
            save(const ObjectType_& object, const element::PropertiesMap& properties) {
                using object_type = ObjectType_;
                static const size_t rank_ = object::object_traits<ObjectType_>::rank;
                using index_type = typename object::object_traits<ObjectType_>::index_type;
                using data_type = typename object::object_traits<ObjectType_>::data_type;
                
                static_assert((rank_ == 2 || rank_ == 3), "To read a MRC type file, the rank of the object should be either 2 or 3 ");
                
                mrc::Image mrc_image_ = get_mrc_image();

                //Copy the current properties
                for (const auto& prop : properties) {
                    mrc_image_.header().set(prop.first, prop.second);
                }

                //Change to the current sizes
                //std::cout << object.range() << "\n";
                mrc_image_.header().set("columns", std::to_string(object.range()[0]));
                mrc_image_.header().set("rows", std::to_string(object.range()[1]));
                if (rank_ == 3) mrc_image_.header().set("sections", std::to_string(object.range()[2]));
                else mrc_image_.header().set("sections", "1");

                //Copy the data
                mrc_image_.data().set(object.vectorize(), 2);

                //Save to the file
                mrc_image_.save();

                return true;
            };
            
            
            const std::string& file_name() const {
                return file_name_;
            }

            std::string& file_name() {
                return file_name_;
            }

        protected:

            virtual mrc::Image get_mrc_image() const {
                if (file_name() == "") {
                    std::cerr << "ERROR: File name is not set.\n";
                    exit(1);
                }
                std::string format = format_;
                if (format == "") {
                    format = element::File(file_name()).extension();
                }
                return mrc::Image(file_name(), format);
            }

            std::string file_name_;
            std::string format_;
        };
    }
}

#endif