/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "font.h"
#include "../game_window.h"
#include "../options/options.h"
#include "../render/render.h"
#include "../image/image_manager.h"
#include "../data/object_manager.h"

using namespace std;

const SDL_Rect Bitmap_Font::FONT_DEFAULT_ALLOWED_AREA = {-1, -1, 0, 0};

Bitmap_Font::Bitmap_Font () {
    gui_padding_x = 0;
    gui_padding_y = 0;

    shadow_distance = 0;

    name = "";

    spacing_x = 0;
    spacing_y = 0;
}

void Bitmap_Font::build_font () {
    // Set the cell dimensions
    double cell_width = sprite.get_width() / 256.0;
    double cell_height = sprite.get_height();

    // The current character we are setting
    int current_char = 0;

    // Go through the cell columns
    for (int cols = 0; cols < 256; cols++) {
        // Set the character offset
        chars[current_char].x = cell_width * cols;
        chars[current_char].y = 0.0;

        // Set the dimensions of the character
        chars[current_char].w = cell_width;
        chars[current_char].h = cell_height;

        // Go to the next character
        current_char++;
    }
}

double Bitmap_Font::get_letter_width () const {
    return sprite.get_width() / 256.0;
}

double Bitmap_Font::get_letter_height () const {
    return sprite.get_height();
}

int Bitmap_Font::get_gui_padding_x () const {
    return gui_padding_x;
}

int Bitmap_Font::get_gui_padding_y () const {
    return gui_padding_y;
}

void Bitmap_Font::set_sprite (string sprite_name) {
    sprite.name = sprite_name;
}

void Bitmap_Font::set_gui_padding_x (int x) {
    gui_padding_x = x;
}

void Bitmap_Font::set_gui_padding_y (int y) {
    gui_padding_y = y;
}

void Bitmap_Font::set_shadow_distance (int distance) {
    shadow_distance = distance;
}

void Bitmap_Font::show (double x, double y, string text, string font_color, double opacity, double scale_x,
                        double scale_y, double angle, const SDL_Rect& allowed_area,
                        const vector<string>& character_colors) {
    show(x, y, text, Object_Manager::get_color(font_color), opacity, scale_x, scale_y, angle, allowed_area,
         character_colors);
}

void Bitmap_Font::show (double x, double y, string text, Color* font_color, double opacity, double scale_x,
                        double scale_y, double angle, const SDL_Rect& allowed_area,
                        const vector<string>& character_colors) {
    // Temporary offsets
    double X = x, Y = y;
    double real_spacing_x = spacing_x * scale_x;
    double real_spacing_y = spacing_y * scale_y;

    // Go through the text
    for (int show = 0; text[show] != '\0'; show++) {
        // Get the ASCII value of the character
        short ascii = (unsigned char) text[show];

        if (text[show] != '\xA') {
            if (X + get_letter_width() * scale_x >= 0 && X <= Game_Window::width() &&
                Y + get_letter_height() * scale_y >= 0 && Y <= Game_Window::height()) {
                bool allowed_area_present = false;

                if (allowed_area.x != -1 || allowed_area.y != -1 || allowed_area.w != 0 || allowed_area.h != 0) {
                    allowed_area_present = true;
                }

                if (!allowed_area_present ||
                    (allowed_area_present && X >= allowed_area.x &&
                     X + get_letter_width() * scale_x <= allowed_area.x + allowed_area.w && Y >= allowed_area.y &&
                     Y + get_letter_height() * scale_y <= allowed_area.y + allowed_area.h)) {
                    if (shadow_distance != 0 && Options::font_shadows) {
                        // Render the shadow
                        Render::render_sprite(X + shadow_distance, Y + shadow_distance,
                                              Image_Manager::get_image(sprite.name), &chars[ascii], opacity, scale_x,
                                              scale_y, angle, "ui_black");
                    }

                    Color* character_color = font_color;

                    if (character_colors.size() == text.length() && character_colors[show].length() > 0) {
                        character_color = Object_Manager::get_color(character_colors[show]);
                    }

                    // Render the character
                    Render::render_sprite(X, Y, Image_Manager::get_image(sprite.name), &chars[ascii], opacity, scale_x,
                                          scale_y, angle, character_color, false, false);
                }
            }

            // Move over the width of the character with one pixel of padding
            X += real_spacing_x;
        } else {
            Y += real_spacing_y;
            X = x;
        }
    }
}
