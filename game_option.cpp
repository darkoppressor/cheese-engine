/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game_option.h"
#include "engine_strings.h"
#include "options.h"
#include "network_client.h"
#include "network_server.h"
#include "log.h"
#include "controller_manager.h"
#include "game_window.h"

using namespace std;

Game_Option::Game_Option(){
    name="";
    description="";
}

string Game_Option::get_value(){
    if(name=="cl_effect_limit"){
        return Strings::num_to_string(Options::effect_limit);
    }
    else if(name=="cl_screen_shake"){
        return Strings::bool_to_string(Options::screen_shake);
    }
    else if(name=="cl_rumble"){
        return Strings::bool_to_string(Options::rumble);
    }
    else if(name=="cl_screensaver"){
        return Strings::bool_to_string(Options::screensaver);
    }
    else if(name=="cl_camera_speed"){
        return Strings::num_to_string(Options::camera_speed);
    }
    else if(name=="cl_camera_zoom"){
        return Strings::num_to_string(Options::camera_zoom);
    }
    else if(name=="cl_name"){
        return Options::name;
    }
    else if(name=="cl_chat_timestamps"){
        return Strings::bool_to_string(Options::chat_timestamps);
    }
    else if(name=="cl_network_rate_bytes"){
        return Strings::num_to_string(Network_Client::rate_bytes);
    }
    else if(name=="cl_network_rate_updates"){
        return Strings::num_to_string(Network_Client::rate_updates);
    }
    else if(name=="cl_network_rate_commands"){
        return Strings::num_to_string(Network_Client::rate_commands);
    }
    else if(name=="sv_max_players"){
        return Strings::num_to_string(Options::max_players);
    }
    else if(name=="sv_network_max_clients"){
        return Strings::num_to_string(Network_Server::max_clients);
    }
    else if(name=="sv_network_port"){
        return Strings::num_to_string(Network_Server::port);
    }
    else if(name=="sv_network_password"){
        return Network_Server::password;
    }
    else if(name=="sv_network_name"){
        return Network_Server::name;
    }
    else if(name=="sv_network_frequent_connection_protection"){
        return Strings::bool_to_string(Network_Server::frequent_connection_protection);
    }
    else if(name=="sv_network_ignore_checksum"){
        return Strings::bool_to_string(Network_Server::ignore_checksum);
    }
    else if(name=="sv_network_rate_bytes_min"){
        return Strings::num_to_string(Network_Server::rate_bytes_min);
    }
    else if(name=="sv_network_rate_bytes_max"){
        return Strings::num_to_string(Network_Server::rate_bytes_max);
    }
    else if(name=="sv_network_rate_updates_min"){
        return Strings::num_to_string(Network_Server::rate_updates_min);
    }
    else if(name=="sv_network_rate_updates_max"){
        return Strings::num_to_string(Network_Server::rate_updates_max);
    }
    else{
        string value="";

        if(!game_get_value(value)){
            Log::add_error("Invalid game option: '"+name+"'");

            return "";
        }
        else{
            return value;
        }
    }
}

void Game_Option::set_value(string new_value){
    if(name=="cl_effect_limit"){
        Options::effect_limit=Strings::string_to_unsigned_long(new_value);
    }
    else if(name=="cl_screen_shake"){
        Options::screen_shake=Strings::string_to_bool(new_value);
    }
    else if(name=="cl_rumble"){
        Options::rumble=Strings::string_to_bool(new_value);

        if(!Options::rumble){
            Controller_Manager::stop_rumble(Controller_Manager::CONTROLLER_ID_ALL);
        }
    }
    else if(name=="cl_screensaver"){
        Options::screensaver=Strings::string_to_bool(new_value);

        Game_Window::set_sdl_hints();
    }
    else if(name=="cl_camera_speed"){
        Options::camera_speed=Strings::string_to_double(new_value);
    }
    else if(name=="cl_camera_zoom"){
        Options::camera_zoom=Strings::string_to_double(new_value);
    }
    else if(name=="cl_name"){
        string old_name=Options::name;

        Options::name=new_value;

        Network_Server::send_name_change(old_name,Options::name,true);
        Network_Client::send_client_data();
    }
    else if(name=="cl_chat_timestamps"){
        Options::chat_timestamps=Strings::string_to_bool(new_value);
    }
    else if(name=="cl_network_rate_bytes"){
        Network_Client::rate_bytes=Strings::string_to_unsigned_long(new_value);

        Network_Client::send_client_data();
    }
    else if(name=="cl_network_rate_updates"){
        Network_Client::rate_updates=Strings::string_to_unsigned_long(new_value);

        Network_Client::send_client_data();
    }
    else if(name=="cl_network_rate_commands"){
        Network_Client::rate_commands=Strings::string_to_unsigned_long(new_value);
    }
    else if(name=="sv_max_players"){
        Options::max_players=Strings::string_to_unsigned_long(new_value);
    }
    else if(name=="sv_network_max_clients"){
        Network_Server::max_clients=Strings::string_to_unsigned_long(new_value);

        Network_Server::update_server_max_connections(Network_Server::max_clients,false);
    }
    else if(name=="sv_network_port"){
        Network_Server::port=Strings::string_to_unsigned_long(new_value);
    }
    else if(name=="sv_network_password"){
        if(new_value.length()>32){
            new_value.resize(32);
        }

        Network_Server::password=new_value;

        Network_Server::update_server_password();
    }
    else if(name=="sv_network_name"){
        if(new_value.length()>48){
            new_value.resize(48);
        }

        Network_Server::name=new_value;

        Network_Server::update_offline_ping_response();
    }
    else if(name=="sv_network_frequent_connection_protection"){
        Network_Server::frequent_connection_protection=Strings::string_to_bool(new_value);
    }
    else if(name=="sv_network_ignore_checksum"){
        Network_Server::ignore_checksum=Strings::string_to_bool(new_value);
    }
    else if(name=="sv_network_rate_bytes_min"){
        Network_Server::rate_bytes_min=Strings::string_to_unsigned_long(new_value);
    }
    else if(name=="sv_network_rate_bytes_max"){
        Network_Server::rate_bytes_max=Strings::string_to_unsigned_long(new_value);
    }
    else if(name=="sv_network_rate_updates_min"){
        Network_Server::rate_updates_min=Strings::string_to_unsigned_long(new_value);
    }
    else if(name=="sv_network_rate_updates_max"){
        Network_Server::rate_updates_max=Strings::string_to_unsigned_long(new_value);
    }
    else{
        game_set_value(new_value);
    }
}
