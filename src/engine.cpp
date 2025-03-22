/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "engine.h"
#include "data/engine_data.h"
#include "game/game_manager.h"
#include "data/data_manager.h"
#include "game_window.h"
#include "ui/gui_manager.h"
#include "sorting.h"
#include "log.h"
#include "strings/engine_strings.h"
#include "data/object_manager.h"
#include "controller/controller_manager.h"
#include "options/options.h"
#include "ui/window_manager.h"
#include "ui/window.h"
#include "render/render.h"
#include "math/engine_math.h"
#include "ui/tooltip.h"
#include "file/vfs.h"
#include "file/directories.h"
#include "android/android.h"
#include "steam/steam.h"
#include "file/file_io.h"

#include <boost/algorithm/string.hpp>
#include <boost/crc.hpp>

using namespace std;

uint32_t Engine::UPDATE_RATE = 60;
uint32_t Engine::SKIP_TICKS = 1000 / Engine::UPDATE_RATE;
uint32_t Engine::UPDATE_RATE_RENDER = 200;
uint32_t Engine::SKIP_TICKS_RENDER = 1000 / Engine::UPDATE_RATE_RENDER;
string Engine::CHECKSUM = "";
string Engine::current_mod = "";
bool Engine::mod_is_changing = false;
string Engine::new_mod = "";
string Engine::changelog = "";
bool Engine::save_data_version_is_different = false;
vector<Toast> Engine::toasts;
Console Engine::console;
Console Engine::chat;
bool Engine::mouse_over = false;
Information* Engine::ptr_mutable_info = 0;
vector<string> Engine::characters_lower;
vector<string> Engine::characters_upper;
vector<string> Engine::characters_numbers;
vector<string> Engine::characters_symbols;
Coords<int> Engine::text_entry_small_selector;
int Engine::counter_cursor = 0;
bool Engine::cursor_fade_direction = false;
int Engine::cursor_opacity = 10;
int Engine::editing_server = 0;

void Engine::build_text_input_characters () {
    characters_symbols.clear();

    characters_lower.clear();

    for (char i = 'a'; i <= 'z'; i++) {
        characters_lower.push_back(string(1, i));
    }

    if (!Engine_Data::controller_text_entry_small) {
        characters_lower.push_back(",");
        characters_lower.push_back(".");
        characters_lower.push_back(":");
        characters_lower.push_back("/");
        characters_lower.push_back("@");
        characters_lower.push_back("-");
    } else {
        characters_symbols.push_back(",");
        characters_symbols.push_back(".");
        characters_symbols.push_back(":");
        characters_symbols.push_back("/");
        characters_symbols.push_back("@");
        characters_symbols.push_back("-");
    }

    characters_upper.clear();

    for (char i = 'A'; i <= 'Z'; i++) {
        characters_upper.push_back(string(1, i));
    }

    if (!Engine_Data::controller_text_entry_small) {
        characters_upper.push_back("?");
        characters_upper.push_back("!");
        characters_upper.push_back(";");
        characters_upper.push_back("\\");
        characters_upper.push_back("&");
        characters_upper.push_back("_");
    } else {
        characters_symbols.push_back("?");
        characters_symbols.push_back("!");
        characters_symbols.push_back(";");
        characters_symbols.push_back("\\");
        characters_symbols.push_back("&");
        characters_symbols.push_back("_");
    }

    characters_numbers.clear();
    characters_numbers.push_back("1");
    characters_numbers.push_back("2");
    characters_numbers.push_back("3");
    characters_numbers.push_back("4");
    characters_numbers.push_back("5");
    characters_numbers.push_back("6");
    characters_numbers.push_back("7");
    characters_numbers.push_back("8");
    characters_numbers.push_back("9");
    characters_numbers.push_back("0");

    if (!Engine_Data::controller_text_entry_small) {
        characters_numbers.push_back("*");
        characters_numbers.push_back("+");
        characters_numbers.push_back(string(1, (unsigned char) 156));
        characters_numbers.push_back(string(1, (unsigned char) 155));
        characters_numbers.push_back("$");
        characters_numbers.push_back("`");
        characters_numbers.push_back("'");
        characters_numbers.push_back("\"");
        characters_numbers.push_back("~");
        characters_numbers.push_back("|");
        characters_numbers.push_back("=");
        characters_numbers.push_back("#");
        characters_numbers.push_back("%");
        characters_numbers.push_back("^");
        characters_numbers.push_back("<");
        characters_numbers.push_back(">");
        characters_numbers.push_back("[");
        characters_numbers.push_back("]");
        characters_numbers.push_back("{");
        characters_numbers.push_back("}");
        characters_numbers.push_back("(");
        characters_numbers.push_back(")");
    } else {
        characters_numbers.push_back(" ");
        characters_numbers.push_back(" ");
        characters_numbers.push_back(" ");

        characters_symbols.push_back("*");
        characters_symbols.push_back("+");
        characters_symbols.push_back(string(1, (unsigned char) 156));
        characters_symbols.push_back(string(1, (unsigned char) 155));
        characters_symbols.push_back("$");
        characters_symbols.push_back("`");
        characters_symbols.push_back("'");
        characters_symbols.push_back("\"");
        characters_symbols.push_back("~");
        characters_symbols.push_back("|");
        characters_symbols.push_back("=");
        characters_symbols.push_back("#");
        characters_symbols.push_back("%");
        characters_symbols.push_back("^");
        characters_symbols.push_back("<");
        characters_symbols.push_back(">");
        characters_symbols.push_back("[");
        characters_symbols.push_back("]");
        characters_symbols.push_back("{");
        characters_symbols.push_back("}");
        characters_symbols.push_back("(");
        characters_symbols.push_back(")");
        characters_symbols.push_back(" ");
        characters_symbols.push_back(" ");
        characters_symbols.push_back(" ");
        characters_symbols.push_back(string(1, (unsigned char) 1));
        characters_symbols.push_back(string(1, (unsigned char) 2));
    }
}

