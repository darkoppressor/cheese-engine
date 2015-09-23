/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "object_manager.h"

using namespace std;

void Object_Manager::animate_cursors(){
    for(size_t i=0;i<cursors.size();i++){
        cursors[i].animate();
    }
}
