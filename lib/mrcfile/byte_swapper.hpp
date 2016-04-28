/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   byte_swapper.hpp
 * Author: biyanin
 *
 * Created on April 18, 2016, 4:41 PM
 */

#ifndef BYTE_SWAPPER_HPP
#define BYTE_SWAPPER_HPP

namespace mrc {

    class ByteSwapper {
    public:
        static void byte_swap(void *data, int size) {
            if (size == 1) return;
            for (int i = 0; i < size / 2; i++) {
                char temp = ((char*) data)[i];
                ((char*) data)[i] = ((char*) data)[size - i - 1];
                ((char*) data)[size - i - 1] = temp;
            }
        };
    };
}

#endif /* BYTE_SWAPPER_HPP */

