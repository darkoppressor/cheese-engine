/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "gui_manager.h"
#include "window_manager.h"
#include "window.h"
#include "engine.h"
#include "log.h"
#include "engine_data.h"
#include "sorting.h"
#include "game_manager.h"
#include "render.h"
#include "engine_math.h"

using namespace std;

bool GUI_Manager::initialized = false;
int GUI_Manager::gui_selected_object = -1;
string GUI_Manager::gui_mode = "mouse";
bool GUI_Manager::hide_gui = false;
vector<GUI_Selector_Chaser> GUI_Manager::gui_selector_chasers;
string GUI_Manager::gui_axis_nav_last_direction = "none";
int GUI_Manager::counter_gui_scroll_axis = 0;
Sprite GUI_Manager::text_selector;
void GUI_Manager::initialize () {
    if (!initialized) {
        for (int i = 0; i < 2; i++) {
            gui_selector_chasers.push_back(GUI_Selector_Chaser());
        }

        initialized = true;
    }
}

void GUI_Manager::deinitialize () {
    if (initialized) {
        initialized = false;

        gui_selector_chasers.clear();
    }
}

void GUI_Manager::gui_check_scrolling_button () {
    if (gui_selected_object != -1) {
        Window* top_window = Window_Manager::get_top_window();

        if (top_window != 0) {
            GUI_Object object = get_gui_selected_object();

            if (object.type == "button") {
                if (top_window->is_button_scrolling(object.index)) {
                    for (int i = 0; i < 10000 && !top_window->is_scrolling_button_on_screen(object.index); i++) {
                        if (top_window->off_screen_scrolling_button_direction(object.index)) {
                            top_window->scroll_up();
                        } else {
                            top_window->scroll_down();
                        }
                    }
                }
            }
        }
    }
}

void GUI_Manager::reset_gui_selector_chasers () {
    for (int i = 0; i < gui_selector_chasers.size(); i++) {
        gui_selector_chasers[i].x = -1.0;
        gui_selector_chasers[i].y = -1.0;
    }
}

void GUI_Manager::reset_gui_selected_object () {
    gui_selected_object = -1;

    if (gui_mode == "keyboard" || gui_mode == "controller") {
        if (get_gui_object_count() > 0) {
            gui_selected_object = 0;
        }
    }

    gui_check_scrolling_button();

    reset_gui_selector_chasers();
}

void GUI_Manager::set_gui_mode (string new_gui_mode) {
    string previous_gui_mode = gui_mode;

    gui_mode = new_gui_mode;

    if (previous_gui_mode != gui_mode) {
        // Only reset selected object if we are switching to or from mouse mode
        if (previous_gui_mode == "mouse" || gui_mode == "mouse") {
            bool was_console = Engine::is_console_selected();
            bool was_chat = Engine::is_chat_selected();

            Engine::clear_mutable_info();

            reset_gui_selected_object();

            if (was_console) {
                Engine::console.toggle_on();
            } else if (was_chat) {
                Engine::chat.toggle_on();
            }
        }

        if (gui_mode != "mouse" && gui_mode != "keyboard" && gui_mode != "controller") {
            Log::add_error("Error setting GUI mode '" + gui_mode + "'");
        }
    }
}

int GUI_Manager::get_gui_object_count () {
    Window* top_window = Window_Manager::get_top_window();

    if (top_window != 0) {
        int button_count = 0;
        int info_count = 0;

        for (int i = 0; i < top_window->buttons.size(); i++) {
            if (!top_window->buttons[i].is_x && !top_window->buttons[i].is_divider) {
                button_count++;
            }
        }

        for (int i = 0; i < top_window->informations.size(); i++) {
            if (top_window->informations[i].text_mutable || top_window->informations[i].scrolling) {
                info_count++;
            }
        }

        return button_count + info_count;
    } else {
        return 0;
    }
}

void GUI_Manager::change_gui_selected_object (string direction) {
    int object_count = get_gui_object_count();
    int fast_nav = (int) Math::ceil((double) object_count * 0.1);

    if (object_count > 0) {
        Engine::clear_mutable_info();

        if (direction == "up") {
            if (--gui_selected_object < 0) {
                gui_selected_object = object_count - 1;
            }
        } else if (direction == "down") {
            if (++gui_selected_object > object_count - 1) {
                gui_selected_object = 0;
            }
        } else if (direction == "left") {
            gui_selected_object -= fast_nav;

            if (gui_selected_object < 0) {
                gui_selected_object = object_count - 1;
            }
        } else if (direction == "right") {
            gui_selected_object += fast_nav;

            if (gui_selected_object > object_count - 1) {
                gui_selected_object = 0;
            }
        } else {
            Log::add_error("Error navigating GUI in direction '" + direction + "'");
        }

        gui_check_scrolling_button();

        reset_gui_selector_chasers();
    } else {
        reset_gui_selected_object();
    }
}

