/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "object_manager.h"
#include "data_reader.h"
#include "engine_strings.h"
#include "game_constants_loader.h"
#include "sound_manager.h"

#include <boost/algorithm/string.hpp>

using namespace std;

vector<Color> Object_Manager::colors;
vector<Animation> Object_Manager::animations;
vector<Bitmap_Font> Object_Manager::fonts;
vector<Color_Theme> Object_Manager::color_themes;
vector<Game_Command> Object_Manager::game_commands;
vector<Game_Option> Object_Manager::game_options;
vector<Cursor> Object_Manager::cursors;
void Object_Manager::unload_data () {
    for (size_t i = 0; i < cursors.size(); i++) {
        cursors[i].free_hw_cursor();
    }

    colors.clear();
    animations.clear();
    fonts.clear();
    color_themes.clear();
    game_commands.clear();
    game_options.clear();
    cursors.clear();
}

void Object_Manager::load_hw_cursors () {
    for (size_t i = 0; i < cursors.size(); i++) {
        cursors[i].load_hw_cursor();
    }
}

void Object_Manager::load_color (File_IO_Load* load) {
    colors.push_back(Color());

    vector<string> lines = Data_Reader::read_data(load, "</color>");

    for (size_t i = 0; i < lines.size(); i++) {
        string& line = lines[i];

        if (Data_Reader::check_prefix(line, "name:")) {
            colors.back().name = line;
        } else if (Data_Reader::check_prefix(line, "rgb:")) {
            vector<string> rgb_values;
            boost::algorithm::split(rgb_values, line, boost::algorithm::is_any_of(","));

            if (rgb_values.size() == 3) {
                colors.back().set(Strings::string_to_long(rgb_values[0]), Strings::string_to_long(
                                      rgb_values[1]), Strings::string_to_long(rgb_values[2]), 255l);
            }
        }
    }
}

void Object_Manager::load_animation (File_IO_Load* load) {
    animations.push_back(Animation());

    double frame_width = 0.0;
    vector<string> lines = Data_Reader::read_data(load, "</animation>");

    for (size_t i = 0; i < lines.size(); i++) {
        string& line = lines[i];

        if (Data_Reader::check_prefix(line, "name:")) {
            animations.back().name = line;
        } else if (Data_Reader::check_prefix(line, "frame_count:")) {
            animations.back().frame_count = Strings::string_to_long(line);
        } else if (Data_Reader::check_prefix(line, "animation_speed:")) {
            animations.back().animation_speed = Strings::string_to_long(line);
        } else if (Data_Reader::check_prefix(line, "frame_width:")) {
            frame_width = Strings::string_to_double(line);
        } else if (Data_Reader::check_prefix(line, "end_behavior:")) {
            animations.back().end_behavior = line;
        }
    }

    animations.back().setup(frame_width);
}

void Object_Manager::load_font (File_IO_Load* load) {
    fonts.push_back(Bitmap_Font());

    vector<string> lines = Data_Reader::read_data(load, "</font>");

    for (size_t i = 0; i < lines.size(); i++) {
        string& line = lines[i];

        if (Data_Reader::check_prefix(line, "name:")) {
            fonts.back().name = line;
        } else if (Data_Reader::check_prefix(line, "sprite:")) {
            fonts.back().set_sprite(line);
        } else if (Data_Reader::check_prefix(line, "spacing_x:")) {
            fonts.back().spacing_x = Strings::string_to_long(line);
        } else if (Data_Reader::check_prefix(line, "spacing_y:")) {
            fonts.back().spacing_y = Strings::string_to_long(line);
        } else if (Data_Reader::check_prefix(line, "gui_padding_x:")) {
            fonts.back().set_gui_padding_x(Strings::string_to_long(line));
        } else if (Data_Reader::check_prefix(line, "gui_padding_y:")) {
            fonts.back().set_gui_padding_y(Strings::string_to_long(line));
        } else if (Data_Reader::check_prefix(line, "shadow_distance:")) {
            fonts.back().set_shadow_distance(Strings::string_to_long(line));
        }
    }

    fonts.back().build_font();
}

