/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "collision.h"
#include "engine_math.h"

#include <algorithm>

using namespace std;

Collision_Rect::Collision_Rect(){
    x=0.0;
    y=0.0;
    w=0.0;
    h=0.0;
}

Collision_Rect::Collision_Rect(double get_x,double get_y,double get_w,double get_h){
    x=get_x;
    y=get_y;
    w=get_w;
    h=get_h;
}

Collision_Rect Collision_Rect::operator*(double scalar){
    return Collision_Rect(x*scalar,y*scalar,w*scalar,h*scalar);
}

Collision_Rect Collision_Rect::operator/(double scalar){
    return Collision_Rect(x/scalar,y/scalar,w/scalar,h/scalar);
}

double Collision_Rect::center_x(){
    return x+w/2.0;
}

double Collision_Rect::center_y(){
    return y+h/2.0;
}

Collision_Circ::Collision_Circ(){
    x=0.0;
    y=0.0;
    r=0.0;
}

Collision_Circ::Collision_Circ(double get_x,double get_y,double get_r){
    x=get_x;
    y=get_y;
    r=get_r;
}

Collision_Circ Collision_Circ::operator*(double scalar){
    return Collision_Circ(x*scalar,y*scalar,r*scalar);
}

Collision_Circ Collision_Circ::operator/(double scalar){
    return Collision_Circ(x/scalar,y/scalar,r/scalar);
}

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

bool Collision::check_rect(Collision_Rect box_a,Collision_Rect box_b){
    if(box_a.y+box_a.h<box_b.y){
        return false;
    }
    else if(box_a.y>box_b.y+box_b.h){
        return false;
    }
    else if(box_a.x+box_a.w<box_b.x){
        return false;
    }
    else if(box_a.x>box_b.x+box_b.w){
        return false;
    }

    return true;
}

bool Collision::check_circ(Collision_Circ circle_a,Collision_Circ circle_b){
    double a=circle_a.r+circle_b.r;

    double dx=circle_a.x-circle_b.x;
    double dy=circle_a.y-circle_b.y;

    return a*a>(dx*dx)+(dy*dy);
}

bool Collision::check_circ_rect(Collision_Circ circle,Collision_Rect box){
    double closest_x=0.0;
    double closest_y=0.0;

    if(circle.x<box.x){
        closest_x=box.x;
    }
    else if(circle.x>box.x+box.w){
        closest_x=box.x+box.w;
    }
    else{
        closest_x=circle.x;
    }

    if(circle.y<box.y){
        closest_y=box.y;
    }
    else if(circle.y>box.y+box.h){
        closest_y=box.y+box.h;
    }
    else{
        closest_y=circle.y;
    }

    double dx=circle.x-closest_x;
    double dy=circle.y-closest_y;

    return circle.r*circle.r>(dx*dx)+(dy*dy);
}

Collision_Rect Collision::get_collision_area_rect(Collision_Rect box_a,Collision_Rect box_b){
    if(check_rect(box_a,box_b)){
        Collision_Rect box;

        box.x=max(box_a.x,box_b.x);
        box.y=max(box_a.y,box_b.y);
        box.w=min(box_a.x+box_a.w,box_b.x+box_b.w)-box.x;
        box.h=min(box_a.y+box_a.h,box_b.y+box_b.h)-box.y;

        return box;
    }
    else{
        Collision_Rect box;

        return box;
    }
}

double Collision::get_angle_to_rect(Collision_Rect box_a,Collision_Rect box_b,Collision_Rect camera){
    double x1=box_a.x+box_a.w/2.0-camera.x;
    double y1=box_a.y+box_a.h/2.0-camera.y;

    double x2=box_b.x+box_b.w/2.0-camera.x;
    double y2=box_b.y+box_b.h/2.0-camera.y;

    double x_component=sqrt((x2-x1)*(x2-x1));
    double y_component=sqrt((y2-y1)*(y2-y1));

    if(x2<x1){
        x_component*=-1.0;
    }

    double angle=Math::radians_to_degrees(atan2(y_component,x_component));

    if(y2>y1){
        angle=360.0-angle;
    }

    return angle;
}

double Collision::get_angle_to_rect(Collision_Circ circle,Collision_Rect box,Collision_Rect camera){
    double x1=circle.x-camera.x;
    double y1=circle.y-camera.y;

    double x2=box.x+box.w/2.0-camera.x;
    double y2=box.y+box.h/2.0-camera.y;

    double x_component=sqrt((x2-x1)*(x2-x1));
    double y_component=sqrt((y2-y1)*(y2-y1));

    if(x2<x1){
        x_component*=-1.0;
    }

    double angle=Math::radians_to_degrees(atan2(y_component,x_component));

    if(y2>y1){
        angle=360.0-angle;
    }

    return angle;
}

double Collision::get_angle_to_circ(Collision_Circ circle_a,Collision_Circ circle_b,Collision_Rect camera){
    double x1=circle_a.x-camera.x;
    double y1=circle_a.y-camera.y;

    double x2=circle_b.x-camera.x;
    double y2=circle_b.y-camera.y;

    double x_component=sqrt((x2-x1)*(x2-x1));
    double y_component=sqrt((y2-y1)*(y2-y1));

    if(x2<x1){
        x_component*=-1.0;
    }

    double angle=Math::radians_to_degrees(atan2(y_component,x_component));

    if(y2>y1){
        angle=360.0-angle;
    }

    return angle;
}

double Collision::get_angle_to_circ(Collision_Rect box,Collision_Circ circle,Collision_Rect camera){
    double x1=box.x+box.w/2.0-camera.x;
    double y1=box.y+box.h/2.0-camera.y;

    double x2=circle.x-camera.x;
    double y2=circle.y-camera.y;

    double x_component=sqrt((x2-x1)*(x2-x1));
    double y_component=sqrt((y2-y1)*(y2-y1));

    if(x2<x1){
        x_component*=-1.0;
    }

    double angle=Math::radians_to_degrees(atan2(y_component,x_component));

    if(y2>y1){
        angle=360.0-angle;
    }

    return angle;
}
