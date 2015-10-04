/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "client_data.h"

using namespace std;

Client_Data::Client_Data(RakNet::RakNetGUID get_id,RakNet::SystemAddress get_address,string get_name,bool get_is_us){
    id=get_id;
    address=get_address;

    connected=false;
    name=get_name;
    ping=0;
    is_us=get_is_us;

    completed_turn=0;

    rate_bytes=0;
    rate_updates=0;

    bytes_this_second=0;
    updates_this_second=0;
    counter_update=0;
}

bool Client_Data::game_command_state(string command){
    for(size_t i=0;i<command_states.size();i++){
        if(command_states[i]==command){
            return true;
        }
    }

    return false;
}