void Engine::unload () {
    Game_Manager::clear_title();
    Game_Manager::done_with_title();
    Game_Manager::stop();

    Data_Manager::unload_world();

    Game_Window::deinitialize();
}

void Engine::quit () {
    unload();

    #ifdef GAME_OS_ANDROID
        Android::deinitialize();
    #endif

    Steam::deinitialize();

    exit(EXIT_SUCCESS);
}

void Engine::set_mutable_info (Information* ptr_info) {
    ptr_mutable_info = ptr_info;

    if (mutable_info_selected()) {
        ptr_mutable_info->check_cursor_position();

        if (allow_screen_keyboard()) {
            SDL_StartTextInput();
        } else if (Engine_Data::controller_text_entry_small && GUI_Manager::gui_mode == "controller") {
            text_entry_small_selector.x = 0;
            text_entry_small_selector.y = 0;
        }
    }
}

void Engine::clear_mutable_info () {
    ptr_mutable_info = 0;

    if (Game_Window::is_screen_keyboard_shown()) {
        SDL_StopTextInput();
    }

    if (console.on) {
        console.toggle_on();
    }

    if (chat.on) {
        chat.toggle_on();
    }
}

bool Engine::mutable_info_selected () {
    if (ptr_mutable_info != 0) {
        return true;
    } else {
        return false;
    }
}

bool Engine::mutable_info_this (Information* ptr_info) {
    if (ptr_mutable_info == ptr_info) {
        return true;
    } else {
        return false;
    }
}

void Engine::compute_checksum () {
    vector<string> files;
    vector<string> file_list = VFS::get_file_list("data");

    for (const auto& file : file_list) {
        string file_name = file;

        files.push_back(file_name);
    }

    Sorting::quick_sort(files);

    string checksum_data = "";

    for (size_t i = 0; i < files.size(); i++) {
        File_IO_Load load(VFS::get_rwops(files[i]));

        if (load.is_opened()) {
            checksum_data += load.get_data();
        } else {
            Log::add_error("Error loading file for checksum calculation: '" + files[i] + "'");
        }
    }

    if (checksum_data.length() > 0) {
        boost::crc_32_type result;

        result.process_bytes(checksum_data.data(), checksum_data.length());

        CHECKSUM = Strings::num_to_string((uint32_t) result.checksum());
    } else {
        CHECKSUM = "";
    }
}

set<string> Engine::get_modlist () {
    set<string> mods;

    for (File_IO_Directory_Iterator it(Directories::get_save_directory() + "mods"); it.evaluate(); it.iterate()) {
        if (it.is_directory()) {
            string file_name = it.get_file_name();

            boost::algorithm::trim(file_name);

            mods.insert(file_name);
        }
    }

    return mods;
}

bool Engine::mod_exists (const string& mod) {
    if (mod.length() == 0) {
        return true;
    }

    set<string> mods = get_modlist();

    return mods.count(mod);
}

void Engine::change_mod (const string& mod) {
    if (mod == current_mod) {
        if (mod.length() > 0) {
            console.add_text("'" + mod + "' is already the active mod");
        } else {
            console.add_text("No mod is currently active");
        }
    } else {
        if (mod_exists(mod)) {
            if (mod.length() > 0) {
                console.add_text("Loading mod '" + mod + "'");
            } else {
                console.add_text("Unloading active mod '" + current_mod + "'");
            }

            mod_is_changing = true;
            new_mod = mod;
        } else {
            console.add_text("'" + mod + "' is not a valid mod");
        }
    }
}

