/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef network_server_h
#define network_server_h

#include "client_data.h"

#include <string>
#include <stdint.h>

class Network_Server{
public:

    static unsigned int max_clients;
    static unsigned short port;
    static std::string password;
    static std::string name;
    static bool frequent_connection_protection;
    static bool ignore_checksum;
    static uint32_t rate_bytes_min;
    static uint32_t rate_bytes_max;
    static uint32_t rate_updates_min;
    static uint32_t rate_updates_max;

    //Returns true if startup was successful
    //Returns false if startup failed
    static bool start_as_server(bool allow_clients=true);

    static void update_server_password();
    static void update_server_max_connections(unsigned int new_max,bool force);
    static void update_offline_ping_response();

    static uint32_t get_client_count();

    static void prepare_server_input_states();

    static void send_version();

    static void receive_client_data();

    static void send_name_change(std::string old_name,std::string new_name,bool own_name);

    static void send_initial_game_data();

    static void receive_connected();

    static void send_client_list();

    static void send_updates();
    static void send_update(Client_Data* client,uint32_t client_rate_bytes);

    static void receive_input();

    static void send_ping_list();

    static void send_paused();

    static void send_sound(std::string sound,RakNet::RakNetGUID target_id,bool broadcast);
};

#endif
