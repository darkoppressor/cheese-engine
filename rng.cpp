/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "rng.h"
#include "log.h"
#include "engine_math.h"

#include <ctime>
#include <vector>

using namespace std;

const uint32_t RNG::WEIGHT_WEAK = 3;
const uint32_t RNG::WEIGHT_NORMAL = 6;
const uint32_t RNG::WEIGHT_STRONG = 18;
const uint32_t RNG::WEIGHT_VERY_STRONG = 256;
uint32_t RNG::get_number () {
    u = (u + 1) & 4095;
    uint64_t t = (uint64_t) 18782 * (uint64_t) Q[u] + (uint64_t) c;
    c = (t >> 32);
    uint32_t x = t + c;

    if (x < c) {
        x++;
        c++;
    }

    return (uint32_t) (Q[u] = 0xfffffffe - x);
}

uint32_t RNG::get_time_number () const {
    return (uint32_t) time(0);
}

RNG::RNG () {
    seed();
}

RNG::RNG (uint32_t seed_value) {
    seed(seed_value);
}

void RNG::seed (uint32_t seed_value) {
    uint32_t s = seed_value;

    for (uint32_t i = 0; i < 4096; i++) {
        Q[i] = s = (s * 1103515245) + 12345;
    }

    c = ((s * 1103515245) + 12345) % 809430660;
    u = 0;
}

void RNG::seed () {
    seed(get_time_number());
}

uint32_t RNG::random_range (uint32_t lownum, uint32_t highnum) {
    if (lownum > highnum) {
        swap(lownum, highnum);
    }

    uint32_t range = highnum - lownum + 1;

    // If range is 0, the equation above wrapped around
    if (range == 0) {
        range--;
    }

    return (get_number() % range) + lownum;
}

uint32_t RNG::weighted_random_range (uint32_t lownum, uint32_t highnum, uint32_t target, uint32_t weight) {
    if (weight == 0) {
        Log::add_error("Error generating weighted random number: weight cannot be 0");
    }

    vector<uint32_t> numbers;

    for (uint32_t i = 0; i < weight; i++) {
        numbers.push_back(random_range(lownum, highnum));
    }

    uint32_t closest = numbers[0];

    for (uint32_t i = 0; i < numbers.size(); i++) {
        if (Math::abs(numbers[i], target) < Math::abs(closest, target)) {
            closest = numbers[i];
        }
    }

    return closest;
}

uint32_t RNG::weighted_random_range (uint32_t lownum, uint32_t highnum, uint32_t target, Weight weight) {
    uint32_t weight_num = WEIGHT_NORMAL;

    if (weight == Weight::WEAK) {
        weight_num = WEIGHT_WEAK;
    } else if (weight == Weight::NORMAL) {
        weight_num = WEIGHT_NORMAL;
    } else if (weight == Weight::STRONG) {
        weight_num = WEIGHT_STRONG;
    } else if (weight == Weight::VERY_STRONG) {
        weight_num = WEIGHT_VERY_STRONG;
    }

    return weighted_random_range(lownum, highnum, target, weight_num);
}