bool Engine::mod_reload_check () {
    return mod_is_changing;
}

void Engine::swap_mods () {
    if (mod_is_changing) {
        mod_is_changing = false;

        current_mod = new_mod;

        new_mod = "";
    }
}

void Engine::set_initial_mod (const string& mod) {
    if (mod_exists(mod)) {
        current_mod = mod;
    }
}

void Engine::load_changelog () {
    changelog = "";

    File_IO_Load load("docs/changelog.md");

    if (load.is_opened()) {
        changelog = load.get_data();

        load.close();
    }
}

string Engine::get_changelog () {
    return changelog;
}

void Engine::get_rgba_masks (uint32_t* rmask, uint32_t* gmask, uint32_t* bmask, uint32_t* amask) {
    if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
        *rmask = 0xff000000;
        *gmask = 0x00ff0000;
        *bmask = 0x0000ff00;
        *amask = 0x000000ff;
    } else {
        *rmask = 0x000000ff;
        *gmask = 0x0000ff00;
        *bmask = 0x00ff0000;
        *amask = 0xff000000;
    }
}

Color_Theme* Engine::current_color_theme () {
    return Object_Manager::get_color_theme(Engine_Data::color_theme);
}

void Engine::set_logic_update_rate (uint32_t frame_rate) {
    UPDATE_RATE = frame_rate;
    SKIP_TICKS = 1000 / UPDATE_RATE;
}

void Engine::set_render_update_rate (uint32_t frame_rate) {
    UPDATE_RATE_RENDER = frame_rate;
    SKIP_TICKS_RENDER = 1000 / UPDATE_RATE_RENDER;
}

string Engine::get_text_entry_small_character () {
    if (text_entry_small_selector.x < 0) {
        text_entry_small_selector.x = 0;
    } else if (text_entry_small_selector.x > 12) {
        text_entry_small_selector.x = 12;
    }

    if (text_entry_small_selector.y < 0) {
        text_entry_small_selector.y = 0;
    } else if (text_entry_small_selector.y > 7) {
        text_entry_small_selector.y = 7;
    }

    string choices[13][8];

    for (int i = 0, n = 0; i < 13; i++, n++) {
        choices[n][0] = characters_upper[i];
    }

    for (int i = 13, n = 0; i < 26; i++, n++) {
        choices[n][1] = characters_upper[i];
    }

    for (int i = 0, n = 0; i < 13; i++, n++) {
        choices[n][2] = characters_lower[i];
    }

    for (int i = 13, n = 0; i < 26; i++, n++) {
        choices[n][3] = characters_lower[i];
    }

    for (int i = 0, n = 0; i < 13; i++, n++) {
        choices[n][4] = characters_numbers[i];
    }

    for (int i = 0, n = 0; i < 13; i++, n++) {
        choices[n][5] = characters_symbols[i];
    }

    for (int i = 13, n = 0; i < 26; i++, n++) {
        choices[n][6] = characters_symbols[i];
    }

    for (int i = 26, n = 0; i < 39; i++, n++) {
        choices[n][7] = characters_symbols[i];
    }

    return choices[text_entry_small_selector.x][text_entry_small_selector.y];
}

int Engine::get_text_input_selected_chunk () {
    int selected_chunk = -1;
    int axis_x = Controller_Manager::controller_state(-1, SDL_CONTROLLER_AXIS_LEFTX);

    if (axis_x > -Engine_Data::controller_dead_zone && axis_x < Engine_Data::controller_dead_zone) {
        axis_x = 0;
    }

    int axis_y = Controller_Manager::controller_state(-1, SDL_CONTROLLER_AXIS_LEFTY);

    if (axis_y > -Engine_Data::controller_dead_zone && axis_y < Engine_Data::controller_dead_zone) {
        axis_y = 0;
    }

    if (axis_x != 0 || axis_y != 0) {
        Collision_Rect<double> box_a(0, 0, 1, 1);
        Collision_Rect<double> box_b(axis_x, axis_y, 1, 1);
        double axis_angle = box_a.get_angle_to_rect(box_b);

        if (axis_angle >= 74 && axis_angle < 118) {
            selected_chunk = 0;
        } else if (axis_angle >= 30 && axis_angle < 74) {
            selected_chunk = 1;
        } else if (axis_angle >= 338 || axis_angle < 30) {
            selected_chunk = 2;
        } else if (axis_angle >= 294 && axis_angle < 338) {
            selected_chunk = 3;
        } else if (axis_angle >= 250 && axis_angle < 294) {
            selected_chunk = 4;
        } else if (axis_angle >= 206 && axis_angle < 250) {
            selected_chunk = 5;
        } else if (axis_angle >= 162 && axis_angle < 206) {
            selected_chunk = 6;
        } else if (axis_angle >= 118 && axis_angle < 162) {
            selected_chunk = 7;
        }
    }

    return selected_chunk;
}

