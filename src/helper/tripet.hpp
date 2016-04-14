/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   tripet.hpp
 * Author: biyanin
 *
 * Created on April 8, 2016, 2:24 PM
 */

#ifndef TRIPET_HPP
#define TRIPET_HPP

#include <iostream>
#include <tuple>

namespace em
{
    template<class T>
    class Triplet
    {
    public:
        
        Triplet() = default;
        
        Triplet(const T& first, const T& second, const T& third)
        {
            reset(first, second, third);
        };
        
        void reset(const T& first, const T& second, const T& third)
        {
            _tuple = std::make_tuple(first, second, third);
        };
        
        void setFirst(const T& value)
        {
           std::get<0>(_tuple) = value; 
        };
        
        void setSecond(const T& value)
        {
            std::get<1>(_tuple) = value; 
        };
        
        void setThird(const T& value)
        {
            std::get<2>(_tuple) = value; 
        };
        
        T first() const
        {
            return std::get<0>(_tuple);
        };
        
        T second() const
        {
            return std::get<1>(_tuple);
        };
        
        T third() const
        {
            return std::get<2>(_tuple); 
        };
        
    private:
        std::tuple<T, T, T> _tuple;   
    };
    
    
}

#endif /* TRIPET_HPP */

