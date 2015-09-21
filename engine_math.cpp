/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "engine_math.h"

#include <cmath>

using namespace std;

const double Math::PI=3.14159265358979323846;
const double Math::E=2.71828182845904523536;

double Math::degrees_to_radians(double degrees){
    return degrees*(PI/180.0);
}

double Math::radians_to_degrees(double radians){
    return radians*(180.0/PI);
}

void Math::reduce_angle(double& angle){
    while(angle>360.0){
        angle-=360.0;
    }

    while(angle<0.0){
        angle+=360.0;
    }
}

int Math::get_angle_quadrant(double angle){
    reduce_angle(angle);

    if(angle>0.0 && angle<90.0){
        return 1;
    }
    else if(angle>90.0 && angle<180.0){
        return 2;
    }
    else if(angle>180.0 && angle<270.0){
        return 3;
    }
    else if(angle>270.0 && angle<360.0){
        return 4;
    }
    else{
        return 0;
    }
}

bool Math::signs_same(int a,int b){
    if((a<0 && b<0) || (a>0 && b>0) || (a==0 && b==0)){
        return true;
    }
    else{
        return false;
    }
}

double Math::distance_between_points(double x1,double y1,double x2,double y2){
    return sqrt(pow(x2-x1,2.0)+pow(y2-y1,2.0));
}
