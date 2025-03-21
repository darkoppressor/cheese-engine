/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef coords_h
#define coords_h

template<typename T>
class Coords {
    public:
        T x;
        T y;

        Coords () {
            x = (T) -1;
            y = (T) -1;
        }

        Coords (T get_x, T get_y) {
            x = get_x;
            y = get_y;
        }

        bool operator== (const Coords& coords) const {
            return x == coords.x && y == coords.y;
        }

        bool operator!= (const Coords& coords) const {
            return x != coords.x || y != coords.y;
        }
};

class Engine_Rect {
    public:
        int x;
        int y;
        int w;
        int h;

        Engine_Rect ();
        Engine_Rect (int get_x, int get_y, int get_w, int get_h);
};

#endif
