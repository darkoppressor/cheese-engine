/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef dice_h
#define dice_h

#include "rng.h"

#include <vector>

class Dice {
    public:
        static std::uint32_t rollTotal(RNG& rng, std::uint32_t dice, std::uint32_t sides);
        static std::vector<std::uint32_t> rollEach(RNG& rng, std::uint32_t dice, std::uint32_t sides);
};

#endif
