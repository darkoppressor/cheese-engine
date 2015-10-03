/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "engine_math.h"

#include <cmath>

const double Math::PI=3.14159265358979323846;
const double Math::E=2.71828182845904523536;

double Math::abs(double x){
    return std::abs(x);
}

double Math::ceil(double x){
    return std::ceil(x);
}

double Math::floor(double x){
    return std::floor(x);
}

double Math::pow(double base,double exponent){
    return std::pow(base,exponent);
}

double Math::sqrt(double x){
    return std::sqrt(x);
}

double Math::cos(double x){
    return std::cos(x);
}

double Math::sin(double x){
    return std::sin(x);
}

double Math::tan(double x){
    return std::tan(x);
}

double Math::acos(double x){
    return std::acos(x);
}

double Math::asin(double x){
    return std::asin(x);
}

double Math::atan(double x){
    return std::atan(x);
}

double Math::atan2(double y,double x){
    return std::atan2(y,x);
}

double Math::degrees_to_radians(double degrees){
    return degrees*(PI/180.0);
}

double Math::radians_to_degrees(double radians){
    return radians*(180.0/PI);
}

void Math::clamp_angle(double& angle){
    while(angle>360.0){
        angle-=360.0;
    }

    while(angle<0.0){
        angle+=360.0;
    }
}

int Math::get_angle_quadrant(double angle){
    clamp_angle(angle);

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

double Math::distance_between_points(double x1,double y1,double x2,double y2){
    return sqrt(pow(x2-x1,2.0)+pow(y2-y1,2.0));
}
