/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef gui_manager_h
#define gui_manager_h

#include "gui_object.h"

#include <vector>
#include <string>

class GUI_Manager{
private:

    static bool initialized;

public:

    //-1 if no object is selected
    static int gui_selected_object;

    //Possible values:
    //mouse
    //keyboard
    //controller
    static std::string gui_mode;

    static bool hide_gui;

    static std::vector<GUI_Selector_Chaser> gui_selector_chasers;

    static std::string gui_axis_nav_last_direction;

    static int counter_gui_scroll_axis;

    static Sprite text_selector;

    static void initialize();
    static void deinitialize();

    //Check if we need to scroll the scrolling button list
    static void gui_check_scrolling_button();
    static void reset_gui_selector_chasers();
    static void reset_gui_selected_object();
    static void set_gui_mode(std::string new_gui_mode);
    //Returns the number of GUI objects on the currently focused window
    //Returns 0 if no window is currently in focus
    static int get_gui_object_count();
    static void change_gui_selected_object(std::string direction);
    //Returns a GUI_Object for the selected GUI object
    //If there is no selected GUI object, the GUI_Object's type will equal none
    static GUI_Object get_gui_selected_object();
    //Returns a Engine_Rect with the position and size of the selected GUI object
    //If there is no selected GUI object, returns a Engine_Rect of all -1's
    static Engine_Rect get_gui_selected_object_pos();
    static bool is_gui_object_selected(Button* button_to_check);
    static bool is_gui_object_selected(Information* info_to_check);
    //Activates the currently selected GUI object
    static void confirm_gui_object();
    static void scroll_gui_object_up();
    static void scroll_gui_object_down();

    //Only used by Android hardware buttons (I think...)
    static void gui_nav_back_android();
    static void gui_nav_toggle_menu_android();

    //Only used by controller mode:
    static void gui_nav_back_controller();
    static void gui_nav_toggle_menu_controller();

    //Only used by mouse and keyboard modes:
    static void gui_nav_back_keyboard_and_mouse();

    //Used by all control modes:
    static void gui_nav_confirm(std::string gui_type);
    static void gui_nav_up(std::string gui_type);
    static void gui_nav_down(std::string gui_type);
    static void gui_nav_left();
    static void gui_nav_right();
    static void gui_scroll_up(std::string gui_type);
    static void gui_scroll_down(std::string gui_type);

    static void animate_gui_selector_chasers();
    static void animate();

    static void render_gui_selector();
};

#endif