GUI_Object GUI_Manager::get_gui_selected_object () {
    GUI_Object object("none", -1, -1, -1);

    if (gui_selected_object != -1) {
        Window* top_window = Window_Manager::get_top_window();

        if (top_window != 0) {
            vector<GUI_Object> objects;

            for (int i = 0; i < top_window->buttons.size(); i++) {
                if (!top_window->buttons[i].is_x && !top_window->buttons[i].is_divider) {
                    int real_y = top_window->buttons[i].y;

                    // If this button is non-scrolling and above the scrolling button area, add it normally
                    if (!top_window->is_button_scrolling(i) &&
                        top_window->buttons[i].y < Engine_Data::scrolling_buttons_offset) {}
                    // If this button is scrolling, place it at the top of the scrolling button area, offset by its
                    // position in the scrolling buttons list
                    else if (top_window->is_button_scrolling(i)) {
                        real_y = Engine_Data::scrolling_buttons_offset + top_window->get_scrolling_button_position(i);
                    }
                    // If this button is non-scrolling and below the scrolling button area, offset it by the bottommost
                    // scrolling button's height
                    else {
                        if (top_window->get_scrolling_button_count() > 0) {
                            real_y += Engine_Data::scrolling_buttons_offset + top_window->get_scrolling_button_count() -
                                      1;
                        }
                    }

                    objects.push_back(GUI_Object("button", i, top_window->buttons[i].x, real_y));
                }
            }

            for (int i = 0; i < top_window->informations.size(); i++) {
                if (top_window->informations[i].text_mutable || top_window->informations[i].scrolling) {
                    int real_y = top_window->informations[i].y;

                    // If there are scrolling buttons and this info is below the scrolling button area, offset it by the
                    // bottommost scrolling button's height
                    if (top_window->get_scrolling_button_count() > 0 &&
                        top_window->informations[i].y >= Engine_Data::scrolling_buttons_offset) {
                        real_y += Engine_Data::scrolling_buttons_offset + top_window->get_scrolling_button_count() - 1;
                    }

                    objects.push_back(GUI_Object("information", i, top_window->informations[i].x, real_y));
                }
            }

            if (objects.size() > gui_selected_object) {
                if (!top_window->no_button_sort) {
                    GUI_Object::set_sort_by_y(objects, true);
                    Sorting::quick_sort(objects);

                    // Build a list of all y values that have at least one duplicate
                    vector<int> y_duplicates_to_sort;
                    vector<int> y_values;

                    for (int i = 0; i < objects.size(); i++) {
                        if (y_values.size() == 0) {
                            y_values.push_back(objects[i].y);
                        } else {
                            for (int j = 0; j < y_values.size(); j++) {
                                if (objects[i].y == y_values[j]) {
                                    if (y_duplicates_to_sort.size() == 0) {
                                        y_duplicates_to_sort.push_back(y_values[j]);
                                    } else {
                                        for (int k = 0; k < y_duplicates_to_sort.size(); k++) {
                                            if (y_values[j] == y_duplicates_to_sort[k]) {
                                                break;
                                            }

                                            if (k == y_duplicates_to_sort.size() - 1) {
                                                y_duplicates_to_sort.push_back(y_values[j]);

                                                break;
                                            }
                                        }
                                    }

                                    break;
                                }

                                if (j == y_values.size() - 1) {
                                    y_values.push_back(objects[i].y);

                                    break;
                                }
                            }
                        }
                    }

                    for (int i = 0; i < y_duplicates_to_sort.size(); i++) {
                        vector<GUI_Object> replacements;
                        int initial_object = -1;

                        for (int j = 0; j < objects.size(); j++) {
                            if (objects[j].y == y_duplicates_to_sort[i]) {
                                if (initial_object == -1) {
                                    initial_object = j;
                                }

                                replacements.push_back(objects[j]);
                            }
                        }

                        GUI_Object::set_sort_by_y(replacements, false);
                        Sorting::quick_sort(replacements);

                        for (int j = 0; j < replacements.size(); j++) {
                            objects[initial_object + j] = replacements[j];
                        }
                    }
                }

                object = objects[gui_selected_object];
            }
        }

        if (object.type == "none") {
            reset_gui_selected_object();
        }
    }

    return object;
}

