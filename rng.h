/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef rng_h
#define rng_h

#include <string>
#include <stdint.h>

class RNG{
private:

    uint32_t Q[4096];
    uint32_t c;
    uint32_t u;

    static const uint32_t WEIGHT_WEAK;
    static const uint32_t WEIGHT_NORMAL;
    static const uint32_t WEIGHT_STRONG;
    static const uint32_t WEIGHT_VERY_STRONG;

    uint32_t get_number();

public:

    RNG();
    RNG(uint32_t seed_value);

    void seed();
    void seed(uint32_t seed_value);

    uint32_t random_range(uint32_t lownum,uint32_t highnum);

    //weight must be >0. weight must also be greater than 1 or the result will be unweighted.
    uint32_t weighted_random_range(uint32_t lownum,uint32_t highnum,uint32_t target,uint32_t weight);

    //Valid values for weight:
    //weak
    //normal
    //strong
    //very_strong
    uint32_t weighted_random_range(uint32_t lownum,uint32_t highnum,uint32_t target,std::string weight="normal");
};

#endif
