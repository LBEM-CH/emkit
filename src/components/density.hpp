/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   density.hpp
 * Author: biyanin
 *
 * Created on April 15, 2016, 10:58 AM
 */

#ifndef DENSITY_HPP
#define DENSITY_HPP

#include "object.hpp"
#include "real_space_representation.hpp"
#include "array_container.hpp"
#include "reciprocal_space_representation.hpp"


namespace em
{
    template<class value_t>
    class DensityObject : public Object, public RealSpaceRepresentation<value_t> {
        
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
        
        std::unique_ptr<ReciprocalSpaceRepresentation<value_t>> get_fourier() const override {
            
        };



        

        
    private:
        ArrayContainer<value_t> _container;

    };
}

#endif /* DENSITY_HPP */