void Object_Manager::load_color_theme (File_IO_Load* load) {
    color_themes.push_back(Color_Theme());

    vector<string> lines = Data_Reader::read_data(load, "</color_theme>");

    for (size_t i = 0; i < lines.size(); i++) {
        string& line = lines[i];

        if (Data_Reader::check_prefix(line, "name:")) {
            color_themes.back().name = line;
        } else if (Data_Reader::check_prefix(line, "window_font:")) {
            color_themes.back().window_font = line;
        } else if (Data_Reader::check_prefix(line, "window_title_bar:")) {
            color_themes.back().window_title_bar = line;
        } else if (Data_Reader::check_prefix(line, "window_background:")) {
            color_themes.back().window_background = line;
        } else if (Data_Reader::check_prefix(line, "window_border:")) {
            color_themes.back().window_border = line;
        } else if (Data_Reader::check_prefix(line, "button_font:")) {
            color_themes.back().button_font = line;
        } else if (Data_Reader::check_prefix(line, "button_background:")) {
            color_themes.back().button_background = line;
        } else if (Data_Reader::check_prefix(line, "button_background_moused:")) {
            color_themes.back().button_background_moused = line;
        } else if (Data_Reader::check_prefix(line, "button_background_click:")) {
            color_themes.back().button_background_click = line;
        } else if (Data_Reader::check_prefix(line, "button_border:")) {
            color_themes.back().button_border = line;
        } else if (Data_Reader::check_prefix(line, "tooltip_font:")) {
            color_themes.back().tooltip_font = line;
        } else if (Data_Reader::check_prefix(line, "tooltip_background:")) {
            color_themes.back().tooltip_background = line;
        } else if (Data_Reader::check_prefix(line, "tooltip_border:")) {
            color_themes.back().tooltip_border = line;
        } else if (Data_Reader::check_prefix(line, "toast_font:")) {
            color_themes.back().toast_font = line;
        } else if (Data_Reader::check_prefix(line, "toast_background:")) {
            color_themes.back().toast_background = line;
        } else if (Data_Reader::check_prefix(line, "toast_border:")) {
            color_themes.back().toast_border = line;
        } else if (Data_Reader::check_prefix(line, "information_font:")) {
            color_themes.back().information_font = line;
        } else if (Data_Reader::check_prefix(line, "information_background:")) {
            color_themes.back().information_background = line;
        } else if (Data_Reader::check_prefix(line, "information_border:")) {
            color_themes.back().information_border = line;
        } else if (Data_Reader::check_prefix(line, "gui_selector_1:")) {
            color_themes.back().gui_selector_1 = line;
        } else if (Data_Reader::check_prefix(line, "gui_selector_2:")) {
            color_themes.back().gui_selector_2 = line;
        }
    }
}

void Object_Manager::load_game_command (File_IO_Load* load) {
    game_commands.push_back(Game_Command());

    vector<string> lines = Data_Reader::read_data(load, "</game_command>");

    for (size_t i = 0; i < lines.size(); i++) {
        string& line = lines[i];

        if (Data_Reader::check_prefix(line, "name:")) {
            game_commands.back().name = line;
        } else if (Data_Reader::check_prefix(line, "title:")) {
            game_commands.back().title = line;
        } else if (Data_Reader::check_prefix(line, "description:")) {
            game_commands.back().description = Strings::process_newlines(line);
        } else if (Data_Reader::check_prefix(line, "developer:")) {
            game_commands.back().dev = Strings::string_to_bool(line);
        } else if (Data_Reader::check_prefix(line, "key:")) {
            game_commands.back().key = SDL_GetScancodeFromName(line.c_str());
        } else if (Data_Reader::check_prefix(line, "controller_button:")) {
            game_commands.back().button = SDL_GameControllerGetButtonFromString(line.c_str());
        } else if (Data_Reader::check_prefix(line, "controller_axis:")) {
            game_commands.back().axis = SDL_GameControllerGetAxisFromString(line.c_str());
        }
    }
}

void Object_Manager::load_game_option (File_IO_Load* load) {
    game_options.push_back(Game_Option());

    string option_default = "";
    vector<string> lines = Data_Reader::read_data(load, "</game_option>");

    for (size_t i = 0; i < lines.size(); i++) {
        string& line = lines[i];

        if (Data_Reader::check_prefix(line, "name:")) {
            game_options.back().name = line;
        } else if (Data_Reader::check_prefix(line, "description:")) {
            game_options.back().description = Strings::process_newlines(line);
        } else if (Data_Reader::check_prefix(line, "default:")) {
            option_default = line;
        }
    }

    if (option_default.length() > 0) {
        game_options.back().set_value(option_default);
    }
}

