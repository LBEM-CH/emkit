/* 
 * Author: Nikhil Biyani - nikhil(dot)biyani(at)gmail(dot)com
 *
 * This file is a part of 2dx.
 * 
 * 2dx is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or any 
 * later version.
 * 
 * 2dx is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public 
 * License for more details <http://www.gnu.org/licenses/>.
 */

#include "miller_index.hpp"


using namespace em;

MillerIndex::MillerIndex()
{
    initialize(0, 0, 0);
}

MillerIndex::MillerIndex(int h, int k, int l)
{
    initialize(h, k, l);
}

MillerIndex::MillerIndex(const MillerIndex& copy)
{
    initialize(copy.h(), copy.k(), copy.l());
}

void MillerIndex::initialize(int h, int k, int l)
{
    _h = h;
    _k = k;
    _l = l;
}

MillerIndex& MillerIndex::operator =(const MillerIndex& rhs)
{
    this->initialize(rhs.h(), rhs.k(), rhs.l());
    return *this;
}

bool MillerIndex::operator ==(const MillerIndex& rhs) const
{
    return (this->h() == rhs.h() && this->k()==rhs.k() && this->l() == rhs.l());
}

bool MillerIndex::operator <(const MillerIndex& rhs) const
{
    return( (this->h() == rhs.h() && this->k() == rhs.k() && this->l() < rhs.l()) || 
            (this->h() == rhs.h() && this->k() < rhs.k()) || 
            (this->h() < rhs.h())
          );
}

std::ostream& MillerIndex::operator <<(std::ostream& os) const
{
    return os << h() << " " << k() << " " << l();
}

void MillerIndex::set_values(int h, int k, int l)
{
    this->initialize(h, k, l);
}

int MillerIndex::h() const
{
    return _h;
}

int MillerIndex::k() const
{
    return _k;
}

int MillerIndex::l() const
{
    return _l;
}

std::string MillerIndex::to_string() const
{
    std::string result = "(" + std::to_string(h()) + ", " + std::to_string(k()) + ", " + std::to_string(l()) +")";
    return result;
}

MillerIndex MillerIndex::FriedelSpot() const
{
    return MillerIndex(-1*h(), -1*k(), -1*l());
}