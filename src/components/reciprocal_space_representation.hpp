/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   reciprocal_space_representation.hpp
 * Author: biyanin
 *
 * Created on April 14, 2016, 5:46 PM
 */

#ifndef RECIPROCAL_SPACE_REPRESENTATION_HPP
#define RECIPROCAL_SPACE_REPRESENTATION_HPP

#include "representation.hpp"
#include "../helper/miller_index.hpp"


namespace em {
    
    template<class value_t>
    class RealSpaceRepresentation;
    
    template<class T>
    class ReciprocalSpaceRepresentation : public Representation {
        
    public:
        virtual T amplitude_at(const MillerIndex& index) const = 0;
        
        virtual T phase_at(const MillerIndex& index) const = 0;
        
        virtual void set_amplitude_at(const MillerIndex& index, T value) = 0;
        
        virtual void set_phase_at(const MillerIndex& index, T value) = 0;
        
        virtual bool exists(const MillerIndex& index) const = 0;
        
        virtual size_t number_Of_reflections() const = 0;
        
        virtual MillerIndex max_allowed_index() const = 0;
        
        virtual MillerIndex min_allowed_index() const = 0;
            
    };
    
}

#endif /* RECIPROCAL_SPACE_REPRESENTATION_HPP */

