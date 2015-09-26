/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "collision.h"

using namespace std;

Collision_Event::Collision_Event(){
    object1=0;
    object2=0;
}

Collision_Event::Collision_Event(uint32_t get_object1,uint32_t get_object2){
    object1=get_object1;
    object2=get_object2;
}

bool Collision_Event::operator==(const Collision_Event& collision1) const{
    return object1==collision1.object1 && object2==collision1.object2;
}
