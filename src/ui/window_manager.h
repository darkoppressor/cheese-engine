/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef window_manager_h
#define window_manager_h

#include "../file/file_io.h"
#include "window.h"

#include <vector>
#include <string>

class Window_Manager {
    private:
        static std::vector<Window> windows;

    public:
        // The z order of all open windows
        static std::vector<Window*> window_z_order;
        // The window that the mouse is currently over
        static Window* window_under_mouse;

        static void unload_data();
        static void load_window(File_IO_Load* load);
        static size_t load_information(std::vector<std::string>& lines, size_t line_index);
        static size_t load_button(std::vector<std::string>& lines, size_t line_index);
        static Window* get_window(std::string name);
        static void create_window_close_buttons();

        // Rebuild any window data if needed
        static void rebuild_window_data();
        static bool is_window_open(Window* window);
        static bool is_window_on_top(Window* window);
        static Window* get_top_window();
        static void open_window(Window* window);
        static void close_window(Window* window);
        static void bring_window_to_top(Window* window);
        static bool is_exclusive_window_open();
        static bool is_any_window_open();
        static int open_window_count();
        static void close_all_windows(Window* ignore = 0);
        static void close_top_window();
        static bool is_mouse_over_window();
        static bool is_mouse_over_open_window();
        static void prepare_for_input();
        static void handle_input_states();
        static void animate();
        static void render();
};

#endif
