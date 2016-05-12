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

#ifndef BYTE_SWAPPER_HPP
#define BYTE_SWAPPER_HPP

namespace em {

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
}

#endif /* BYTE_SWAPPER_HPP */

