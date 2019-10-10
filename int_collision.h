/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef int_collision_h
#define int_collision_h

#include "collision.h"
#include "int_math.h"

#include <cstdint>

class Int_Collision {
    public:
        static std::int32_t get_angle_to_rect (const Collision_Rect<std::int32_t>& box_a,
                                               const Collision_Rect<std::int32_t>& box_b) {
            return Int_Math::get_angle_to_point(box_a.get_center(), box_b.get_center());
        }

        static std::int32_t get_angle_to_rect (const Collision_Circ<std::int32_t>& circle,
                                               const Collision_Rect<std::int32_t>& box) {
            return Int_Math::get_angle_to_point(circle.get_center(), box.get_center());
        }

        static std::int32_t get_angle_to_circ (const Collision_Circ<std::int32_t>& circle_a,
                                               const Collision_Circ<std::int32_t>& circle_b) {
            return Int_Math::get_angle_to_point(circle_a.get_center(), circle_b.get_center());
        }

        static std::int32_t get_angle_to_circ (const Collision_Rect<std::int32_t>& box,
                                               const Collision_Circ<std::int32_t>& circle) {
            return Int_Math::get_angle_to_point(box.get_center(), circle.get_center());
        }
};

#endif
