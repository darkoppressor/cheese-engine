/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "window_manager.h"
#include "../data/data_reader.h"
#include "../strings/engine_strings.h"
#include "../log.h"
#include "../engine.h"
#include "../collision/collision.h"
#include "../data/engine_data.h"
#include "gui_manager.h"

#include <boost/algorithm/string.hpp>

using namespace std;

vector<Window> Window_Manager::windows;
vector<Window*> Window_Manager::window_z_order;
Window* Window_Manager::window_under_mouse = 0;

void Window_Manager::unload_data () {
    windows.clear();

    window_z_order.clear();
    window_under_mouse = 0;
}

void Window_Manager::load_window (File_IO_Load* load) {
    windows.push_back(Window());

    vector<string> lines = Data_Reader::read_data(load, "</window>");

    for (size_t i = 0; i < lines.size(); i++) {
        string& line = lines[i];

        if (Data_Reader::check_prefix(line, "name:")) {
            windows.back().name = line;
        } else if (Data_Reader::check_prefix(line, "starting_position:")) {
            vector<string> start_coords;

            boost::algorithm::split(start_coords, line, boost::algorithm::is_any_of(","));

            windows.back().x = Strings::string_to_long(start_coords[0]);
            windows.back().y = Strings::string_to_long(start_coords[1]);

            windows.back().start_x = windows.back().x;
            windows.back().start_y = windows.back().y;
        } else if (Data_Reader::check_prefix(line, "starting_size:")) {
            vector<string> start_dimensions;

            boost::algorithm::split(start_dimensions, line, boost::algorithm::is_any_of(","));

            windows.back().start_width = Strings::string_to_long(start_dimensions[0]);
            windows.back().start_height = Strings::string_to_long(start_dimensions[1]);
        } else if (Data_Reader::check_prefix(line, "width:")) {
            windows.back().w = Strings::string_to_long(line);
        } else if (Data_Reader::check_prefix(line, "height:")) {
            windows.back().h = Strings::string_to_long(line);
        } else if (Data_Reader::check_prefix(line, "fit_content:")) {
            windows.back().fit_content = Strings::string_to_bool(line);
        } else if (Data_Reader::check_prefix(line, "title:")) {
            windows.back().title = Strings::process_newlines(line);
        } else if (Data_Reader::check_prefix(line, "font:")) {
            windows.back().font = line;
        } else if (Data_Reader::check_prefix(line, "font_color:")) {
            windows.back().font_color = line;
        } else if (Data_Reader::check_prefix(line, "border:")) {
            windows.back().border.name = line;
        } else if (Data_Reader::check_prefix(line, "background:")) {
            windows.back().background.name = line;
        } else if (Data_Reader::check_prefix(line, "scrolling_buttons:")) {
            windows.back().scrolling_buttons = line;
        } else if (Data_Reader::check_prefix(line, "close_function:")) {
            windows.back().close_function = line;
        } else if (Data_Reader::check_prefix(line, "disallow_closing:")) {
            windows.back().disallow_closing = Strings::string_to_bool(line);
        } else if (Data_Reader::check_prefix(line, "exclusive:")) {
            windows.back().exclusive = Strings::string_to_bool(line);
        } else if (Data_Reader::check_prefix(line, "no_button_sort:")) {
            windows.back().no_button_sort = Strings::string_to_bool(line);
        } else if (Data_Reader::check_prefix(line, "<info>")) {
            i = load_information(lines, i + 1);
        } else if (Data_Reader::check_prefix(line, "<button>")) {
            i = load_button(lines, i + 1);
        }
    }

    windows.back().set_dimensions();
    windows.back().center_in_game_window();
}

