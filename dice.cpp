/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "dice.h"

using namespace std;

uint32_t Dice::rollTotal (RNG& rng, uint32_t dice, uint32_t sides) {
    uint32_t result = 0u;

    for (uint32_t x = 0; x < dice; x++) {
        result += rng.random_range(1, sides);
    }

    return result;
}

vector<uint32_t> Dice::rollEach (RNG& rng, uint32_t dice, uint32_t sides) {
    vector<uint32_t> results;

    for (uint32_t x = 0; x < dice; x++) {
        results.push_back(rng.random_range(1, sides));
    }

    return results;
}