void Object_Manager::load_game_constant (File_IO_Load* load) {
    string constant_name = "";
    string constant_value = "";
    vector<string> lines = Data_Reader::read_data(load, "</game_constant>");

    for (size_t i = 0; i < lines.size(); i++) {
        string& line = lines[i];

        if (Data_Reader::check_prefix(line, "name:")) {
            constant_name = line;
        } else if (Data_Reader::check_prefix(line, "value:")) {
            constant_value = line;
        }
    }

    Game_Constants_Loader::set_game_constant(constant_name, constant_value);
}

void Object_Manager::load_custom_sound (File_IO_Load* load) {
    Custom_Sound sound;
    vector<string> lines = Data_Reader::read_data(load, "</custom_sound>");

    for (size_t i = 0; i < lines.size(); i++) {
        string& line = lines[i];

        if (Data_Reader::check_prefix(line, "name:")) {
            sound.name = line;
        } else if (Data_Reader::check_prefix(line, "sample_rate:")) {
            sound.sample_rate = Strings::string_to_long(line);
        } else if (Data_Reader::check_prefix(line, "tempo:")) {
            sound.tempo = Strings::string_to_double(line);
        } else if (Data_Reader::check_prefix(line, "channels:")) {
            sound.channels = Strings::string_to_long(line);
        } else if (Data_Reader::check_prefix(line, "sharps:")) {
            sound.sharps = line;
        } else if (Data_Reader::check_prefix(line, "flats:")) {
            sound.flats = line;
        } else if (Data_Reader::check_prefix(line, "length:")) {
            sound.set_length(line);
        } else if (Data_Reader::check_prefix(line, "volume:")) {
            sound.set_volumes(line);
        } else if (Data_Reader::check_prefix(line, "waveform:")) {
            sound.set_waveforms(line);
        } else if (Data_Reader::check_prefix(line, "frequency:")) {
            sound.set_frequencies(line);
        } else if (Data_Reader::check_prefix(line, "<data>")) {
            i = load_custom_sound_data(lines, i + 1, sound);
        }
    }

    Sound_Manager::add_sound(sound);
}

size_t Object_Manager::load_custom_sound_data (vector<string>& lines, size_t line_index, Custom_Sound& sound) {
    for (size_t i = line_index; i < lines.size(); i++) {
        string& line = lines[i];

        if (Data_Reader::check_prefix(line, "length:")) {
            sound.set_length(line);
        } else if (Data_Reader::check_prefix(line, "volume:")) {
            sound.set_volumes(line);
        } else if (Data_Reader::check_prefix(line, "waveform:")) {
            sound.set_waveforms(line);
        } else if (Data_Reader::check_prefix(line, "frequency:")) {
            sound.set_frequencies(line);
        } else if (Data_Reader::check_prefix(line, "</data>")) {
            return i;
        } else {
            vector<string> components;
            boost::algorithm::split(components, line, boost::algorithm::is_any_of(","));

            if (components.size() > 0 && components[0].length() > 0) {
                string frequency = "";
                string length = "";
                string waveform = "off";
                double volume = -1.0;

                if (components.size() >= 4) {
                    frequency = components[0];
                    length = components[1];
                    waveform = components[2];
                    volume = Strings::string_to_double(components[3]);
                } else if (components.size() == 3) {
                    frequency = components[0];
                    length = components[1];
                    waveform = components[2];
                } else if (components.size() == 2) {
                    frequency = components[0];
                    length = components[1];
                } else {
                    if (isdigit(components[0][0])) {
                        frequency = components[0];
                    } else if (components[0][0] >= 'A' && components[0][0] <= 'G') {
                        frequency = components[0];
                    } else if (components[0] == "rest") {
                        frequency = components[0];
                    } else {
                        length = components[0];
                    }
                }

                // Allow spaces instead of underscores in note length names for convenience
                boost::algorithm::replace_all(length, " ", "_");

                sound.add_note(frequency, length, waveform, volume);
            }
        }
    }

    return lines.size() - 1;
}

void Object_Manager::load_cursor (File_IO_Load* load) {
    cursors.push_back(Cursor());

    vector<string> lines = Data_Reader::read_data(load, "</cursor>");

    for (size_t i = 0; i < lines.size(); i++) {
        string& line = lines[i];

        if (Data_Reader::check_prefix(line, "name:")) {
            cursors.back().name = line;
        } else if (Data_Reader::check_prefix(line, "sprite:")) {
            cursors.back().sprite.name = line;
        }
    }
}
