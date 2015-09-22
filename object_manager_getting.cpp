/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "object_manager.h"
#include "log.h"

using namespace std;

Color* Object_Manager::get_color(string name){
    Color* ptr_object=0;

    for(size_t i=0;i<colors.size();i++){
        if(colors[i].name==name){
            ptr_object=&colors[i];

            break;
        }
    }

    if(ptr_object==0){
        Log::add_error("Error accessing color '"+name+"'");
    }

    return ptr_object;
}

Animation* Object_Manager::get_animation(string name){
    Animation* ptr_object=0;

    for(size_t i=0;i<animations.size();i++){
        if(animations[i].name==name){
            ptr_object=&animations[i];

            break;
        }
    }

    if(ptr_object==0){
        Log::add_error("Error accessing animation '"+name+"'");
    }

    return ptr_object;
}
