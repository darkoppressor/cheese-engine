/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef gui_object_h
#define gui_object_h

#include <string>
#include <vector>

class GUI_Object {
    public:
        // Valid values:
        // button
        // information
        std::string type;
        int index;
        int x;
        int y;
        bool sort_by_y;

        GUI_Object (std::string get_type, int get_index, int get_x, int get_y);

        bool operator<= (GUI_Object object);

        static void set_sort_by_y(std::vector<GUI_Object>& objects, bool sort_value);
};

class GUI_Selector_Chaser {
    public:
        double x;
        double y;

        GUI_Selector_Chaser ();
};

#endif
