/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef client_data_h
#define client_data_h

#include <vector>
#include <string>
#include <cstdint>

#include "raknet/Source/RakPeerInterface.h"

class Client_Data{
public:

    RakNet::RakNetGUID id;
    RakNet::SystemAddress address;

    bool connected;
    std::string name;
    int ping;
    bool is_us;
    bool spectator;

    //The most recently completed turn (used in the lockstep networking model)
    std::uint32_t completed_turn;

    std::uint32_t rate_bytes;
    std::uint32_t rate_updates;

    //The number of bytes sent to this client over the duration of the current second
    std::uint32_t bytes_this_second;
    //The number of updates sent to this client over the duration of the current second
    std::uint32_t updates_this_second;
    std::uint32_t counter_update;

    std::vector<std::string> command_buffer;
    std::vector<std::string> command_states;

    Client_Data(RakNet::RakNetGUID get_id,RakNet::SystemAddress get_address,std::string get_name,bool get_is_us);

    bool game_command_state(std::string command);
};

#endif
