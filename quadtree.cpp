/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "quadtree.h"

using namespace std;

Quadtree_Rect::Quadtree_Rect(){
    x=0;
    y=0;
    w=0;
    h=0;
}

Quadtree_Rect::Quadtree_Rect(uint32_t get_x,uint32_t get_y,uint32_t get_w,uint32_t get_h){
    x=get_x;
    y=get_y;
    w=get_w;
    h=get_h;
}
