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

#ifndef MRC_HEADER_PROPERTY_HPP
#define MRC_HEADER_PROPERTY_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <type_traits>

#include "header_value.hpp"

namespace em {
    namespace mrc {

        class HeaderProperty {
        public:

            HeaderProperty(const std::string& identifier, HeaderValueInterface* value)
            : identifier_(identifier), value_(std::unique_ptr<HeaderValueInterface>(value)) {
            };

            bool operator!=(const std::string& right) const {
                bool result = !(*this == right); // Reuse equals operator
                return result;
            }

            bool operator==(const std::string& right) const {
                return (identifier_ == right);
            }

            friend inline std::ostream& operator<<(std::ostream& os, const HeaderProperty& prop) {
                os << prop.value_->to_string();
                return os;
            }

            std::string value() const {
                return value_->to_string();
            };

            bool write_value(std::ofstream& ofs) const {
                return value_->to_ofstream(ofs);
            }

            bool set_value(const std::string& str) {
                return value_->from_string(str);
            }

            bool set_value(std::ifstream& ifs) {
                return value_->from_ifstream(ifs);
            }

            const std::string& identifier() const {
                return identifier_;
            }

            std::string& identifier() {
                return identifier_;
            };

        private:
            std::string identifier_;
            std::unique_ptr<HeaderValueInterface> value_;
        };
    }
}

#endif /* OBJECTPROPERTY_HPP */

