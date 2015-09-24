/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef window_h
#define window_h

#include "button.h"
#include "information.h"
#include "sprite.h"

#include <vector>
#include <string>

class Window{
public:

    std::string name;

    //The coordinate location and dimensions
    int x;
    int y;
    int w;
    int h;

    //The starting coordinates
    int start_x;
    int start_y;

    //The starting size
    //This is only used when fit_content is true
    int start_width;
    int start_height;

    bool fit_content;

    //The offsets from the mouse to the edge of the window
    //Used for window movement
    int mouse_offset_x,mouse_offset_y;

    std::string title;

    std::string font;
    std::string font_color;

    Sprite border;

    Sprite background;

    //If true, the window is processed
    //If false, the window is not processed
    bool on;

    //If true, the window is currently being dragged with the mouse
    //If false, the window is not moving
    bool moving;

    //If true, do not allow standard means of closing this window
    //If false, this window can close normally
    bool disallow_closing;

    //If true, do not allow other windows to be opened while this window is open
    //If false, allow other windows
    bool exclusive;

    //Prevent buttons from being sorted by their y position for GUI nav purposes
    bool no_button_sort;

    //If this string is not empty, it identifies which scrolling buttons list this window should display
    std::string scrolling_buttons;

    //If this string is not empty, it identifies what the close function should do
    std::string close_function;

    //The last button that is NOT part of the scrolling button list
    int last_normal_button;

    int scroll_offset;

    bool need_to_rebuild_scrolling_buttons;

    //All of the information pieces that this window has
    std::vector<Information> informations;

    //All of the buttons that this window has
    std::vector<Button> buttons;

    Window();

    //Returns the text from the mutable info represented by the passed number
    //This number relates only to mutable infos
    //Returns an empty string if the passed number does not correspond to a mutable info
    std::string get_info_text(int mutable_info_number);

    //Returns the state data from the stated button represented by the passed number
    //This number relates only to stated buttons
    //Returns an empty string if the passed number does not correspond to a stated button
    std::string get_stated_button_state(int stated_button_number);

    //Sets the text/tooltip in the mutable info represented by the passed number
    //This number relates only to mutable infos
    void set_info_text(int mutable_info_number,std::string get_text);
    void set_info_tooltip(int mutable_info_number,std::string get_text);

    //Sets the state data/tooltip in the stated button represented by the passed number
    //This number relates only to stated buttons
    void set_stated_button_state_index(int stated_button_number,uint32_t get_state_index);
    void set_stated_button_tooltip(int stated_button_number,std::string get_text);

    void set_default_font();

    void set_dimensions();

    void create_close_button();

    void exec_close_function();

    //This should be called only once, after creating all buttons including the close button, but before the first call to build_scrolling_buttons()
    void set_last_normal_button();

    //Returns true if the button with the passed index is part of the scrolling list
    //Returns false if the button is a normal button or does not exist
    bool is_button_scrolling(int index);

    //Returns the y offset for the scrolling button with passed index
    //Returns -1 if the button is a normal button or does not exist
    int get_scrolling_button_offset(int index);

    //Returns the position in the list of scrolling buttons for the scrolling button with passed index
    //Returns -1 if the button is a normal button or does not exist
    int get_scrolling_button_position(int index);

    //Returns the number of scrolling buttons in the scrolling buttons list
    int get_scrolling_button_count();

    //Returns true if this button is a scrolling button and is currently being displayed on-screen
    //Returns false if this button is not being displayed on-screen, is not a scrolling button, or does not exist
    bool is_scrolling_button_on_screen(int index);

    //Returns true if this scrolling button is above the scrolling button area
    //Returns false if this scrolling button is below the scrolling button area
    //Also returns false if this button is within the scrolling button area, is not a scrolling button, or doesn't exist
    bool off_screen_scrolling_button_direction(int index);

    //Call this to cause build_scrolling_buttons to be called when it is safe
    void rebuild_scrolling_buttons();
    //Do NOT call this on its own. Use rebuild_scrolling_buttons()
    void build_scrolling_buttons();

    void center_in_game_window();

    void reset_scrolling_lists();

    void toggle_on(bool force=false,bool force_value=false);

    //Resets the moused over state of all buttons
    void reset_buttons_moused_over();

    void scroll_up();
    void scroll_down();

    void handle_input_states();

    //Returns true if the event was consumed,
    //false otherwise
    bool handle_input_events();

    void animate();
    void render();
};

#endif
