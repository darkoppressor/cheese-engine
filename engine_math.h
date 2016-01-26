/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef engine_math_h
#define engine_math_h

#include "coords.h"

#include <algorithm>

class Math{
public:

    static const double PI;
    static const double E;

    static double abs(double x);
    static double ceil(double x);
    static double floor(double x);
    static double pow(double base,double exponent);
    static double sqrt(double x);
    static double cos(double x);
    static double sin(double x);
    static double tan(double x);
    static double acos(double x);
    static double asin(double x);
    static double atan(double x);
    static double atan2(double y,double x);

    template<typename T>
    static T abs(const T& a,const T& b){
        return std::max(a,b)-std::min(a,b);
    }

    static double degrees_to_radians(double degrees);
    static double radians_to_degrees(double radians);

    //Clamp the passed angle to within [0.0,360.0]
    static void clamp_angle(double& angle);
    //Returns the quadrant that the passed angle is in, or 0 if it is a quadrantal angle
    static int get_angle_quadrant(double angle);

    static Coords<double> rotate_point(const Coords<double>& point,const Coords<double>& center,double angle);

    static double get_angle_to_point(const Coords<double>& point_a,const Coords<double>& point_b);

    template<typename T>
    static bool signs_same(const T& a,const T& b){
        if((a<0 && b<0) || (a>0 && b>0) || (a==0 && b==0)){
            return true;
        }
        else{
            return false;
        }
    }

    static double distance_between_points(double x1,double y1,double x2,double y2);
};

#endif
