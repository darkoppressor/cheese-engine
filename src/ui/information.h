/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef information_h
#define information_h

#include "../sprite/sprite.h"

#include <string>
#include <vector>

class Information {
    public:
        // The coordinate location
        // This location, as used by a window, is an offset from the window, not the actual screen
        int x;
        int y;
        int w;
        int h;

        // The starting coordinates
        int start_x;
        int start_y;

        // Only one of text, special_info_text, special_info_sprite, and sprite should be used
        std::string text;
        std::string tooltip_text;

        // Holds strings for character color overrides in text
        // An empty string means no override for that character
        // The size of this vector must equal the length of text
        std::vector<std::string> text_character_colors;

        // The position of the text cursor
        // Only applicable if the text is mutable
        int cursor_position;

        // If true, text is mutable
        // If false, text is immutable
        bool text_mutable;
        // The maximum length of text when it is mutable
        int max_text_length;
        // The kind of text input(s) allowed for a mutable string
        std::vector<std::string> allowed_input;
        bool scrolling;
        // The dimensions (in characters for width and lines for height) of the scroll box
        // Obviously only applies if scrolling is true
        int scroll_width;
        int scroll_height;
        int scroll_offset;
        std::string special_info_text;
        std::string special_info_sprite;
        Sprite sprite;
        std::string background_type;
        double background_opacity;
        std::string font;
        std::string font_color;

        Information ();

        void set_default_font();

        void set_dimensions();

        void center_in_window(int window_width, int window_height);

        void reset_cursor_position();
        void check_cursor_position();
        void move_cursor_left();
        void move_cursor_right();

        // Returns a string with the text of the line the cursor is currently on
        std::string get_cursor_line();

        // Ensures that text length does not exceed max_text_length
        void check_text();

        void add_text(std::string get_text);
        void input_backspace();
        void input_delete();
        void input_newline();

        void set_text(std::string get_text);

        void set_colored_text(std::string get_text);

        void set_sprite(std::string sprite_name);

        // If there is special info text, this function sets the text
        // This is used to create strings that take some runtime stuff into account
        void set_special_text();

        // If there is a special info sprite, this function sets the sprite
        // This is used to create sprites that take some runtime stuff into account
        void set_special_sprite();

        // Returns true if the passed input type is allowed
        bool allows_input(std::string input_type);

        void begin_editing();

        void scroll_up(int y_offset);
        void scroll_down(int y_offset);
        void scroll_to_cursor();

        void handle_input_states(int mouse_x, int mouse_y, int x_offset, int y_offset);

        // Returns true if the event was consumed,
        // false otherwise
        bool handle_input_events(int mouse_x, int mouse_y, int x_offset, int y_offset);

        void animate();
        void render(int x_offset, int y_offset);
};

#endif
