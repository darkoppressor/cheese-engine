/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "button_events.h"
#include "window_manager.h"
#include "engine.h"
#include "object_manager.h"
#include "data_manager.h"
#include "options.h"
#include "directories.h"
#include "game_window.h"
#include "game_manager.h"
#include "engine_strings.h"
#include "network_server.h"
#include "network_client.h"
#include "network_lan_browser.h"
#include "engine_input.h"
#include "log.h"
#include "engine_math.h"

#include <vector>
#include <cstdint>

#include <boost/algorithm/string.hpp>

using namespace std;

string Button_Events::DEFAULT_AUDIO_DEVICE = "System default";
bool Button_Events::handle_button_event (string button_event, Window* parent_window) {
    bool window_opened_on_top = false;

    if (button_event.length() > 0) {
        if (button_event == "close_window") {
            if (parent_window != 0) {
                parent_window->toggle_on(true, false);
            }
        } else if (boost::algorithm::starts_with(button_event, "open_window_")) {
            boost::algorithm::erase_first(button_event, "open_window_");

            Window_Manager::get_window(button_event)->toggle_on(true, true);
            window_opened_on_top = true;
        } else if (button_event == "quit") {
            Engine::quit();
        } else if (button_event == "configure_commands") {
            Window_Manager::get_window("configure_commands")->toggle_on();
            window_opened_on_top = true;
        } else if (button_event == "reset_default_commands") {
            Object_Manager::clear_game_commands();
            Data_Manager::load_data("game_command");
            Options::save_game_commands();
        } else if (button_event == "show_data_location") {
            string save_path = Directories::get_save_directory_absolute();
            vector<string> lines;
            uint32_t spacing_x = Object_Manager::get_font("small")->spacing_x;
            uint32_t window_width = (uint32_t) Math::floor((double) Game_Window::width() * 0.8);

            while (save_path.length() * spacing_x > window_width) {
                uint32_t i = window_width / spacing_x;
                lines.push_back(string(save_path, 0, i));
                save_path.erase(save_path.begin(), save_path.begin() + i);
            }

            if (save_path.length() > 0) {
                lines.push_back(save_path);
            }

            save_path = "";

            for (size_t i = 0; i < lines.size(); i++) {
                save_path += lines[i];

                if (i < lines.size() - 1) {
                    save_path += "\n";
                }
            }

            Engine::make_notice("Your save data is located here:\n\n" + save_path);
            window_opened_on_top = true;
        } else if (button_event == "start_game_window") {
            Window_Manager::get_window("start_game")->toggle_on();
            window_opened_on_top = true;
        } else if (button_event == "start_game") {
            Window_Manager::close_all_windows();

            Game_Manager::stop();

            Game_Manager::start();
        } else if (button_event == "stop_game") {
            Window_Manager::close_all_windows();

            Game_Manager::stop();

            Window_Manager::get_window("main_menu")->toggle_on();
        } else if (button_event == "options_window") {
            Window* window = Window_Manager::get_window("options");

            window->set_info_text(0, Options::get_option_value("cl_effect_limit"));
            window->set_info_tooltip(0, Options::get_option_description("cl_effect_limit"));

            window->set_stated_button_state_index(0,
                                                  Strings::string_to_bool(Options::get_option_value("cl_screen_shake")));
            window->set_stated_button_tooltip(0, Options::get_option_description("cl_screen_shake"));

            window->set_stated_button_state_index(1, Strings::string_to_bool(Options::get_option_value("cl_rumble")));
            window->set_stated_button_tooltip(1, Options::get_option_description("cl_rumble"));

            window->toggle_on();
            window_opened_on_top = true;
        } else if (button_event == "options" || button_event == "options_apply") {
            if (parent_window != 0) {
                Options::apply_options(parent_window->get_info_text(0),
                                       Strings::bool_to_string(Strings::string_to_bool(parent_window->
                                                                                       get_stated_button_state(
                                                                                           0))),
                                       Strings::bool_to_string(Strings::string_to_bool(parent_window->
                                                                                       get_stated_button_state(
                                                                                           1))));

                if (button_event == "options") {
                    handle_button_event("close_window", parent_window);
                }
            }
        } else if (button_event == "options_graphics_window") {
            Window* window = Window_Manager::get_window("options_graphics");

            window->set_info_text(0, Options::get_option_value("cl_screen_width"));
            window->set_info_tooltip(0, Options::get_option_description("cl_screen_width"));

            window->set_info_text(1, Options::get_option_value("cl_screen_height"));
            window->set_info_tooltip(1, Options::get_option_description("cl_screen_height"));

            window->set_info_text(2, Options::get_option_value("cl_display_number"));
            window->set_info_tooltip(2, Options::get_option_description("cl_display_number"));

            window->set_stated_button_state_index(0,
                                                  Strings::string_to_bool(Options::get_option_value(
                                                                              "cl_fullscreen_state")));
            window->set_stated_button_tooltip(0, Options::get_option_description("cl_fullscreen_state"));

            window->set_stated_button_state_index(1, Strings::string_to_bool(Options::get_option_value("cl_vsync")));
            window->set_stated_button_tooltip(1, Options::get_option_description("cl_vsync"));

            uint32_t fs_mode = 0;

            if (Options::get_option_value("cl_fullscreen_mode") == "desktop") {
                fs_mode = 1;
            } else if (Options::get_option_value("cl_fullscreen_mode") == "windowed") {
                fs_mode = 2;
            }

            window->set_stated_button_state_index(2, fs_mode);
            window->set_stated_button_tooltip(2, Options::get_option_description("cl_fullscreen_mode"));

            window->set_stated_button_state_index(3, Strings::string_to_bool(Options::get_option_value("cl_fps")));
            window->set_stated_button_tooltip(3, Options::get_option_description("cl_fps"));

            window->set_stated_button_state_index(4,
                                                  Strings::string_to_bool(Options::get_option_value("cl_hw_cursor")));
            window->set_stated_button_tooltip(4, Options::get_option_description("cl_hw_cursor"));

            window->set_stated_button_state_index(5,
                                                  Strings::string_to_bool(Options::get_option_value("cl_font_shadows")));
            window->set_stated_button_tooltip(5, Options::get_option_description("cl_font_shadows"));

            window->toggle_on();
            window_opened_on_top = true;
        } else if (button_event == "options_graphics" || button_event == "options_graphics_apply") {
            if (parent_window != 0) {
                Options::apply_options_graphics(parent_window->get_info_text(0), parent_window->get_info_text(
                                                    1), parent_window->get_info_text(2),
                                                Strings::bool_to_string(Strings::string_to_bool(parent_window->
                                                                                                get_stated_button_state(
                                                                                                    0))),
                                                Strings::lower_case(parent_window->get_stated_button_state(
                                                                        2)),
                                                Strings::bool_to_string(Strings::string_to_bool(parent_window->
                                                                                                get_stated_button_state(
                                                                                                    1))),
                                                Strings::bool_to_string(Strings::string_to_bool(parent_window->
                                                                                                get_stated_button_state(
                                                                                                    3))),
                                                Strings::bool_to_string(Strings::string_to_bool(parent_window->
                                                                                                get_stated_button_state(
                                                                                                    4))),
                                                Strings::bool_to_string(Strings::string_to_bool(parent_window->
                                                                                                get_stated_button_state(
                                                                                                    5))));

                if (button_event == "options_graphics") {
                    handle_button_event("close_window", parent_window);
                }
            }
        } else if (button_event == "options_audio_window") {
            Window* window = Window_Manager::get_window("options_audio");
            double volume_global = Strings::string_to_double(Options::get_option_value("cl_volume_global"));
            uint32_t n_volume_global = 2;

            if (volume_global <= 0.25) {
                n_volume_global = 0;
            } else if (volume_global <= 0.5) {
                n_volume_global = 1;
            }

            window->set_stated_button_state_index(0, n_volume_global);
            window->set_stated_button_tooltip(0, Options::get_option_description("cl_volume_global"));

            window->set_stated_button_state_index(1, Strings::string_to_bool(Options::get_option_value(
                                                                                 "cl_mute_global")));
            window->set_stated_button_tooltip(1, Options::get_option_description("cl_mute_global"));

            double volume_sound = Strings::string_to_double(Options::get_option_value("cl_volume_sound"));
            uint32_t n_volume_sound = 2;

            if (volume_sound <= 0.25) {
                n_volume_sound = 0;
            } else if (volume_sound <= 0.5) {
                n_volume_sound = 1;
            }

            window->set_stated_button_state_index(2, n_volume_sound);
            window->set_stated_button_tooltip(2, Options::get_option_description("cl_volume_sound"));

            window->set_stated_button_state_index(3,
                                                  Strings::string_to_bool(Options::get_option_value("cl_mute_sound")));
            window->set_stated_button_tooltip(3, Options::get_option_description("cl_mute_sound"));

            double volume_music = Strings::string_to_double(Options::get_option_value("cl_volume_music"));
            uint32_t n_volume_music = 2;

            if (volume_music <= 0.25) {
                n_volume_music = 0;
            } else if (volume_music <= 0.5) {
                n_volume_music = 1;
            }

            window->set_stated_button_state_index(4, n_volume_music);
            window->set_stated_button_tooltip(4, Options::get_option_description("cl_volume_music"));

            window->set_stated_button_state_index(5,
                                                  Strings::string_to_bool(Options::get_option_value("cl_mute_music")));
            window->set_stated_button_tooltip(5, Options::get_option_description("cl_mute_music"));

            vector<string> audio_devices = Game_Window::get_audio_devices();
            audio_devices.insert(audio_devices.begin(), DEFAULT_AUDIO_DEVICE);
            string current_device = Options::get_option_value("cl_audio_playback_device");
            uint32_t state_index = 0;

            if (current_device.length() > 0 && Game_Window::is_audio_playback_device_present(current_device)) {
                for (size_t i = 0; i < audio_devices.size(); i++) {
                    if (current_device == audio_devices[i]) {
                        state_index = i;

                        break;
                    }
                }
            }

            window->set_stated_button_states(6, audio_devices);
            window->set_stated_button_state_index(6, state_index);
            window->set_stated_button_tooltip(6, Options::get_option_description("cl_audio_playback_device"));

            window->toggle_on();
            window_opened_on_top = true;
        } else if (button_event == "options_audio" || button_event == "options_audio_apply") {
            if (parent_window != 0) {
                string mute_global = "false";

                if (parent_window->get_stated_button_state(1) == "Muted") {
                    mute_global = "true";
                }

                string mute_sound = "false";

                if (parent_window->get_stated_button_state(3) == "Muted") {
                    mute_sound = "true";
                }

                string mute_music = "false";

                if (parent_window->get_stated_button_state(5) == "Muted") {
                    mute_music = "true";
                }

                string audio_device = parent_window->get_stated_button_state(6);

                if (audio_device == DEFAULT_AUDIO_DEVICE) {
                    audio_device = "";
                }

                Options::apply_options_audio(parent_window->get_stated_button_state(
                                                 0), mute_global, parent_window->get_stated_button_state(
                                                 2), mute_sound, parent_window->get_stated_button_state(
                                                 4), mute_music, audio_device);

                if (button_event == "options_audio") {
                    handle_button_event("close_window", parent_window);
                }
            }
        } else if (button_event == "options_input_window") {
            Window* window = Window_Manager::get_window("options_input");

            window->set_stated_button_state_index(0, Strings::string_to_bool(Options::get_option_value(
                                                                                 "cl_bind_cursor")));
            window->set_stated_button_tooltip(0, Options::get_option_description("cl_bind_cursor"));

            window->set_stated_button_state_index(1,
                                                  Strings::string_to_bool(Options::get_option_value(
                                                                              "cl_screen_keyboard")));
            window->set_stated_button_tooltip(1, Options::get_option_description("cl_screen_keyboard"));

            window->set_stated_button_state_index(2,
                                                  Strings::string_to_bool(Options::get_option_value(
                                                                              "cl_accelerometer_controller")));
            window->set_stated_button_tooltip(2, Options::get_option_description("cl_accelerometer_controller"));

            window->set_stated_button_state_index(3,
                                                  Strings::string_to_bool(Options::get_option_value(
                                                                              "cl_touch_controller_state")));
            window->set_stated_button_tooltip(3, Options::get_option_description("cl_touch_controller_state"));

            double tc_opacity = Strings::string_to_double(Options::get_option_value("cl_touch_controller_opacity"));
            uint32_t n_tc_opacity = 2;

            if (tc_opacity <= 0.25) {
                n_tc_opacity = 0;
            } else if (tc_opacity <= 0.5) {
                n_tc_opacity = 1;
            }

            window->set_stated_button_state_index(4, n_tc_opacity);
            window->set_stated_button_tooltip(4, Options::get_option_description("cl_touch_controller_opacity"));

            window->toggle_on();
            window_opened_on_top = true;
        } else if (button_event == "options_input" || button_event == "options_input_apply") {
            if (parent_window != 0) {
                Options::apply_options_input(Strings::bool_to_string(Strings::string_to_bool(parent_window->
                                                                                             get_stated_button_state(
                                                                                                 0))),
                                             Strings::bool_to_string(Strings::string_to_bool(parent_window->
                                                                                             get_stated_button_state(
                                                                                                 1))),
                                             Strings::bool_to_string(Strings::string_to_bool(parent_window->
                                                                                             get_stated_button_state(
                                                                                                 2))),
                                             Strings::bool_to_string(Strings::string_to_bool(parent_window->
                                                                                             get_stated_button_state(
                                                                                                 3))), parent_window->get_stated_button_state(
                                                 4));

                if (button_event == "options_input") {
                    handle_button_event("close_window", parent_window);
                }
            }
        } else if (button_event == "options_network_window") {
            Window* window = Window_Manager::get_window("options_network");

            window->set_info_text(0, Options::get_option_value("cl_name"));
            window->set_info_tooltip(0, Options::get_option_description("cl_name"));

            window->set_stated_button_state_index(0,
                                                  Strings::string_to_bool(Options::get_option_value(
                                                                              "cl_chat_timestamps")));
            window->set_stated_button_tooltip(0, Options::get_option_description("cl_chat_timestamps"));

            window->toggle_on();
            window_opened_on_top = true;
        } else if (button_event == "options_network" || button_event == "options_network_apply") {
            if (parent_window != 0) {
                Options::apply_options_network(parent_window->get_info_text(0),
                                               Strings::bool_to_string(Strings::string_to_bool(parent_window->
                                                                                               get_stated_button_state(
                                                                                                   0))));

                if (button_event == "options_network") {
                    handle_button_event("close_window", parent_window);
                }
            }
        } else if (button_event == "start_server") {
            Window_Manager::close_all_windows();

            Window* window = Window_Manager::get_window("start_server");

            Options::change_option("sv_network_name", window->get_info_text(0));
            Options::change_option("sv_network_password", window->get_info_text(1));

            Game_Manager::stop();

            Game_Manager::start();

            if (!Network_Server::start_as_server()) {
                Game_Manager::stop();
            }
        } else if (button_event == "start_server_lockstep") {
            Window_Manager::close_all_windows();

            Window* window = Window_Manager::get_window("start_server");

            Options::change_option("sv_network_name", window->get_info_text(0));
            Options::change_option("sv_network_password", window->get_info_text(1));

            Game_Manager::stop();

            Game_Manager::start_server_lockstep();

            if (!Network_Server::start_as_server()) {
                Game_Manager::stop();
            }
        } else if (button_event == "start_server_window") {
            Window* window = Window_Manager::get_window("start_server");

            window->set_info_text(0, Options::get_option_value("sv_network_name"));
            window->set_info_text(1, Options::get_option_value("sv_network_password"));

            window->toggle_on();
            window_opened_on_top = true;
        } else if (button_event == "server_list_window") {
            Network_Client::refresh_server_list();

            Window_Manager::get_window("server_list")->toggle_on();
            window_opened_on_top = true;
        } else if (button_event == "server_list_delete_window") {
            Network_Client::refresh_server_list();

            Window_Manager::get_window("server_list_delete")->toggle_on();
            window_opened_on_top = true;
        } else if (button_event == "server_list_edit_window") {
            Network_Client::refresh_server_list();

            Window_Manager::get_window("server_list_edit")->toggle_on();
            window_opened_on_top = true;
        } else if (button_event == "lan_server_list_window") {
            Network_LAN_Browser::lan_refresh();

            Window_Manager::get_window("lan_server_list")->toggle_on();
            window_opened_on_top = true;
        } else if (button_event == "add_server_window") {
            Window* window = Window_Manager::get_window("add_server");

            if (window->get_info_text(2).length() == 0) {
                window->set_info_text(2, Strings::num_to_string(Network_Server::port));
            }

            window->toggle_on();
            window_opened_on_top = true;
        } else if (button_event == "add_server") {
            if (parent_window != 0) {
                string password = parent_window->get_info_text(3);
                bool password_required = false;

                if (password.length() > 0) {
                    password_required = true;
                }

                Network_Client::add_server(parent_window->get_info_text(0), parent_window->get_info_text(
                                               1), Strings::string_to_unsigned_long(parent_window->get_info_text(
                                                                                        2)), &password, password_required, 0, 0, "",
                                           0);

                handle_button_event("close_window", parent_window);
            }
        } else if (button_event == "edit_server") {
            if (parent_window != 0) {
                Network_Client::edit_server(Engine::editing_server, parent_window->get_info_text(
                                                0), parent_window->get_info_text(1),
                                            Strings::string_to_unsigned_long(parent_window->get_info_text(
                                                                                 2)), parent_window->get_info_text(3));

                Window_Manager::get_window("server_list")->rebuild_scrolling_buttons();
                Window_Manager::get_window("server_list_delete")->rebuild_scrolling_buttons();
                Window_Manager::get_window("server_list_edit")->rebuild_scrolling_buttons();

                handle_button_event("close_window", parent_window);
            }
        } else if (boost::algorithm::starts_with(button_event, "configure_commands_")) {
            boost::algorithm::erase_first(button_event, "configure_commands_");

            Object_Manager::configure_command = Strings::string_to_long(button_event);
            Window_Manager::get_window("configure_command")->toggle_on(true, true);
            window_opened_on_top = true;
        } else if (boost::algorithm::starts_with(button_event, "server_list_delete_")) {
            boost::algorithm::erase_first(button_event, "server_list_delete_");

            Network_Client::remove_server(Strings::string_to_long(button_event));

            Window_Manager::get_window("server_list")->rebuild_scrolling_buttons();
            Window_Manager::get_window("server_list_delete")->rebuild_scrolling_buttons();
            Window_Manager::get_window("server_list_edit")->rebuild_scrolling_buttons();
        } else if (boost::algorithm::starts_with(button_event, "server_list_edit_")) {
            boost::algorithm::erase_first(button_event, "server_list_edit_");

            Window* window = Window_Manager::get_window("edit_server");
            Server* server = Network_Client::get_server(Strings::string_to_long(button_event));

            if (server != 0) {
                window->set_info_text(0, server->name);
                window->set_info_text(1, server->address);
                window->set_info_text(2, Strings::num_to_string(server->port));
                window->set_info_text(3, server->password);

                Engine::editing_server = Strings::string_to_long(button_event);

                window->toggle_on(true, true);
                window_opened_on_top = true;
            }
        } else if (boost::algorithm::starts_with(button_event, "server_list_")) {
            boost::algorithm::erase_first(button_event, "server_list_");

            int server_index = Strings::string_to_long(button_event);
            Server* server = Network_Client::get_server(server_index);

            if (server != 0) {
                // If the server requires a password but we don't have one saved
                if (server->password_required && server->password.length() == 0) {
                    Network_LAN_Browser::server_list_connecting_index = server_index;

                    Window_Manager::get_window("input_server_password")->toggle_on(true, true);
                    window_opened_on_top = true;
                } else {
                    Window_Manager::close_all_windows();

                    Game_Manager::stop();

                    Network_Client::set_server_target(server_index);

                    if (!Network_Client::start_as_client()) {
                        Game_Manager::stop();
                    }
                }
            }
        } else if (button_event == "refresh_server_list") {
            Network_Client::refresh_server_list();
        } else if (button_event == "lan_refresh") {
            Network_LAN_Browser::lan_refresh();
        } else if (button_event == "lan_refresh_quick") {
            Network_LAN_Browser::lan_refresh_quick();
        } else if (boost::algorithm::starts_with(button_event, "lan_server_list_save_")) {
            boost::algorithm::erase_first(button_event, "lan_server_list_save_");

            Server* server = Network_LAN_Browser::get_lan_server(Strings::string_to_long(button_event));

            if (server != 0) {
                bool server_added = Network_Client::add_server(server->name, server->address, server->port, 0,
                                                               server->password_required, server->slots_filled,
                                                               server->slots_total, server->version, server->ping);

                if (server_added) {
                    Engine::make_toast("Server added to list");
                } else {
                    Engine::make_toast("Server updated on list");
                }
            }
        } else if (boost::algorithm::starts_with(button_event, "lan_server_list_")) {
            boost::algorithm::erase_first(button_event, "lan_server_list_");

            int lan_server_index = Strings::string_to_long(button_event);
            Server* server = Network_LAN_Browser::get_lan_server(lan_server_index);

            if (server != 0) {
                if (server->password_required) {
                    Network_LAN_Browser::lan_connecting_index = lan_server_index;

                    Window_Manager::get_window("input_server_password")->toggle_on(true, true);
                    window_opened_on_top = true;
                } else {
                    Window_Manager::close_all_windows();

                    Game_Manager::stop();

                    Network_LAN_Browser::set_lan_server_target(lan_server_index);

                    if (!Network_Client::start_as_client()) {
                        Game_Manager::stop();
                    }
                }
            }
        } else if (button_event == "server_password") {
            Window_Manager::close_all_windows();

            int lan_server_index = Network_LAN_Browser::lan_connecting_index;
            int server_list_server_index = Network_LAN_Browser::server_list_connecting_index;

            Game_Manager::stop();

            Window* window = Window_Manager::get_window("input_server_password");

            if (lan_server_index != -1) {
                Network_LAN_Browser::set_lan_server_target(lan_server_index, window->get_info_text(0));
            } else {
                Network_Client::set_server_target(server_list_server_index, window->get_info_text(0));
            }

            window->set_info_text(0, "");

            if (!Network_Client::start_as_client()) {
                Game_Manager::stop();
            }
        } else {
            if (!handle_button_event_game(button_event, parent_window, window_opened_on_top)) {
                Log::add_error("Invalid button event: '" + button_event + "'");
            }
        }
    }

    return window_opened_on_top;
}
