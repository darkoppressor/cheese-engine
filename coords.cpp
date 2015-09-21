/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "coords.h"

using namespace std;

Coords::Coords(){
    x=-1;
    y=-1;
}

Coords::Coords(int get_x,int get_y){
    x=get_x;
    y=get_y;
}

Engine_Rect::Engine_Rect(){
    x=-1;
    y=-1;
    w=-1;
    h=-1;
}

Engine_Rect::Engine_Rect(int get_x,int get_y,int get_w,int get_h){
    x=get_x;
    y=get_y;
    w=get_w;
    h=get_h;
}
