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

#ifndef DENSITY_HPP
#define DENSITY_HPP

#include "object.hpp"
#include "type_recognizer.hpp"
#include "real_space_representation.hpp"
#include "array_container.hpp"
#include "reciprocal_space_representation.hpp"


namespace em {

    class DensityObject : public Object, public RealSpaceRepresentation {
    public:

        DensityObject();

        DensityObject(size_t columns, size_t rows, size_t sections);

        value_t density_at(Triplet<int> v) const override {
            return _container.data_at(v.first(), v.second(), v.third());
        };

        void set_density_at(Triplet<int> v, value_t value) override {
            _container.set_data_at(v.first(), v.second(), v.third(), value);
        };

        size_t number_of_voxels() const override {
            return _container.size();
        };

        std::unique_ptr<ReciprocalSpaceRepresentation> get_fourier() const override {

        };

        const Container& container() const override {
            return _container;
        };

        Container& container() override {
            return _container;
        };

        void set_container(const Container& c) override {
            if (c.container_type() == Container::Type::ARRAY) {
                _container = *(static_cast<const ArrayContainer*>(&c));
            } else {
                throw std::invalid_argument("The container type does not match the object type\n");
            }
        };


    private:
        ArrayContainer _container;


    };
}

#endif /* DENSITY_HPP */

