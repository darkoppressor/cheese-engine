/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef int_math_h
#define int_math_h

#include "../../collision/coords.h"

#include <cstdint>

class Int_Math {
    private:
        // Used for integer trigonometry
        static const std::int32_t sine_values[];
        static const std::int32_t atan_values[];

    public:
        static const std::int32_t trig_adjust;

        // Clamp the passed angle to within [0,359]
        static void clamp_angle(std::int32_t& angle);

        // These return their value multiplied by trig_adjust
        // After using the result from sin or cos, divide by trig adjust
        // to correct the final result
        static std::int32_t sin(std::int32_t x);
        static std::int32_t cos(std::int32_t x);
        static std::int32_t atan2(std::int32_t y, std::int32_t x);

        // sqrt takes uint64_t instead of uint32_t because it is often passed a number squared (and then another number
        // squared, and then the two numbers' sum)
        static std::uint64_t sqrt(std::uint64_t x);
        static std::int32_t get_angle_to_point(const Coords<std::int32_t>& point_a,
                                               const Coords<std::int32_t>& point_b);

        // get_distance_between_points may be removed at some point, if it is not needed
        // get_distance_between_points_no_sqrt is much faster
        static std::uint64_t get_distance_between_points(const Coords<std::int32_t>& point_a,
                                                         const Coords<std::int32_t>& point_b);
        static std::uint64_t get_distance_between_points_no_sqrt(const Coords<std::int32_t>& point_a,
                                                                 const Coords<std::int32_t>& point_b);
};

#endif