vector<string>* Engine::get_text_input_character_set () {
    vector<string>* characters = &characters_lower;

    if (Controller_Manager::controller_state(-1, SDL_CONTROLLER_AXIS_TRIGGERLEFT) > Engine_Data::controller_dead_zone) {
        characters = &characters_upper;
    } else if (Controller_Manager::controller_state(-1,
                                                    SDL_CONTROLLER_AXIS_TRIGGERRIGHT) >
               Engine_Data::controller_dead_zone) {
        characters = &characters_numbers;
    }

    return characters;
}

void Engine::handle_text_input (string text) {
    if (mutable_info_selected()) {
        const uint8_t* keystates = SDL_GetKeyboardState(NULL);

        // Remove characters that are not allowed
        for (int i = 0; i < text.length(); i++) {
            bool keep_char = false;

            if ((ptr_mutable_info->allows_input("letters") &&
                 ((text[i] >= 'A' && text[i] <= 'Z') || (text[i] >= 'a' && text[i] <= 'z'))) ||
                (ptr_mutable_info->allows_input("numbers") && text[i] >= '0' && text[i] <= '9') ||
                (ptr_mutable_info->allows_input("symbols") &&
                 ((text[i] >= '!' && text[i] <= '/') || (text[i] >= ':' && text[i] <= '@') ||
                  (text[i] >= '[' && text[i] <= '`') || ((unsigned char) text[i] >= '{'))) ||
                (ptr_mutable_info->allows_input("symbols_posix") &&
                 (text[i] == '.' || text[i] == '_' || text[i] == '-')) ||
                (ptr_mutable_info->allows_input("space") && (text[i] == ' ' || text[i] == '\t'))) {
                // Only allow the console toggle key's grave character to be typed if we are not in the console
                if (!is_console_selected() ||
                    !(text[i] == '`' && (GUI_Manager::gui_mode == "mouse" || GUI_Manager::gui_mode == "keyboard") &&
                      !keystates[SDL_SCANCODE_LSHIFT] && !keystates[SDL_SCANCODE_RSHIFT])) {
                    keep_char = true;
                }
            }

            if (!keep_char) {
                text.erase(text.begin() + i);
                i--;
            }
        }

        // Convert tabs into spaces
        // I realize this is not ideal (or even correct),
        // but I think it's good enough, and implementing real tabs would be a headache
        string tab = "";

        for (int i = 0; i < Engine_Data::spaces_per_tab; i++) {
            tab += " ";
        }

        boost::algorithm::replace_all(text, "\t", tab);

        // Don't allow the mutable string to exceed its maximum size
        for (int i = text.length() - 1;
             i >= 0 && ptr_mutable_info->text.length() + text.length() > ptr_mutable_info->max_text_length; i--) {
            text.erase(text.begin() + i);
        }

        if (text.length() > 0) {
            ptr_mutable_info->add_text(text);
        }
    }
}

void Engine::get_mouse_state (int* mouse_x, int* mouse_y) {
    SDL_Rect rect;

    Game_Window::get_renderer_viewport(&rect);

    float scale_x = 0.0f;
    float scale_y = 0.0f;

    Game_Window::get_renderer_scale(&scale_x, &scale_y);

    SDL_GetMouseState(mouse_x, mouse_y);

    float offset_x = (float) rect.x * scale_x;
    float offset_y = (float) rect.y * scale_y;

    *mouse_x = (int) Math::ceil(((float) *mouse_x - offset_x) / scale_x);
    *mouse_y = (int) Math::ceil(((float) *mouse_y - offset_y) / scale_y);
}

bool Engine::mouse_allowed () {
    if (!Controller_Manager::touch_controls) {
        return true;
    } else {
        return false;
    }
}

bool Engine::allow_screen_keyboard () {
    if (Options::screen_keyboard && SDL_HasScreenKeyboardSupport()) {
        return true;
    } else {
        return false;
    }
}

void Engine::add_chat (string message) {
    chat.add_text(message);
}

bool Engine::is_console_selected () {
    if (mutable_info_selected() && mutable_info_this(&console.info_input)) {
        return true;
    } else {
        return false;
    }
}

