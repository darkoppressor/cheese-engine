/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef collision_h
#define collision_h

#include "engine_math.h"

#include <algorithm>
#include <stdint.h>

template<typename T>
class Collision_Rect{
public:

    T x;
    T y;
    T w;
    T h;

    Collision_Rect(){
        x=(T)0;
        y=(T)0;
        w=(T)0;
        h=(T)0;
    }

    Collision_Rect(T get_x,T get_y,T get_w,T get_h){
        x=get_x;
        y=get_y;
        w=get_w;
        h=get_h;
    }

    Collision_Rect<T> operator*(T scalar){
        return Collision_Rect(x*scalar,y*scalar,w*scalar,h*scalar);
    }

    Collision_Rect<T> operator/(T scalar){
        return Collision_Rect(x/scalar,y/scalar,w/scalar,h/scalar);
    }

    T center_x() const{
        return x+w/(T)2;
    }

    T center_y() const{
        return y+h/(T)2;
    }
};

template<typename T>
class Collision_Circ{
public:

    T x;
    T y;
    T r;

    Collision_Circ(){
        x=(T)0;
        y=(T)0;
        r=(T)0;
    }

    Collision_Circ(T get_x,T get_y,T get_r){
        x=get_x;
        y=get_y;
        r=get_r;
    }

    Collision_Circ<T> operator*(T scalar){
        return Collision_Circ(x*scalar,y*scalar,r*scalar);
    }

    Collision_Circ<T> operator/(T scalar){
        return Collision_Circ(x/scalar,y/scalar,r/scalar);
    }
};

class Collision{
public:

    template<typename T>
    static bool check_rect(Collision_Rect<T> box_a,Collision_Rect<T> box_b){
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

    template<typename T>
    static bool check_circ(Collision_Circ<T> circle_a,Collision_Circ<T> circle_b){
        T a=circle_a.r+circle_b.r;

        T dx=circle_a.x-circle_b.x;
        T dy=circle_a.y-circle_b.y;

        return a*a>(dx*dx)+(dy*dy);
    }

    template<typename T>
    static bool check_circ_rect(Collision_Circ<T> circle,Collision_Rect<T> box){
        T closest_x=(T)0;
        T closest_y=(T)0;

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

        T dx=circle.x-closest_x;
        T dy=circle.y-closest_y;

        return circle.r*circle.r>(dx*dx)+(dy*dy);
    }

    //Returns a rectangle containing the collision area of the two passed rectangles
    template<typename T>
    static Collision_Rect<T> get_collision_area_rect(Collision_Rect<T> box_a,Collision_Rect<T> box_b){
        if(check_rect(box_a,box_b)){
            Collision_Rect<T> box;

            box.x=max(box_a.x,box_b.x);
            box.y=max(box_a.y,box_b.y);
            box.w=min(box_a.x+box_a.w,box_b.x+box_b.w)-box.x;
            box.h=min(box_a.y+box_a.h,box_b.y+box_b.h)-box.y;

            return box;
        }
        else{
            Collision_Rect<T> box;

            return box;
        }
    }

    static double get_angle_to_rect(Collision_Rect<double> box_a,Collision_Rect<double> box_b,Collision_Rect<double> camera){
        double x1=box_a.x+box_a.w/2.0-camera.x;
        double y1=box_a.y+box_a.h/2.0-camera.y;

        double x2=box_b.x+box_b.w/2.0-camera.x;
        double y2=box_b.y+box_b.h/2.0-camera.y;

        double x_component=Math::sqrt((x2-x1)*(x2-x1));
        double y_component=Math::sqrt((y2-y1)*(y2-y1));

        if(x2<x1){
            x_component*=-1.0;
        }

        double angle=Math::radians_to_degrees(Math::atan2(y_component,x_component));

        if(y2>y1){
            angle=360.0-angle;
        }

        return angle;
    }

    static double get_angle_to_rect(Collision_Circ<double> circle,Collision_Rect<double> box,Collision_Rect<double> camera){
        double x1=circle.x-camera.x;
        double y1=circle.y-camera.y;

        double x2=box.x+box.w/2.0-camera.x;
        double y2=box.y+box.h/2.0-camera.y;

        double x_component=Math::sqrt((x2-x1)*(x2-x1));
        double y_component=Math::sqrt((y2-y1)*(y2-y1));

        if(x2<x1){
            x_component*=-1.0;
        }

        double angle=Math::radians_to_degrees(Math::atan2(y_component,x_component));

        if(y2>y1){
            angle=360.0-angle;
        }

        return angle;
    }

    static double get_angle_to_circ(Collision_Circ<double> circle_a,Collision_Circ<double> circle_b,Collision_Rect<double> camera){
        double x1=circle_a.x-camera.x;
        double y1=circle_a.y-camera.y;

        double x2=circle_b.x-camera.x;
        double y2=circle_b.y-camera.y;

        double x_component=Math::sqrt((x2-x1)*(x2-x1));
        double y_component=Math::sqrt((y2-y1)*(y2-y1));

        if(x2<x1){
            x_component*=-1.0;
        }

        double angle=Math::radians_to_degrees(Math::atan2(y_component,x_component));

        if(y2>y1){
            angle=360.0-angle;
        }

        return angle;
    }

    static double get_angle_to_circ(Collision_Rect<double> box,Collision_Circ<double> circle,Collision_Rect<double> camera){
        double x1=box.x+box.w/2.0-camera.x;
        double y1=box.y+box.h/2.0-camera.y;

        double x2=circle.x-camera.x;
        double y2=circle.y-camera.y;

        double x_component=Math::sqrt((x2-x1)*(x2-x1));
        double y_component=Math::sqrt((y2-y1)*(y2-y1));

        if(x2<x1){
            x_component*=-1.0;
        }

        double angle=Math::radians_to_degrees(Math::atan2(y_component,x_component));

        if(y2>y1){
            angle=360.0-angle;
        }

        return angle;
    }
};

class Collision_Event{
public:

    uint32_t object1;
    uint32_t object2;

    Collision_Event();
    Collision_Event(uint32_t get_object1,uint32_t get_object2);

    bool operator==(const Collision_Event& collision1) const;
};

#endif
