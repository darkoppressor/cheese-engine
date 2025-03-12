/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef network_server_h
#define network_server_h

#include "client_data.h"

#include <string>
#include <cstdint>

class Network_Server {
    public:
        static unsigned int max_clients;
        static unsigned short port;
        static std::string password;
        static std::string name;
        static bool frequent_connection_protection;
        static bool ignore_checksum;
        static std::uint32_t rate_bytes_min;
        static std::uint32_t rate_bytes_max;
        static std::uint32_t rate_updates_min;
        static std::uint32_t rate_updates_max;

        // Returns true if startup was successful
        // Returns false if startup failed
        static bool start_as_server(bool allow_clients = true);
        static void update_server_password();
        static void update_server_max_connections(unsigned int new_max, bool force);
        static void update_offline_ping_response();
        static std::uint32_t get_client_count();
        static void toggle_client_spectator(std::uint32_t client_index);
        static void prepare_server_input_states();
        static void send_version();
        static void receive_client_data();
        static void send_name_change(std::string old_name, std::string new_name, bool own_name);
        static void send_initial_game_data();
        static void receive_connected();
        static void send_client_list();
        static void send_updates();
        static void send_update(Client_Data* client, std::uint32_t client_rate_bytes);
        static void receive_input();
        static void send_ping_list();
        static void send_paused();
        static void send_sound(std::string sound, RakNet::RakNetGUID target_id, bool broadcast);

        // Tell all clients to begin the next turn
        // This also carries a list of all commands to be executed on this turn
        // This is used in the lockstep networking model
        static void send_server_ready();

        // Receive notification from a client that it has completed the current turn
        // This is used in the lockstep networking model
        static void receive_client_ready();
};

#endif
