/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef rng_h
#define rng_h

#include <cstdint>

class RNG{
public:

    enum class Weight : std::uint8_t{
        WEAK,
        NORMAL,
        STRONG,
        VERY_STRONG
    };

private:

    std::uint32_t Q[4096];
    std::uint32_t c;
    std::uint32_t u;

    static const std::uint32_t WEIGHT_WEAK;
    static const std::uint32_t WEIGHT_NORMAL;
    static const std::uint32_t WEIGHT_STRONG;
    static const std::uint32_t WEIGHT_VERY_STRONG;

    std::uint32_t get_number();

public:

    RNG();
    RNG(std::uint32_t seed_value);

    void seed(std::uint32_t seed_value);

    std::uint32_t random_range(std::uint32_t lownum,std::uint32_t highnum);

    //weight must be > 0
    //weight must also be greater than 1 or the result will be unweighted
    std::uint32_t weighted_random_range(std::uint32_t lownum,std::uint32_t highnum,std::uint32_t target,std::uint32_t weight);

    std::uint32_t weighted_random_range(std::uint32_t lownum,std::uint32_t highnum,std::uint32_t target,Weight weight=Weight::NORMAL);
};

#endif
