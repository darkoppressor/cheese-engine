/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef engine_math_h
#define engine_math_h

class Math{
public:

    static const double PI;
    static const double E;

    static double degrees_to_radians(double degrees);
    static double radians_to_degrees(double radians);

    //Reduce the passed angle so that it is within [0.0,360.0]
    static void reduce_angle(double& angle);
    //Returns the quadrant that the passed angle is in, or 0 if it is a quadrantal angle.
    static int get_angle_quadrant(double angle);

    static bool signs_same(int a,int b);

    static double distance_between_points(double x1,double y1,double x2,double y2);
};

#endif
