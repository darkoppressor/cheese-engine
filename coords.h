/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef coords_h
#define coords_h

class Coords{
public:

    int x;
    int y;

    Coords();
    Coords(int get_x,int get_y);
};

class Engine_Rect{
public:

    int x;
    int y;
    int w;
    int h;

    Engine_Rect();
    Engine_Rect(int get_x,int get_y,int get_w,int get_h);
};

#endif
