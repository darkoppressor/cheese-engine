/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "gui_object.h"

using namespace std;

GUI_Object::GUI_Object (string get_type, int get_index, int get_x, int get_y) {
    type = get_type;
    index = get_index;
    x = get_x;
    y = get_y;
    sort_by_y = false;
}

bool GUI_Object::operator<= (GUI_Object object) {
    if (sort_by_y) {
        return y <= object.y;
    } else {
        return x <= object.x;
    }
}

void GUI_Object::set_sort_by_y (vector<GUI_Object>& objects, bool sort_value) {
    for (int i = 0; i < objects.size(); i++) {
        objects[i].sort_by_y = sort_value;
    }
}

GUI_Selector_Chaser::GUI_Selector_Chaser () {
    x = -1.0;
    y = -1.0;
}
