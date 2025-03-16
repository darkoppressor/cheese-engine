/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "collision.h"

using namespace std;

size_t Collision::VERTEX_UPPER_LEFT = 0;
size_t Collision::VERTEX_UPPER_RIGHT = 1;
size_t Collision::VERTEX_LOWER_LEFT = 2;
size_t Collision::VERTEX_LOWER_RIGHT = 3;

Collision_Event::Collision_Event () {
    object1 = 0;
    object2 = 0;
}

Collision_Event::Collision_Event (uint32_t get_object1, uint32_t get_object2) {
    object1 = get_object1;
    object2 = get_object2;
}

bool Collision_Event::operator== (const Collision_Event& collision1) const {
    return object1 == collision1.object1 && object2 == collision1.object2;
}
