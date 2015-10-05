/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef network_engine_h
#define network_engine_h

#include "client_data.h"
#include "timer.h"

#include <string>
#include <vector>
#include <cstdint>

#include "raknet/Source/BitStream.h"

class Network_Engine{
public:

    static std::string status;
    static RakNet::RakPeerInterface* peer;
    static RakNet::RakNetGUID id;
    static RakNet::SystemAddress address;
    static RakNet::Packet* packet;
    static std::vector<Client_Data> clients;
    //Keeps track of each passing second and resets client update counts (server) or command packet counts (client)
    static Timer timer_tick;
    static std::uint64_t stat_bytes_received;
    static std::uint64_t stat_counter_bytes_received;
    static std::uint64_t stat_bytes_sent;
    static std::uint64_t stat_counter_bytes_sent;

    static RakNet::RakNetGUID server_id;

    static void reset();
    static void stop();

    static void tick();

    static void receive_packets();

    static std::string translate_startup_error(RakNet::StartupResult result);

    static std::string get_name_list();
    static std::string get_ping_list();
    static std::string get_stats();

    static Client_Data* get_packet_client();
    static int get_client_index(Client_Data* client);

    //Returns a list of clients that are currently players
    static std::vector<Client_Data*> get_players();
    static std::uint32_t get_player_count();
    //Returns our player number, or -1 if we are not a player
    static int get_our_player_number();

    static bool client_name_taken(std::string name);

    static std::string receive_incompatible_protocol();

    static void receive_chat_message();
    static void send_chat_message(std::string message,RakNet::RakNetGUID target_id,bool broadcast);

    static void add_command(std::string command);

    static bool receive_game_packet(RakNet::Packet* packet,const RakNet::MessageID& packet_id);

    static std::string allow_new_connection();

    static void write_initial_game_data(RakNet::BitStream* bitstream);
    static void read_initial_game_data(RakNet::BitStream* bitstream);

    static void write_update(RakNet::BitStream* bitstream);
    static void read_update(RakNet::BitStream* bitstream);

    static void write_server_ready(RakNet::BitStream* bitstream);
    static void read_server_ready(RakNet::BitStream* bitstream);
};

#endif