bool Engine::is_chat_selected () {
    if (mutable_info_selected() && mutable_info_this(&chat.info_input)) {
        return true;
    } else {
        return false;
    }
}

void Engine::make_notice (string message) {
    Window* window = Window_Manager::get_window("notice");

    window->informations[0].set_text(message);

    window->set_dimensions();

    window->toggle_on(true, true);
}

void Engine::make_toast (string message, string length, int custom_length) {
    if (message.length() > 0) {
        int real_length = 0;

        if (custom_length > -1) {
            real_length = custom_length;
        } else {
            if (length == "short") {
                real_length = Engine_Data::toast_length_short;
            } else if (length == "medium") {
                real_length = Engine_Data::toast_length_medium;
            } else if (length == "long") {
                real_length = Engine_Data::toast_length_long;
            } else {
                real_length = Engine_Data::toast_length_medium;
            }
        }

        // Real length was in terms of seconds. Now we translate it to frames
        real_length = (int) Math::ceil((double) real_length * (double) UPDATE_RATE);

        if (real_length < 1) {
            real_length = 1;
        }

        toasts.push_back(Toast(message, 1.0 / (double) real_length));
    }
}

void Engine::update_window_caption (int render_rate, double ms_per_frame, int logic_frame_rate) {
    string msg = "";

    // Set the window caption
    if (Options::dev) {
        msg = Engine_Data::game_title + " (DEV Mode)";
    } else {
        msg = Engine_Data::game_title;
    }

    msg += Game_Manager::get_game_window_caption();

    if (Options::fps) {
        msg += "  FPS: " + Strings::num_to_string(render_rate);

        msg += "  LUPS: " + Strings::num_to_string(logic_frame_rate);

        if (ms_per_frame >= 1.0) {
            msg += "  MS/Frame: " + Strings::num_to_string(ms_per_frame);
        } else {
            msg += "  MS/Frame: <1";
        }
    }

    Game_Window::set_title(msg);
}

string Engine::get_system_info () {
    string msg = "";
    int logical_width = 0;
    int logical_height = 0;

    Game_Window::get_renderer_logical_size(&logical_width, &logical_height);

    SDL_Rect rect;

    Game_Window::get_renderer_viewport(&rect);

    float scale_x = 0.0;
    float scale_y = 0.0;

    Game_Window::get_renderer_scale(&scale_x, &scale_y);

    int actual_width = 0;
    int actual_height = 0;

    Game_Window::get_renderer_output_size(&actual_width, &actual_height);

    SDL_RendererInfo info;

    Game_Window::get_renderer_info(&info);

    string renderer_name = info.name;
    int mouse_x = 0;
    int mouse_y = 0;

    Engine::get_mouse_state(&mouse_x, &mouse_y);

    int mouse_real_x = 0;
    int mouse_real_y = 0;

    SDL_GetMouseState(&mouse_real_x, &mouse_real_y);

    int power_seconds = 0;
    int power_percentage = 0;
    SDL_PowerState power_state = SDL_GetPowerInfo(&power_seconds, &power_percentage);
    string str_power_seconds = Strings::time_string(power_seconds) + " remaining";
    string str_power_percentage = Strings::num_to_string(power_percentage) + "%";

    Controller_Manager::get_controller_info(msg);

    msg += "Resolution (Logical): " + Strings::num_to_string(logical_width) + " x " +
           Strings::num_to_string(logical_height) + "\n";
    msg += "Logical Viewport: " + Strings::num_to_string(rect.x) + "," + Strings::num_to_string(rect.y) + "," +
           Strings::num_to_string(rect.w) + "," + Strings::num_to_string(rect.h) + "\n";
    msg += "Render Scale: " + Strings::num_to_string(scale_x) + "," + Strings::num_to_string(scale_y) + "\n";
    msg += "Resolution (Actual): " + Strings::num_to_string(actual_width) + " x " +
           Strings::num_to_string(actual_height) + "\n";
    msg += "Renderer: " + renderer_name + "\n";
    msg += "Max Texture Size: " + Strings::num_to_string(info.max_texture_width) + " x " +
           Strings::num_to_string(info.max_texture_height) + "\n";
    msg += "Current Gui Mode: " + Strings::first_letter_capital(GUI_Manager::gui_mode) + "\n";

    msg += "Mouse Position (Logical): " + Strings::num_to_string(mouse_x) + "," + Strings::num_to_string(mouse_y) +
           "\n";
    msg += "Mouse Position (Actual): " + Strings::num_to_string(mouse_real_x) + "," +
           Strings::num_to_string(mouse_real_y) + "\n";

    msg += "Power State: ";

    if (power_state == SDL_POWERSTATE_UNKNOWN) {
        msg += "Unknown";
    } else if (power_state == SDL_POWERSTATE_ON_BATTERY) {
        msg += "Unplugged, on battery";
    } else if (power_state == SDL_POWERSTATE_NO_BATTERY) {
        msg += "Plugged in, no battery";
    } else if (power_state == SDL_POWERSTATE_CHARGING) {
        msg += "Plugged in, charging";
    } else if (power_state == SDL_POWERSTATE_CHARGED) {
        msg += "Plugged in, fully charged";
    }

    if (power_seconds != -1 || power_percentage != -1) {
        msg += " (";
    }

    if (power_percentage != -1) {
        msg += str_power_percentage;
    }

    if (power_seconds != -1) {
        if (power_percentage != -1) {
            msg += ", ";
        }

        msg += str_power_seconds;
    }

    if (power_seconds != -1 || power_percentage != -1) {
        msg += ")";
    }

    return msg;
}

