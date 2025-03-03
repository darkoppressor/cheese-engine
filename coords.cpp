/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "coords.h"

using namespace std;

Engine_Rect::Engine_Rect () {
    x = -1;
    y = -1;
    w = -1;
    h = -1;
}

Engine_Rect::Engine_Rect (int get_x, int get_y, int get_w, int get_h) {
    x = get_x;
    y = get_y;
    w = get_w;
    h = get_h;
}
