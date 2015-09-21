/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "color.h"

using namespace std;

Color_Data::Color_Data(){
    red=0.0;
    green=0.0;
    blue=0.0;
    alpha=0.0;
}

Color::Color(){
    name="";

    rgb.red=1.0;
    rgb.green=1.0;
    rgb.blue=1.0;
    rgb.alpha=1.0;
}

double Color::get_red(){
    return rgb.red;
}
double Color::get_green(){
    return rgb.green;
}
double Color::get_blue(){
    return rgb.blue;
}
double Color::get_alpha(){
    return rgb.alpha;
}

short Color::get_red_short(){
    return (short)(rgb.red*255.0);
}
short Color::get_green_short(){
    return (short)(rgb.green*255.0);
}
short Color::get_blue_short(){
    return (short)(rgb.blue*255.0);
}
short Color::get_alpha_short(){
    return (short)(rgb.alpha*255.0);
}

void Color::set_rgb(short red,short green,short blue,short alpha){
    rgb.red=red/255.0;
    rgb.green=green/255.0;
    rgb.blue=blue/255.0;
    rgb.alpha=alpha/255.0;
}

bool Color::is_equal(Color* color){
    if(color->get_red_short()==get_red_short() && color->get_green_short()==get_green_short() &&
       color->get_blue_short()==get_blue_short() && color->get_alpha_short()==get_alpha_short()){
        return true;
    }
    else{
        return false;
    }
}
