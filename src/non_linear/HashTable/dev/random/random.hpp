/*
    SPDX-License-Identifier: MPL-2.0
    --------------------------------
    This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
    If a copy of the MPL was not distributed with this file,
    You can obtain one at https://mozilla.org/MPL/2.0/.

    Provided “as is”, without warranty of any kind.

    Copyright © 2025 Alar Akilbekov. All rights reserved.
 */

#pragma once
#ifndef _RANDOM_HPP_
#define _RANDOM_HPP_

// #include <cstdlib> // rand, srand, and RAND_MAX
#include <random>

template <typename IntType = int>
class Random {
public:
    std::uniform_int_distribution<IntType> uniform;
    std::mt19937 mt_gen; // генератор Mersenne Twister
    /*
        random_device - источник энтропии
        random_device seeds mt19937
        random_device generates random number, but it is expensive operation
        поэтому не нужно использовать rd(), как генератор
    */
    Random(IntType min, IntType max) : uniform(min, max), mt_gen(std::random_device{}()) {}
    IntType random(){ return uniform(mt_gen); }
    IntType operator()(){ return uniform(mt_gen); } // functor, syntax sugar
    IntType min() const { return uniform.min(); }
    IntType max() const { return uniform.max(); }
};

#endif