size_t Window_Manager::load_information (vector<string>& lines, size_t line_index) {
    windows.back().informations.push_back(Information());

    for (size_t i = line_index; i < lines.size(); i++) {
        string& line = lines[i];

        if (Data_Reader::check_prefix(line, "location:")) {
            vector<string> location;

            boost::algorithm::split(location, line, boost::algorithm::is_any_of(","));

            windows.back().informations.back().x = Strings::string_to_long(location[0]);
            windows.back().informations.back().y = Strings::string_to_long(location[1]);

            windows.back().informations.back().start_x = windows.back().informations.back().x;
            windows.back().informations.back().start_y = windows.back().informations.back().y;
        } else if (Data_Reader::check_prefix(line, "text:")) {
            windows.back().informations.back().text = Strings::process_newlines(line);
        } else if (Data_Reader::check_prefix(line, "colored_text:")) {
            windows.back().informations.back().set_colored_text(Strings::process_newlines(line));
        } else if (Data_Reader::check_prefix(line, "tooltip:")) {
            windows.back().informations.back().tooltip_text = Strings::process_newlines(line);
        } else if (Data_Reader::check_prefix(line, "mutable:")) {
            windows.back().informations.back().text_mutable = Strings::string_to_bool(line);
        } else if (Data_Reader::check_prefix(line, "max_text_length:")) {
            windows.back().informations.back().max_text_length = Strings::string_to_long(line);
        } else if (Data_Reader::check_prefix(line, "allowed_input:")) {
            vector<string> inputs;

            boost::algorithm::split(inputs, line, boost::algorithm::is_any_of(","));

            windows.back().informations.back().allowed_input = inputs;
        } else if (Data_Reader::check_prefix(line, "scrolling:")) {
            windows.back().informations.back().scrolling = Strings::string_to_bool(line);
        } else if (Data_Reader::check_prefix(line, "box:")) {
            vector<string> scroll_dimensions;

            boost::algorithm::split(scroll_dimensions, line, boost::algorithm::is_any_of(","));

            windows.back().informations.back().scroll_width = Strings::string_to_long(scroll_dimensions[0]);
            windows.back().informations.back().scroll_height = Strings::string_to_long(scroll_dimensions[1]);
        } else if (Data_Reader::check_prefix(line, "sprite:")) {
            windows.back().informations.back().sprite.name = line;
        } else if (Data_Reader::check_prefix(line, "background_type:")) {
            windows.back().informations.back().background_type = line;
        } else if (Data_Reader::check_prefix(line, "background_opacity:")) {
            windows.back().informations.back().background_opacity = Strings::string_to_double(line);
        } else if (Data_Reader::check_prefix(line, "font_color:")) {
            windows.back().informations.back().font_color = line;
        } else if (Data_Reader::check_prefix(line, "special_text:")) {
            windows.back().informations.back().special_info_text = line;
        } else if (Data_Reader::check_prefix(line, "special_sprite:")) {
            windows.back().informations.back().special_info_sprite = line;
        } else if (Data_Reader::check_prefix(line, "font:")) {
            windows.back().informations.back().font = line;
        } else if (Data_Reader::check_prefix(line, "</info>")) {
            windows.back().informations.back().reset_cursor_position();
            windows.back().informations.back().set_dimensions();
            windows.back().informations.back().center_in_window(windows.back().w, windows.back().h);

            return i;
        }
    }

    return lines.size() - 1;
}

