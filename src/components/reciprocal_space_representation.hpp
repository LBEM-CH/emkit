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

#ifndef RECIPROCAL_SPACE_REPRESENTATION_HPP
#define RECIPROCAL_SPACE_REPRESENTATION_HPP

#include <iostream>
#include <memory>

#include "representation.hpp"
#include "type_recognizer.hpp"
#include "../helper/miller_index.hpp"

namespace em {

    class RealSpaceRepresentation;

    class ReciprocalSpaceRepresentation : public Representation {
    public:
        virtual value_t amplitude_at(const MillerIndex& index) const = 0;

        virtual value_t phase_at(const MillerIndex& index) const = 0;

        virtual void set_amplitude_at(const MillerIndex& index, value_t value) = 0;

        virtual void set_phase_at(const MillerIndex& index, value_t value) = 0;

        virtual bool exists(const MillerIndex& index) const = 0;

        virtual size_t number_Of_reflections() const = 0;

        virtual MillerIndex max_allowed_index() const = 0;

        virtual MillerIndex min_allowed_index() const = 0;

        virtual std::unique_ptr<RealSpaceRepresentation> get_real() const = 0;
    };

}

#endif /* RECIPROCAL_SPACE_REPRESENTATION_HPP */

