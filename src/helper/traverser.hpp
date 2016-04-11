/* 
 * File:   traverser.hpp
 * Author: biyanin
 *
 * Created on March 18, 2016, 3:41 PM
 */

#ifndef ARRAY_TRAVERSER_HPP
#define	ARRAY_TRAVERSER_HPP

#include <tuple>

namespace em
{
    /**
     * An abstract interface to be used as the traverser for the
     * array class. The traverser provides methods to begin and 
     * end a traverse and to get to the next value of the array.
     * At any point in the traverse current triplet index, 
     * and value would be made available.
     */
    template<class T>
    class ArrayTraverser3D
    {
        /**
         * Marks the traverser to being traversing
         */
        virtual void begin();
        
        /**
         * Checks if the traverse is finished
         * @return (bool) finished traversing?
         */
        virtual bool finished();
        
        /**
         * Traverses to the next element
         */
        virtual void next();
        
        /**
         * Fetches the current index of the element traversing
         */
        virtual TripletIndex current_index();
        
        /**
         * Fetches the current value of the element traversing
         */
        virtual T current_value();
        
    };
}

#endif	/* TRAVERSER_HPP */

