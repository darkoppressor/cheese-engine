/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef int_collision_h
#define int_collision_h

#include "../collision.h"
#include "../../math/int/int_math.h"

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