size_t Window_Manager::load_button (vector<string>& lines, size_t line_index) {
    windows.back().buttons.push_back(Button());

    for (size_t i = line_index; i < lines.size(); i++) {
        string& line = lines[i];

        if (Data_Reader::check_prefix(line, "location:")) {
            vector<string> location;

            boost::algorithm::split(location, line, boost::algorithm::is_any_of(","));

            windows.back().buttons.back().x = Strings::string_to_long(location[0]);
            windows.back().buttons.back().y = Strings::string_to_long(location[1]);

            windows.back().buttons.back().start_x = windows.back().buttons.back().x;
            windows.back().buttons.back().start_y = windows.back().buttons.back().y;
        } else if (Data_Reader::check_prefix(line, "text:")) {
            windows.back().buttons.back().text = Strings::process_newlines(line);
        } else if (Data_Reader::check_prefix(line, "tooltip:")) {
            windows.back().buttons.back().tooltip_text = Strings::process_newlines(line);
        } else if (Data_Reader::check_prefix(line, "font:")) {
            windows.back().buttons.back().font = line;
        } else if (Data_Reader::check_prefix(line, "font_color:")) {
            windows.back().buttons.back().font_color = line;
        } else if (Data_Reader::check_prefix(line, "states:")) {
            vector<string> states;

            boost::algorithm::split(states, line, boost::algorithm::is_any_of(","));

            for (int n = 0; n < states.size(); n++) {
                windows.back().buttons.back().states.push_back(states[n]);
            }
        } else if (Data_Reader::check_prefix(line, "event:")) {
            windows.back().buttons.back().event_function = line;
        } else if (Data_Reader::check_prefix(line, "alt_event1:")) {
            windows.back().buttons.back().alt_function1 = line;
        } else if (Data_Reader::check_prefix(line, "alt_event2:")) {
            windows.back().buttons.back().alt_function2 = line;
        } else if (Data_Reader::check_prefix(line, "alt_event3:")) {
            windows.back().buttons.back().alt_function3 = line;
        } else if (Data_Reader::check_prefix(line, "sound_moused:")) {
            windows.back().buttons.back().mouse_over_sound = line;
        } else if (Data_Reader::check_prefix(line, "sound_click:")) {
            windows.back().buttons.back().event_fire_sound = line;
        } else if (Data_Reader::check_prefix(line, "sprite:")) {
            windows.back().buttons.back().sprite.name = line;
        } else if (Data_Reader::check_prefix(line, "sprite_moused:")) {
            windows.back().buttons.back().sprite_moused.name = line;
        } else if (Data_Reader::check_prefix(line, "sprite_click:")) {
            windows.back().buttons.back().sprite_click.name = line;
        } else if (Data_Reader::check_prefix(line, "text_sprite:")) {
            windows.back().buttons.back().text_sprite.name = line;
        } else if (Data_Reader::check_prefix(line, "</button>")) {
            windows.back().buttons.back().set_dimensions();
            windows.back().buttons.back().center_in_window(windows.back().w, windows.back().h);

            return i;
        }
    }

    return lines.size() - 1;
}

Window* Window_Manager::get_window (string name) {
    Window* ptr_object = 0;

    for (size_t i = 0; i < windows.size(); i++) {
        if (windows[i].name == name) {
            ptr_object = &windows[i];

            break;
        }
    }

    if (ptr_object == 0) {
        Log::add_error("Error accessing window '" + name + "'");
    }

    return ptr_object;
}

void Window_Manager::create_window_close_buttons () {
    for (size_t i = 0; i < windows.size(); i++) {
        windows[i].create_close_button();
        windows[i].set_last_normal_button();
    }
}

void Window_Manager::rebuild_window_data () {
    for (size_t i = 0; i < windows.size(); i++) {
        if (windows[i].need_to_rebuild_scrolling_buttons) {
            windows[i].need_to_rebuild_scrolling_buttons = false;

            windows[i].build_scrolling_buttons();
        }
    }
}

bool Window_Manager::is_window_open (Window* window) {
    for (size_t i = 0; i < window_z_order.size(); i++) {
        if (window_z_order[i] == window) {
            return true;
        }
    }

    return false;
}

bool Window_Manager::is_window_on_top (Window* window) {
    if (window_z_order.size() > 0 && window_z_order[0] == window) {
        return true;
    }

    return false;
}

Window* Window_Manager::get_top_window () {
    if (window_z_order.size() > 0) {
        return window_z_order[0];
    }

    return 0;
}

void Window_Manager::open_window (Window* window) {
    if (!is_window_open(window)) {
        window_z_order.insert(window_z_order.begin(), window);

        GUI_Manager::reset_gui_selected_object();
    }
}

void Window_Manager::close_window (Window* window) {
    for (size_t i = 0; i < window_z_order.size(); i++) {
        if (window_z_order[i] == window) {
            window_z_order.erase(window_z_order.begin() + i);

            GUI_Manager::reset_gui_selected_object();

            break;
        }
    }
}