void Engine::animate () {
    Window_Manager::animate();

    console.animate();
    chat.animate();

    if (toasts.size() > 0) {
        toasts[0].animate();

        if (toasts[0].is_done()) {
            toasts.erase(toasts.begin());
        }
    }

    // Update the text cursor
    if (++counter_cursor >= (int) Math::ceil((48.0 / 1000.0) * (double) UPDATE_RATE)) {
        counter_cursor = 0;

        if (cursor_opacity == 10) {
            cursor_fade_direction = false;
        } else if (cursor_opacity == 0) {
            cursor_fade_direction = true;
        }

        if (cursor_fade_direction && cursor_opacity < 10) {
            cursor_opacity += 1;
        } else if (!cursor_fade_direction && cursor_opacity > 0) {
            cursor_opacity -= 1;
        }
    }

    Object_Manager::animate_cursors();

    GUI_Manager::animate();
}

Cursor* Engine::get_current_cursor () {
    if (!mouse_over) {
        return Object_Manager::get_cursor(Engine_Data::cursor);
    } else {
        return Object_Manager::get_cursor(Engine_Data::cursor_mouse_over);
    }
}

void Engine::render_toast () {
    if (toasts.size() > 0) {
        toasts[0].render();
    }
}

void Engine::render_small_text_inputter () {
    Render::render_rectangle(0, 0, Game_Window::width(), Game_Window::height(), 0.75,
                             current_color_theme()->window_border);
    Render::render_rectangle(Engine_Data::window_border_thickness, Engine_Data::window_border_thickness,
                             Game_Window::width() - Engine_Data::window_border_thickness * 2.0,
                             Game_Window::height() - Engine_Data::window_border_thickness * 2.0, 0.75,
                             current_color_theme()->window_background);

    if (mutable_info_selected()) {
        Bitmap_Font* font = Object_Manager::get_font("small");

        font->show(Engine_Data::window_border_thickness + 2.0, Engine_Data::window_border_thickness + 2.0,
                   ptr_mutable_info->get_cursor_line(), current_color_theme()->window_font);
    }

    Bitmap_Font* font = Object_Manager::get_font("standard");
    double buttons_start_y = Engine_Data::window_border_thickness + 2.0 + font->spacing_y * 2.0;
    double x_offset = ((double) Game_Window::width() - font->get_letter_width() * 13.0) / 2.0 +
                      Engine_Data::window_border_thickness;
    double offset_y = buttons_start_y;

    for (int i = 0; i < 13; i++) {
        font->show(x_offset + i * font->get_letter_width(), offset_y, characters_upper[i],
                   current_color_theme()->window_font);
    }

    offset_y += font->get_letter_height();

    for (int i = 13; i < characters_upper.size(); i++) {
        font->show(x_offset + (i - 13) * font->get_letter_width(), offset_y, characters_upper[i],
                   current_color_theme()->window_font);
    }

    offset_y += font->get_letter_height();

    for (int i = 0; i < 13; i++) {
        font->show(x_offset + i * font->get_letter_width(), offset_y, characters_lower[i],
                   current_color_theme()->window_font);
    }

    offset_y += font->get_letter_height();

    for (int i = 13; i < characters_lower.size(); i++) {
        font->show(x_offset + (i - 13) * font->get_letter_width(), offset_y, characters_lower[i],
                   current_color_theme()->window_font);
    }

    offset_y += font->get_letter_height();

    for (int i = 0; i < characters_numbers.size(); i++) {
        font->show(x_offset + i * font->get_letter_width(), offset_y, characters_numbers[i],
                   current_color_theme()->window_font);
    }

    offset_y += font->get_letter_height();

    for (int i = 0; i < 13; i++) {
        font->show(x_offset + i * font->get_letter_width(), offset_y, characters_symbols[i],
                   current_color_theme()->window_font);
    }

    offset_y += font->get_letter_height();

    for (int i = 13; i < 26; i++) {
        font->show(x_offset + (i - 13) * font->get_letter_width(), offset_y, characters_symbols[i],
                   current_color_theme()->window_font);
    }

    offset_y += font->get_letter_height();

    for (int i = 26; i < characters_symbols.size(); i++) {
        font->show(x_offset + (i - 26) * font->get_letter_width(), offset_y, characters_symbols[i],
                   current_color_theme()->window_font);
    }

    GUI_Manager::text_selector.render(x_offset + text_entry_small_selector.x * font->get_letter_width() -
                                      (GUI_Manager::text_selector.get_width() - font->get_letter_width()) / 2.0,
                                      buttons_start_y + text_entry_small_selector.y * font->get_letter_height() -
                                      (GUI_Manager::text_selector.get_height() - font->get_letter_height()) / 2.0);
}

