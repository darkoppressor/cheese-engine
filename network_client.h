/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef network_client_h
#define network_client_h

#include "server.h"

#include <vector>
#include <string>
#include <cstdint>

#include "raknet/Source/RakPeerInterface.h"

class Network_Client {
    public:
        static std::vector<Server> server_list;
        static std::string server_address;
        static unsigned short server_port;
        static std::string server_password;
        static std::uint32_t rate_bytes;
        static std::uint32_t rate_updates;
        static std::uint32_t rate_commands;
        static std::uint32_t commands_this_second;
        static std::uint32_t counter_commands;
        // When a client connects to a server, its Engine::UPDATE_RATE is set to the server's
        // When the game is stopped, the client reverts to its original Engine::UPDATE_RATE
        static std::uint32_t recall_update_rate;
        static RakNet::Time last_update_time;
        static std::vector<std::string> command_buffer;
        static void set_server_target(std::string get_address, unsigned short get_port, std::string get_password);
        static void set_server_target(int index, std::string get_password = "");
        // Pass 0 for get_password to not touch the password
        // Returns true if the server was added
        // Retursn false if the server was updated
        static bool add_server(std::string get_name, std::string get_address, unsigned short get_port,
                               const std::string* get_password, bool password_required, std::uint32_t slots_filled,
                               std::uint32_t slots_total, std::string version, int ping);
        // Update the server with the passed address/port, do not add it if it is not in the list
        // This ONLY updates transient data, and it DOES NOT touch the server name
        static void update_server(std::string get_address, unsigned short get_port, bool password_required,
                                  std::uint32_t slots_filled, std::uint32_t slots_total, std::string version, int ping);
        static void remove_server(int index);
        static void edit_server(int index, std::string get_name, std::string get_address, unsigned short get_port,
                                std::string get_password);
        static Server* get_server(int index);
        static void refresh_server_list();

        // Returns true if startup was successful
        // Returns false if startup failed
        static bool start_as_client();
        static void connect_to_server();
        static void receive_version();
        static void send_client_data(bool first_send = false);
        static void receive_initial_game_data();
        static void send_connected();
        static void receive_client_list();
        static void receive_update();
        static void send_input();
        static void receive_ping_list();
        static void receive_paused();
        static void receive_sound();

        // Receive notification from the server that we can start the next turn
        // This also carries a list of all commands to be executed on this turn
        // This is used in the lockstep networking model
        static void receive_server_ready();

        // Notify the server that we are done with this turn
        // This is used in the lockstep networking model
        static void send_client_ready();
};

#endif
