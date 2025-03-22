/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "data_manager.h"
#include "../ui/gui_manager.h"
#include "../options/options.h"
#include "../game_window.h"
#include "../image/image_manager.h"
#include "../sound/sound_manager.h"
#include "../music/music_manager.h"
#include "../engine.h"
#include "../render/rtt_manager.h"
#include "object_manager.h"
#include "../ui/window_manager.h"
#include "../game/game_manager.h"
#include "data_reader.h"
#include "engine_data.h"
#include "../log.h"
#include "../strings/engine_strings.h"
#include "../file/vfs.h"

using namespace std;

bool Data_Manager::images_loaded = false;
bool Data_Manager::fonts_loaded = false;
bool Data_Manager::colors_loaded = false;
bool Data_Manager::world_loaded = false;
const int Data_Manager::world_load_item_count = 20;

bool Data_Manager::are_images_loaded () {
    return images_loaded;
}

bool Data_Manager::are_fonts_loaded () {
    return fonts_loaded;
}

bool Data_Manager::are_colors_loaded () {
    return colors_loaded;
}

bool Data_Manager::is_world_loaded () {
    return world_loaded;
}

bool Data_Manager::load_world (Progress_Bar& bar) {
    GUI_Manager::initialize();

    load_data_game_options();

    if (!Options::load_options()) {
        return false;
    }

    if (!Game_Window::initialize()) {
        return false;
    }

    load_data_colors(bar);

    bar.progress("Loading images");

    Image_Manager::load_images();

    images_loaded = true;

    load_data_fonts(bar);

    bar.progress("Loading sounds");

    Sound_Manager::load_sounds();

    bar.progress("Loading music");

    Music_Manager::prepare_tracks();

    load_data_main(bar);

    bar.progress("Preparing render target textures");

    add_rtts();

    bar.progress("Preparing console");

    Engine::console.setup(false);

    bar.progress("Preparing chat box");

    Engine::chat.setup(true);

    bar.progress("Loading game commands cfg");

    if (!Options::load_game_commands()) {
        return false;
    }

    bar.progress("Loading servers list");

    Options::load_servers();

    bar.progress("Loading error image");

    // To be safe, this should be at the very bottom of load_world()
    Image_Manager::set_error_image();

    bar.progress("Done loading data");
    Log::add_log("Data loaded in " + Strings::num_to_string(bar.get_time_elapsed()) + " ms");

    world_loaded = true;

    Log::post_world_load_errors();
    Log::post_world_load_logs();

    return true;
}

void Data_Manager::unload_world () {
    if (world_loaded) {
        world_loaded = false;
        colors_loaded = false;
        fonts_loaded = false;
        images_loaded = false;

        Image_Manager::unload_images();

        Rtt_Manager::unload_textures();

        Sound_Manager::unload_sounds();

        Music_Manager::unload_tracks();

        unload_data();

        GUI_Manager::deinitialize();
    }
}

void Data_Manager::unload_data () {
    Engine::clear_mutable_info();

    Object_Manager::unload_data();

    Window_Manager::unload_data();

    Game_Manager::unload_data_game();
}

void Data_Manager::load_data_colors (Progress_Bar& bar) {
    bar.progress("Loading colors");

    load_data("color");

    colors_loaded = true;
}

void Data_Manager::load_data_fonts (Progress_Bar& bar) {
    bar.progress("Loading animations");

    load_data("animation");

    bar.progress("Loading fonts");

    load_data("font");

    fonts_loaded = true;
}

void Data_Manager::load_data_main (Progress_Bar& bar) {
    bar.progress("Loading cursors");

    load_data("cursor");

    bar.progress("Loading color themes");

    load_data("color_theme");

    bar.progress("Loading windows");

    load_data("window");

    bar.progress("Loading game commands");

    load_data("game_command");

    bar.progress("Loading game constants");

    load_data("game_constant");

    bar.progress("Loading custom sounds");

    load_data("custom_sound");

    Game_Manager::load_data_game(bar);

    bar.progress("Completing main data loading process");

    Window_Manager::create_window_close_buttons();

    Object_Manager::load_hw_cursors();

    GUI_Manager::text_selector.set_name("text_selector");
}

bool Data_Manager::load_data_engine () {
    bool load_result = load_data("engine");

    Engine::build_text_input_characters();

    return load_result;
}

void Data_Manager::load_data_game_options () {
    load_data("game_option");
}

bool Data_Manager::load_data (string tag) {
    vector<string> file_list = VFS::get_file_list("data");

    for (const auto& file : file_list) {
        File_IO_Load load(VFS::get_rwops(file));

        if (load.is_opened()) {
            while (!load.eof()) {
                vector<string> lines = Data_Reader::read_data(&load, "<" + tag + ">");

                if (lines.size() > 0 && Data_Reader::check_prefix(lines.back(), "<" + tag + ">")) {
                    if (tag == "engine") {
                        Engine_Data::load_engine_data(&load);
                    } else if (tag == "font") {
                        Object_Manager::load_font(&load);
                    } else if (tag == "cursor") {
                        Object_Manager::load_cursor(&load);
                    } else if (tag == "color") {
                        Object_Manager::load_color(&load);
                    } else if (tag == "color_theme") {
                        Object_Manager::load_color_theme(&load);
                    } else if (tag == "animation") {
                        Object_Manager::load_animation(&load);
                    } else if (tag == "window") {
                        Window_Manager::load_window(&load);
                    } else if (tag == "game_command") {
                        Object_Manager::load_game_command(&load);
                    } else if (tag == "game_option") {
                        Object_Manager::load_game_option(&load);
                    } else if (tag == "game_constant") {
                        Object_Manager::load_game_constant(&load);
                    } else if (tag == "custom_sound") {
                        Object_Manager::load_custom_sound(&load);
                    } else {
                        Game_Manager::load_data_tag_game(tag, &load);
                    }
                }
            }
        } else {
            Log::add_error("Error loading tag data: '" + tag + "'");

            return false;
        }
    }

    return true;
}