void Engine::render_text_inputter () {
    vector<string>* characters = get_text_input_character_set();
    int selected_chunk = get_text_input_selected_chunk();
    double outer_radius = Game_Window::width() / 5;
    double outer_center_y = outer_radius + 10;

    // The topmost instructions' y position.
    int words_y = outer_center_y + outer_radius + 10;
    int x_adjust = 0;

    if (mutable_info_selected()) {
        Engine_Rect object_pos;

        if (is_console_selected()) {
            object_pos.x = console.info_input.x;
            object_pos.y = console.info_input.y;
            object_pos.w = console.info_input.w;
            object_pos.h = console.info_input.h;
        } else if (is_chat_selected()) {
            object_pos.x = chat.info_input.x;
            object_pos.y = chat.info_input.y;
            object_pos.w = chat.info_input.w;
            object_pos.h = chat.info_input.h;
        } else {
            object_pos = GUI_Manager::get_gui_selected_object_pos();
        }

        if (object_pos.x != -1) {
            if (object_pos.x <= Game_Window::width() / 2) {
                x_adjust = Game_Window::width() / 2;
            }

            if (object_pos.y <= Game_Window::height() / 2) {
                outer_center_y = Game_Window::height() - outer_radius - 10;
                words_y = outer_center_y - outer_radius - 120;
            }
        }
    }

    double outer_center_x = x_adjust + Game_Window::width() / 4;
    Bitmap_Font* font = Object_Manager::get_font("standard");

    Render::render_rectangle(x_adjust, 0, Game_Window::width() / 2, Game_Window::height(), 0.75,
                             current_color_theme()->window_background);
    Render::render_rectangle_empty(x_adjust, 0, Game_Window::width() / 2, Game_Window::height(), 0.75,
                                   current_color_theme()->window_border, 2.0);
    Render::render_circle(outer_center_x + 4, outer_center_y + 4, outer_radius, 1.0, "ui_black");
    Render::render_circle(outer_center_x, outer_center_y, outer_radius, 1.0, current_color_theme()->window_border);

    Render::render_circle(outer_center_x, outer_center_y, 16.0, 1.0, "ui_gray");
    font->show(outer_center_x - 12.0, outer_center_y - 8.0, "LS", "ui_white");

    string text = "";

    text = "(Press for Return)";
    font->show(x_adjust + (Game_Window::width() / 2 - text.length() * font->spacing_x) / 2,
               outer_center_y - 8.0 + font->spacing_y, text, "ui_white");

    text = "[LB] Backspace";
    font->show(x_adjust + 100, words_y, text, current_color_theme()->window_font);
    text = "Space [RB]";
    font->show(x_adjust + Game_Window::width() / 2 - text.length() * font->spacing_x - 100, words_y, text,
               current_color_theme()->window_font);

    text = "(LT) Caps";
    font->show(x_adjust + 100, words_y + 30, text, current_color_theme()->window_font);
    text = "Numbers (RT)";
    font->show(x_adjust + Game_Window::width() / 2 - text.length() * font->spacing_x - 100, words_y + 30, text,
               current_color_theme()->window_font);

    if (selected_chunk == -1) {
        if (is_console_selected()) {
            text = "(A) Auto-complete";
            font->show(x_adjust + (Game_Window::width() / 2 - text.length() * font->spacing_x) / 2, words_y + 60, text,
                       current_color_theme()->window_font);
        }

        text = "(B) Back/Done";
        font->show(x_adjust + (Game_Window::width() / 2 - text.length() * font->spacing_x) / 2, words_y + 90, text,
                   current_color_theme()->window_font);
    }

    int character_chunk = 0;
    int character = 0;

    for (double angle = -90.0; angle < 270.0; angle += 45.0) {
        double mid_radius = outer_radius * 0.75;
        double inner_radius = 50.0;
        double inner_center_x = outer_center_x + mid_radius * Math::cos(Math::degrees_to_radians(angle));
        double inner_center_y = outer_center_y + mid_radius * Math::sin(Math::degrees_to_radians(angle));
        string outer_circle_color = current_color_theme()->window_background;

        if (character_chunk == selected_chunk) {
            outer_circle_color = current_color_theme()->window_title_bar;
        }

        Render::render_circle(inner_center_x, inner_center_y, inner_radius, 1.0, outer_circle_color);

        for (double letter_angle = -180.0; letter_angle < 180.0; letter_angle += 90.0) {
            double letter_circle_radius = inner_radius * 0.75;
            double letter_radius = 16.0;
            string circle_color = "text_input_blue";

            if (letter_angle == -180.0) {
                circle_color = "text_input_blue";
            } else if (letter_angle == -90.0) {
                circle_color = "text_input_yellow";
            } else if (letter_angle == 0.0) {
                circle_color = "text_input_red";
            } else if (letter_angle == 90.0) {
                circle_color = "text_input_green";
            }

            double letter_center_x = inner_center_x + letter_circle_radius *
                                     Math::cos(Math::degrees_to_radians(letter_angle));
            double letter_center_y = inner_center_y + letter_circle_radius *
                                     Math::sin(Math::degrees_to_radians(letter_angle));
            string font_color = current_color_theme()->window_font;

            if (character_chunk == selected_chunk) {
                font_color = "ui_white";

                Render::render_circle(letter_center_x, letter_center_y, letter_radius, 1.0, circle_color);
            }

            if (character < characters->size()) {
                font->show(letter_center_x - 8.0, letter_center_y - 8.0, characters->at(character), font_color);
            }

            character++;
        }

        character_chunk++;
    }
}