void Window_Manager::bring_window_to_top (Window* window) {
    if (is_window_open(window)) {
        close_window(window);

        open_window(window);
    }
}

bool Window_Manager::is_exclusive_window_open () {
    for (int i = 0; i < windows.size(); i++) {
        if (windows[i].on && windows[i].exclusive) {
            return true;
        }
    }

    return false;
}

bool Window_Manager::is_any_window_open () {
    for (int i = 0; i < windows.size(); i++) {
        if (windows[i].on) {
            return true;
        }
    }

    return false;
}

int Window_Manager::open_window_count () {
    int open_windows = 0;

    for (int i = 0; i < windows.size(); i++) {
        if (windows[i].on) {
            open_windows++;
        }
    }

    return open_windows;
}

void Window_Manager::close_all_windows (Window* ignore) {
    for (int i = 0; i < windows.size(); i++) {
        if (&windows[i] != ignore) {
            windows[i].toggle_on(true, false);
        }
    }
}

void Window_Manager::close_top_window () {
    if (is_any_window_open()) {
        Window* window = window_z_order[0];

        if (!window->disallow_closing) {
            window->toggle_on(true, false);
        }
    }
}

bool Window_Manager::is_mouse_over_window () {
    if (window_under_mouse != 0) {
        return true;
    } else {
        return false;
    }
}

bool Window_Manager::is_mouse_over_open_window () {
    if (window_under_mouse != 0 && is_window_open(window_under_mouse)) {
        return true;
    } else {
        return false;
    }
}

void Window_Manager::prepare_for_input () {
    window_under_mouse = 0;

    vector<int> closed_windows;

    for (int i = 0; i < windows.size(); i++) {
        if (!is_window_open(&windows[i])) {
            closed_windows.push_back(i);
        }
    }

    int mouse_x = 0;
    int mouse_y = 0;

    Engine::get_mouse_state(&mouse_x, &mouse_y);

    for (int i = 0; i < window_z_order.size() && window_under_mouse == 0; i++) {
        Collision_Rect<double> box_a(mouse_x, mouse_y, Engine_Data::cursor_width, Engine_Data::cursor_height);
        Collision_Rect<double> box_b(window_z_order[i]->x, window_z_order[i]->y, window_z_order[i]->w,
                                     window_z_order[i]->h);

        if (Collision::check_rect(box_a, box_b)) {
            window_under_mouse = window_z_order[i];
        }
    }

    for (int i = 0; i < closed_windows.size() && window_under_mouse == 0; i++) {
        Collision_Rect<double> box_a(mouse_x, mouse_y, Engine_Data::cursor_width, Engine_Data::cursor_height);
        Collision_Rect<double> box_b(windows[closed_windows[i]].x, windows[closed_windows[i]].y,
                                     windows[closed_windows[i]].w, windows[closed_windows[i]].h);

        if (Collision::check_rect(box_a, box_b)) {
            window_under_mouse = &windows[closed_windows[i]];
        }
    }
}

void Window_Manager::handle_input_states () {
    Window* top_window = Window_Manager::get_top_window();

    for (size_t i = 0; i < windows.size(); i++) {
        if (top_window == 0 || top_window != &windows[i]) {
            windows[i].reset_buttons_moused_over();
        }
    }

    if (top_window != 0) {
        top_window->handle_input_states();
    }
}

void Window_Manager::animate () {
    for (size_t i = 0; i < windows.size(); i++) {
        windows[i].animate();
    }
}

void Window_Manager::render () {
    vector<int> closed_windows;

    for (int i = 0; i < windows.size(); i++) {
        if (!is_window_open(&windows[i])) {
            closed_windows.push_back(i);
        }
    }

    for (int i = 0; i < closed_windows.size(); i++) {
        windows[closed_windows[i]].render();
    }

    for (int i = window_z_order.size() - 1; i >= 0; i--) {
        window_z_order[i]->render();
    }
}