Engine_Rect GUI_Manager::get_gui_selected_object_pos () {
    Engine_Rect object_pos;

    if (gui_selected_object != -1) {
        Window* top_window = Window_Manager::get_top_window();

        if (top_window != 0) {
            GUI_Object object = get_gui_selected_object();

            if (object.type == "button") {
                Button* button = &top_window->buttons[object.index];

                object_pos.x = top_window->x + button->x;
                object_pos.y = top_window->y + button->y;
                object_pos.w = button->w;
                object_pos.h = button->h;

                if (top_window->is_button_scrolling(object.index)) {
                    object_pos.y = top_window->get_scrolling_button_offset(object.index) + button->y;
                }
            } else if (object.type == "information") {
                Information* info = &top_window->informations[object.index];

                object_pos.x = top_window->x + info->x;
                object_pos.y = top_window->y + info->y;
                object_pos.w = info->w;
                object_pos.h = info->h;
            }
        }
    }

    return object_pos;
}

bool GUI_Manager::is_gui_object_selected (Button* button_to_check) {
    if (gui_selected_object != -1) {
        Window* top_window = Window_Manager::get_top_window();

        if (top_window != 0) {
            GUI_Object object = get_gui_selected_object();

            if (object.type == "button") {
                Button* button = &top_window->buttons[object.index];

                if (button_to_check == button) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool GUI_Manager::is_gui_object_selected (Information* info_to_check) {
    if (gui_selected_object != -1) {
        Window* top_window = Window_Manager::get_top_window();

        if (top_window != 0) {
            GUI_Object object = get_gui_selected_object();

            if (object.type == "information") {
                Information* info = &top_window->informations[object.index];

                if (info_to_check == info) {
                    return true;
                }
            }
        }
    }

    return false;
}

void GUI_Manager::confirm_gui_object () {
    if (gui_selected_object != -1) {
        Window* top_window = Window_Manager::get_top_window();

        if (top_window != 0) {
            GUI_Object object = get_gui_selected_object();

            if (object.type == "button") {
                Button* button = &top_window->buttons[object.index];

                button->mouse_button_down();
                button->mouse_button_up(top_window);
                button->reset_clicked();
            } else if (object.type == "information") {
                Information* info = &top_window->informations[object.index];

                if (info->text_mutable) {
                    info->begin_editing();
                }
            }
        }
    }
}

void GUI_Manager::scroll_gui_object_up () {
    if (Engine::is_console_selected()) {
        Engine::console.info_display.scroll_up(0);
    } else if (Engine::is_chat_selected()) {
        Engine::chat.info_display.scroll_up(0);
    } else if (gui_selected_object != -1) {
        Window* top_window = Window_Manager::get_top_window();

        if (top_window != 0) {
            GUI_Object object = get_gui_selected_object();

            if (object.type == "information") {
                Information* info = &top_window->informations[object.index];

                info->scroll_up(top_window->y);
            }
        }
    }
}

void GUI_Manager::scroll_gui_object_down () {
    if (Engine::is_console_selected()) {
        Engine::console.info_display.scroll_down(0);
    } else if (Engine::is_chat_selected()) {
        Engine::chat.info_display.scroll_down(0);
    } else if (gui_selected_object != -1) {
        Window* top_window = Window_Manager::get_top_window();

        if (top_window != 0) {
            GUI_Object object = get_gui_selected_object();

            if (object.type == "information") {
                Information* info = &top_window->informations[object.index];

                info->scroll_down(top_window->y);
            }
        }
    }
}

void GUI_Manager::gui_nav_back_android () {
    string window_name = "main_menu";
    Window* window = Window_Manager::get_window(window_name);

    if (Engine::mutable_info_selected()) {
        Engine::clear_mutable_info();
    } else if (Window_Manager::is_any_window_open()) {
        if (!Game_Manager::in_progress && Window_Manager::is_window_open(window) &&
            Window_Manager::open_window_count() == 1) {
            Engine::quit();
        } else {
            Window_Manager::close_top_window();
        }
    } else {
        Engine::quit();
    }
}

void GUI_Manager::gui_nav_toggle_menu_android () {
    string window_name = "main_menu";

    if (Game_Manager::in_progress) {
        window_name = "ingame_menu";
    }

    Window* window = Window_Manager::get_window(window_name);

    if (Window_Manager::is_window_open(window) && !Window_Manager::is_window_on_top(window)) {
        Window_Manager::bring_window_to_top(window);
    } else {
        window->toggle_on();
    }
}

void GUI_Manager::gui_nav_back_controller () {
    set_gui_mode("controller");

    if (Engine::mutable_info_selected()) {
        Engine::clear_mutable_info();
    } else if (Window_Manager::is_any_window_open()) {
        Window_Manager::close_top_window();
    }
}

void GUI_Manager::gui_nav_toggle_menu_controller () {
    set_gui_mode("controller");

    string window_name = "main_menu";

    if (Game_Manager::in_progress) {
        window_name = "ingame_menu";
    }

    Window* window = Window_Manager::get_window(window_name);

    if (Window_Manager::is_window_open(window) && !Window_Manager::is_window_on_top(window)) {
        Window_Manager::bring_window_to_top(window);
    } else {
        window->toggle_on();
    }
}

void GUI_Manager::gui_nav_back_keyboard_and_mouse () {
    if (gui_mode == "controller") {
        set_gui_mode("mouse");
    }

    if (Engine::mutable_info_selected()) {
        Engine::clear_mutable_info();
    } else if (Window_Manager::is_any_window_open()) {
        Window_Manager::close_top_window();
    } else {
        if (Game_Manager::in_progress) {
            Window_Manager::get_window("ingame_menu")->toggle_on();
        } else {
            Window_Manager::get_window("main_menu")->toggle_on();
        }
    }
}

void GUI_Manager::gui_nav_confirm (string gui_type) {
    set_gui_mode(gui_type);

    confirm_gui_object();
}

void GUI_Manager::gui_nav_up (string gui_type) {
    set_gui_mode(gui_type);

    change_gui_selected_object("up");
}

void GUI_Manager::gui_nav_down (string gui_type) {
    set_gui_mode(gui_type);

    change_gui_selected_object("down");
}

void GUI_Manager::gui_nav_left () {
    if (Engine::mutable_info_selected()) {
        Engine::ptr_mutable_info->move_cursor_left();
    }
}

void GUI_Manager::gui_nav_right () {
    if (Engine::mutable_info_selected()) {
        Engine::ptr_mutable_info->move_cursor_right();
    }
}

void GUI_Manager::gui_scroll_up (string gui_type) {
    set_gui_mode(gui_type);

    scroll_gui_object_up();
}

void GUI_Manager::gui_scroll_down (string gui_type) {
    set_gui_mode(gui_type);

    scroll_gui_object_down();
}

void GUI_Manager::animate_gui_selector_chasers () {
    Engine_Rect object_pos = get_gui_selected_object_pos();

    if (object_pos.x != -1) {
        double distance_to_cover = object_pos.w * 2 + object_pos.h * 2;
        double move_speed = distance_to_cover / (5760.0 / (double) Engine::UPDATE_RATE);

        for (int i = 0; i < gui_selector_chasers.size(); i++) {
            if (gui_selector_chasers[i].x > (double) object_pos.x &&
                gui_selector_chasers[i].x < (double) object_pos.x + (double) object_pos.w &&
                gui_selector_chasers[i].y == (double) object_pos.y) {
                gui_selector_chasers[i].x += move_speed;

                if (gui_selector_chasers[i].x >= (double) object_pos.x + (double) object_pos.w) {
                    gui_selector_chasers[i].x = (double) object_pos.x + (double) object_pos.w;
                    gui_selector_chasers[i].y += move_speed;
                }
            } else if (gui_selector_chasers[i].x > (double) object_pos.x &&
                       gui_selector_chasers[i].x < (double) object_pos.x + (double) object_pos.w &&
                       gui_selector_chasers[i].y == (double) object_pos.y + (double) object_pos.h) {
                gui_selector_chasers[i].x -= move_speed;

                if (gui_selector_chasers[i].x <= (double) object_pos.x) {
                    gui_selector_chasers[i].x = (double) object_pos.x;
                    gui_selector_chasers[i].y -= move_speed;
                }
            } else if (gui_selector_chasers[i].y > (double) object_pos.y &&
                       gui_selector_chasers[i].y < (double) object_pos.y + (double) object_pos.h &&
                       gui_selector_chasers[i].x == (double) object_pos.x) {
                gui_selector_chasers[i].y -= move_speed;

                if (gui_selector_chasers[i].y <= (double) object_pos.y) {
                    gui_selector_chasers[i].y = (double) object_pos.y;
                    gui_selector_chasers[i].x += move_speed;
                }
            } else if (gui_selector_chasers[i].y > (double) object_pos.y &&
                       gui_selector_chasers[i].y < (double) object_pos.y + (double) object_pos.h &&
                       gui_selector_chasers[i].x == (double) object_pos.x + (double) object_pos.w) {
                gui_selector_chasers[i].y += move_speed;

                if (gui_selector_chasers[i].y >= (double) object_pos.y + (double) object_pos.h) {
                    gui_selector_chasers[i].y = (double) object_pos.y + (double) object_pos.h;
                    gui_selector_chasers[i].x -= move_speed;
                }
            } else {
                if (i == 0) {
                    gui_selector_chasers[i].x = (double) object_pos.x + move_speed;
                    gui_selector_chasers[i].y = (double) object_pos.y;
                } else if (i == 1) {
                    gui_selector_chasers[i].x = (double) object_pos.x + (double) object_pos.w - move_speed;
                    gui_selector_chasers[i].y = (double) object_pos.y + (double) object_pos.h;
                }
            }
        }
    }
}

void GUI_Manager::animate () {
    animate_gui_selector_chasers();

    text_selector.animate();
}

void GUI_Manager::render_gui_selector () {
    Engine_Rect object_pos = get_gui_selected_object_pos();

    if (object_pos.x != -1) {
        if (Engine_Data::gui_selector_style == "standard" || Engine_Data::gui_selector_style == "corners" ||
            Engine_Data::gui_selector_style == "chasers") {
            double thickness = Engine_Data::gui_border_thickness;
            double offset = Engine_Data::gui_border_thickness;

            if (Engine::mutable_info_selected() && !Engine::is_console_selected() && !Engine::is_chat_selected()) {
                thickness += 2.0;
                offset += 2.0;
            }

            Render::render_rectangle_empty(object_pos.x - offset, object_pos.y - offset, object_pos.w + offset * 2.0,
                                           object_pos.h + offset * 2.0, 1.0,
                                           Engine::current_color_theme()->gui_selector_1, thickness);
        }

        if (Engine_Data::gui_selector_style == "corners") {
            double corner_size = Engine_Data::gui_border_thickness + 4.0;

            if (Engine::mutable_info_selected() && !Engine::is_console_selected() && !Engine::is_chat_selected()) {
                corner_size += 2.0;
            }

            Render::render_rectangle(object_pos.x - corner_size / 2.0 + 1, object_pos.y - corner_size / 2.0 + 1,
                                     corner_size, corner_size, 1.0, Engine::current_color_theme()->gui_selector_2);
            Render::render_rectangle(object_pos.x + object_pos.w - corner_size / 2.0 - 1,
                                     object_pos.y - corner_size / 2.0 + 1, corner_size, corner_size, 1.0,
                                     Engine::current_color_theme()->gui_selector_2);
            Render::render_rectangle(object_pos.x - corner_size / 2.0 + 1,
                                     object_pos.y + object_pos.h - corner_size / 2.0 - 1, corner_size, corner_size, 1.0,
                                     Engine::current_color_theme()->gui_selector_2);
            Render::render_rectangle(object_pos.x + object_pos.w - corner_size / 2.0 - 1,
                                     object_pos.y + object_pos.h - corner_size / 2.0 - 1, corner_size, corner_size, 1.0,
                                     Engine::current_color_theme()->gui_selector_2);
        }

        if (Engine_Data::gui_selector_style == "underline") {
            double thickness = Engine_Data::gui_border_thickness + 1.0;

            if (Engine::mutable_info_selected() && !Engine::is_console_selected() && !Engine::is_chat_selected()) {
                thickness += 3.0;
            }

            for (int i = 0; i < thickness; i++) {
                Render::render_line(object_pos.x, object_pos.y + object_pos.h + i, object_pos.x + object_pos.w - 1,
                                    object_pos.y + object_pos.h + i, 1.0,
                                    Engine::current_color_theme()->gui_selector_1);
            }
        }

        if (Engine_Data::gui_selector_style == "chasers") {
            double chaser_size = Engine_Data::gui_border_thickness + 4.0;

            if (Engine::mutable_info_selected() && !Engine::is_console_selected() && !Engine::is_chat_selected()) {
                chaser_size += 2.0;
            }

            for (int i = 0; i < gui_selector_chasers.size(); i++) {
                Render::render_rectangle(gui_selector_chasers[i].x - chaser_size / 2.0,
                                         gui_selector_chasers[i].y - chaser_size / 2.0, chaser_size, chaser_size, 1.0,
                                         Engine::current_color_theme()->gui_selector_2);
            }
        }
    }
}