void Engine::render_text_editing () {
    if (mutable_info_selected()) {
        Bitmap_Font* font = Object_Manager::get_font("small");
        string text = ptr_mutable_info->get_cursor_line();

        Render::render_rectangle(0.0, 0.0, Game_Window::width(),
                                 font->spacing_y + Engine_Data::window_border_thickness * 2.0, 0.75,
                                 current_color_theme()->window_border);
        Render::render_rectangle(Engine_Data::window_border_thickness, Engine_Data::window_border_thickness,
                                 Game_Window::width() - Engine_Data::window_border_thickness * 2.0, font->spacing_y,
                                 0.75, current_color_theme()->window_background);

        font->show((Game_Window::width() - (text.length() * font->spacing_x)) / 2.0,
                   Engine_Data::window_border_thickness, text, current_color_theme()->window_font);
    }
}

void Engine::render (int render_rate, double ms_per_frame, int logic_frame_rate) {
    if (!GUI_Manager::hide_gui) {
        if (Game_Manager::in_progress && Game_Manager::paused) {
            Game_Manager::render_pause();
        }

        Window_Manager::render();

        GUI_Manager::render_gui_selector();

        console.render();
        chat.render();

        Tooltip::render();

        render_toast();

        if (mutable_info_selected() && GUI_Manager::gui_mode == "controller" && !allow_screen_keyboard()) {
            if (!Engine_Data::controller_text_entry_small) {
                render_text_inputter();
            } else {
                render_small_text_inputter();
            }
        }

        if (mutable_info_selected() && allow_screen_keyboard() && Game_Window::is_screen_keyboard_shown()) {
            render_text_editing();
        }

        Controller_Manager::render_touch_controller();

        if (Options::dev) {
            render_dev_info();
        }

        if (Options::fps) {
            Game_Manager::render_fps(render_rate, ms_per_frame, logic_frame_rate);
        }

        if (GUI_Manager::gui_mode == "mouse" &&
            (Engine_Data::cursor_render_always || Window_Manager::is_any_window_open() || console.on)) {
            int mouse_x = 0;
            int mouse_y = 0;

            get_mouse_state(&mouse_x, &mouse_y);

            get_current_cursor()->render(mouse_x, mouse_y);
        } else {
            if (Options::hw_cursor) {
                SDL_ShowCursor(SDL_DISABLE);
            }
        }
    } else {
        if (Options::hw_cursor) {
            SDL_ShowCursor(SDL_DISABLE);
        }
    }
}
