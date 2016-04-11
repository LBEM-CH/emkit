

#ifndef VOXEL_HPP
#define	VOXEL_HPP

#include <iostream>

#include "tripet.hpp"

namespace em
{
    class TripletIndex : Triplet<size_t>
    {
    public:
        
        TripletIndex()
            : Triplet<size_t>()
        {   
        };
        
        TripletIndex(size_t x, size_t y, size_t z)
            : Triplet<size_t>(x, y, z)
        {
        }
        
        void reset(size_t x, size_t y, size_t z)
        {
            Triplet<size_t>::reset(x,y,z);
        }
        
        size_t x()
        {
            return Triplet<size_t>::first();
        }
        
        size_t y()
        {
            return Triplet<size_t>::second();
        }
        
        size_t z()
        {
            return Triplet<size_t>::third();
        }
    
    };
}


#endif	/* VOXEL_HPP */

