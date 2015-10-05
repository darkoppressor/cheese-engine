/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "network_server.h"
#include "network_engine.h"
#include "options.h"
#include "log.h"
#include "window_manager.h"
#include "engine.h"
#include "engine_strings.h"
#include "engine_version.h"
#include "engine_data.h"
#include "game_manager.h"
#include "network_message_identifiers.h"

#include "raknet/Source/BitStream.h"
#include "raknet/Source/GetTime.h"

using namespace std;

unsigned int Network_Server::max_clients=0;
unsigned short Network_Server::port=0;
string Network_Server::password="";
string Network_Server::name="";
bool Network_Server::frequent_connection_protection=false;
bool Network_Server::ignore_checksum=false;
uint32_t Network_Server::rate_bytes_min=0;
uint32_t Network_Server::rate_bytes_max=0;
uint32_t Network_Server::rate_updates_min=0;
uint32_t Network_Server::rate_updates_max=0;

bool Network_Server::start_as_server(bool allow_clients){
    if(Network_Engine::status=="off"){
        unsigned int actual_max_clients=max_clients;
        unsigned short actual_port=port;

        if(!allow_clients){
            actual_max_clients=0;
            actual_port=0;
        }

        Network_Engine::peer=RakNet::RakPeerInterface::GetInstance();

        RakNet::SocketDescriptor sd(actual_port,"");
        RakNet::StartupResult startup=Network_Engine::peer->Startup(max_clients,&sd,1);

        if(startup==RakNet::RAKNET_STARTED){
            Network_Engine::status="server";

            update_server_max_connections(actual_max_clients,true);

            Network_Engine::peer->SetUnreliableTimeout(500);

            Network_Engine::peer->SetOccasionalPing(true);

            Network_Engine::peer->SetLimitIPConnectionFrequency(frequent_connection_protection);

            update_server_password();

            Network_Engine::id=Network_Engine::peer->GetGuidFromSystemAddress(RakNet::UNASSIGNED_SYSTEM_ADDRESS);
            Network_Engine::address=Network_Engine::peer->GetSystemAddressFromGuid(Network_Engine::id);

            Network_Engine::clients.push_back(Client_Data(Network_Engine::id,Network_Engine::address,Options::name,true));
            Network_Engine::clients[0].connected=true;
            update_offline_ping_response();

            Network_Engine::timer_tick.start();

            return true;
        }
        else{
            string error_message="Error initializing server: "+Network_Engine::translate_startup_error(startup);

            Log::add_error(error_message);

            Window_Manager::get_window("main_menu")->toggle_on("on","on");

            Engine::make_notice(error_message);
        }
    }

    return false;
}

void Network_Server::update_server_password(){
    if(Network_Engine::status=="server"){
        if(password.length()>0){
            Network_Engine::peer->SetIncomingPassword(password.c_str(),password.length());
        }
        else{
            Network_Engine::peer->SetIncomingPassword(0,0);
        }

        update_offline_ping_response();
    }
}

void Network_Server::update_server_max_connections(unsigned int new_max,bool force){
    if(Network_Engine::status=="server"){
        //If the maximum incoming connections is set to 0, we are not allowing clients
        //If this is the case, do not allow the maximum incoming connections to get modified
        if(force || Network_Engine::peer->GetMaximumIncomingConnections()>0){
            Network_Engine::peer->SetMaximumIncomingConnections(new_max);

            update_offline_ping_response();
        }
    }
}

void Network_Server::update_offline_ping_response(){
    if(Network_Engine::status=="server"){
        string data="0@";
        if(password.length()>0){
            data="1@";
        }

        data+=Strings::num_to_string(get_client_count())+"@";
        data+=Strings::num_to_string(Network_Engine::peer->GetMaximumIncomingConnections()+1)+"@";

        data+=Engine_Version::get_version()+"@";

        data+=Engine_Data::game_title+"@";

        data+=name;

        Network_Engine::peer->SetOfflinePingResponse(data.c_str(),data.length());
    }
}

uint32_t Network_Server::get_client_count(){
    uint32_t client_count=0;

    if(Network_Engine::status=="server"){
        for(int i=0;i<Network_Engine::clients.size();i++){
            if(Network_Engine::clients[i].connected){
                client_count++;
            }
        }
    }

    return client_count;
}

void Network_Server::toggle_client_spectator(uint32_t client_index){
    if(Network_Engine::status=="server"){
        if(Network_Engine::clients.size()>0 && client_index<Network_Engine::clients.size()){
            if(!Network_Engine::clients[client_index].spectator || Network_Engine::get_player_count()<Options::max_players){
                Network_Engine::clients[client_index].spectator=!Network_Engine::clients[client_index].spectator;

                send_client_list();
            }
        }
    }
}

void Network_Server::prepare_server_input_states(){
    Network_Engine::clients[0].command_states.clear();

    Network_Engine::clients[0].command_states=Game_Manager::command_states;
}

void Network_Server::send_version(){
    if(Network_Engine::status=="server"){
        RakNet::BitStream bitstream;
        bitstream.Write((RakNet::MessageID)ID_GAME_VERSION);

        bitstream.WriteCompressed((RakNet::RakString)Engine_Data::game_title.c_str());

        bitstream.WriteCompressed((RakNet::RakString)Engine_Version::get_version().c_str());

        if(!ignore_checksum){
            bitstream.WriteCompressed((RakNet::RakString)Engine::CHECKSUM.c_str());
        }
        else{
            bitstream.WriteCompressed((RakNet::RakString)"");
        }

        bitstream.WriteCompressed((RakNet::RakString)Network_Engine::allow_new_connection().c_str());

        Network_Engine::stat_counter_bytes_sent+=bitstream.GetNumberOfBytesUsed();
        Network_Engine::peer->Send(&bitstream,MEDIUM_PRIORITY,RELIABLE_ORDERED,ORDERING_CHANNEL_CONNECTION,Network_Engine::packet->guid,false);
    }
}

void Network_Server::receive_client_data(){
    RakNet::BitStream bitstream(Network_Engine::packet->data,Network_Engine::packet->length,false);
    Network_Engine::stat_counter_bytes_received+=bitstream.GetNumberOfBytesUsed();

    RakNet::MessageID type_id;
    bitstream.Read(type_id);

    bool first_send=false;
    bitstream.ReadCompressed(first_send);

    RakNet::RakString rstring;
    bitstream.ReadCompressed(rstring);

    uint32_t client_rate_bytes=0;
    bitstream.ReadCompressed(client_rate_bytes);

    uint32_t client_rate_updates=0;
    bitstream.ReadCompressed(client_rate_updates);

    Client_Data* client=Network_Engine::get_packet_client();

    if(client!=0){
        string old_name=client->name;

        client->name=rstring.C_String();
        client->rate_bytes=client_rate_bytes;
        client->rate_updates=client_rate_updates;

        if(first_send){
            send_initial_game_data();
        }
        else if(client->name!=old_name){
            send_name_change(old_name,client->name,false);
        }
    }
}

void Network_Server::send_name_change(string old_name,string new_name,bool own_name){
    if(Network_Engine::status=="server"){
        if(own_name){
            Network_Engine::clients[0].name=new_name;
        }

        string msg=old_name+" has changed their name to "+new_name;

        Engine::add_chat(msg);
        Network_Engine::send_chat_message(msg,RakNet::UNASSIGNED_RAKNET_GUID,true);

        send_client_list();
    }
}

void Network_Server::send_initial_game_data(){
    if(Network_Engine::status=="server"){
        RakNet::BitStream bitstream;
        bitstream.Write((RakNet::MessageID)ID_GAME_INITIAL_GAME_DATA);

        bitstream.WriteCompressed(Engine::UPDATE_RATE);

        Network_Engine::write_initial_game_data(&bitstream);

        Network_Engine::stat_counter_bytes_sent+=bitstream.GetNumberOfBytesUsed();
        Network_Engine::peer->Send(&bitstream,MEDIUM_PRIORITY,RELIABLE_ORDERED,ORDERING_CHANNEL_CONNECTION,Network_Engine::packet->guid,false);
    }
}

void Network_Server::receive_connected(){
    RakNet::BitStream bitstream(Network_Engine::packet->data,Network_Engine::packet->length,false);
    Network_Engine::stat_counter_bytes_received+=bitstream.GetNumberOfBytesUsed();

    RakNet::MessageID type_id;
    bitstream.Read(type_id);

    Client_Data* client=Network_Engine::get_packet_client();

    if(client!=0){
        client->connected=true;
        update_offline_ping_response();

        string msg=client->name+" has connected from "+Network_Engine::packet->systemAddress.ToString(true);

        Engine::add_chat(msg);
        Network_Engine::send_chat_message(msg,Network_Engine::packet->guid,true);

        send_client_list();
    }
}

void Network_Server::send_client_list(){
    if(Network_Engine::status=="server"){
        int client_index=-1;

        for(int i=0;i<Network_Engine::clients.size();i++){
            Client_Data* client=&Network_Engine::clients[i];

            if(client->connected){
                client_index++;

                RakNet::BitStream bitstream;
                bitstream.Write((RakNet::MessageID)ID_GAME_CLIENT_LIST);

                int clients_size=0;
                for(int j=0;j<Network_Engine::clients.size();j++){
                    if(Network_Engine::clients[j].connected){
                        clients_size++;
                    }
                }

                bitstream.WriteCompressed(clients_size);
                for(int j=0;j<Network_Engine::clients.size();j++){
                    if(Network_Engine::clients[j].connected){
                        bitstream.WriteCompressed((RakNet::RakString)Network_Engine::clients[j].name.c_str());
                        bitstream.WriteCompressed(Network_Engine::clients[j].spectator);
                    }
                }

                bitstream.WriteCompressed(client_index);

                Network_Engine::stat_counter_bytes_sent+=bitstream.GetNumberOfBytesUsed();
                Network_Engine::peer->Send(&bitstream,HIGH_PRIORITY,RELIABLE_ORDERED,ORDERING_CHANNEL_CLIENT_LIST,client->id,false);
            }
        }
    }
}

void Network_Server::send_updates(){
    if(Network_Engine::status=="server"){
        if(Game_Manager::in_progress){
            for(int i=1;i<Network_Engine::clients.size();i++){
                if(Network_Engine::clients[i].connected){
                    uint32_t client_rate_bytes=Network_Engine::clients[i].rate_bytes;
                    if(rate_bytes_min!=0 && client_rate_bytes<rate_bytes_min){
                        client_rate_bytes=rate_bytes_min;
                    }
                    else if(rate_bytes_max!=0 && client_rate_bytes>rate_bytes_max){
                        client_rate_bytes=rate_bytes_max;
                    }

                    uint32_t client_rate_updates=Network_Engine::clients[i].rate_updates;
                    if(rate_updates_min!=0 && client_rate_updates<rate_updates_min){
                        client_rate_updates=rate_updates_min;
                    }
                    else if(rate_updates_max!=0 && client_rate_updates>rate_updates_max){
                        client_rate_updates=rate_updates_max;
                    }

                    if(Network_Engine::clients[i].updates_this_second<client_rate_updates && ++Network_Engine::clients[i].counter_update>=Engine::UPDATE_RATE/client_rate_updates){
                        Network_Engine::clients[i].counter_update=0;

                        send_update(&Network_Engine::clients[i],client_rate_bytes);
                    }
                }
            }
        }
    }
}

void Network_Server::send_update(Client_Data* client,uint32_t client_rate_bytes){
    if(Network_Engine::status=="server"){
        RakNet::BitStream bitstream;
        bitstream.Write((RakNet::MessageID)ID_TIMESTAMP);
        bitstream.Write(RakNet::GetTime());
        bitstream.Write((RakNet::MessageID)ID_GAME_UPDATE);

        Network_Engine::write_update(&bitstream);

        uint32_t stream_bytes=bitstream.GetNumberOfBytesUsed();
        if(client->bytes_this_second+stream_bytes<=client_rate_bytes){
            client->bytes_this_second+=stream_bytes;
            client->updates_this_second++;

            Network_Engine::stat_counter_bytes_sent+=stream_bytes;
            Network_Engine::peer->Send(&bitstream,HIGH_PRIORITY,UNRELIABLE,ORDERING_CHANNEL_UNORDERED,client->id,false);
        }
    }
}

void Network_Server::receive_input(){
    RakNet::BitStream bitstream(Network_Engine::packet->data,Network_Engine::packet->length,false);
    Network_Engine::stat_counter_bytes_received+=bitstream.GetNumberOfBytesUsed();

    RakNet::MessageID type_id;
    bitstream.Read(type_id);

    Client_Data* client=Network_Engine::get_packet_client();

    if(client!=0){
        client->command_states.clear();

        int command_buffer_size=0;
        bitstream.ReadCompressed(command_buffer_size);

        for(int i=0;i<command_buffer_size;i++){
            RakNet::RakString rstring;
            bitstream.ReadCompressed(rstring);

            client->command_buffer.push_back(rstring.C_String());
        }

        int command_states_size=0;
        bitstream.ReadCompressed(command_states_size);

        for(int i=0;i<command_states_size;i++){
            RakNet::RakString rstring;
            bitstream.ReadCompressed(rstring);

            client->command_states.push_back(rstring.C_String());
        }
    }
}

void Network_Server::send_ping_list(){
    if(Network_Engine::status=="server"){
        RakNet::BitStream bitstream;
        bitstream.Write((RakNet::MessageID)ID_GAME_PING_LIST);

        bitstream.WriteCompressed((int)Network_Engine::clients.size());
        for(int i=1;i<Network_Engine::clients.size();i++){
            int ping=0;
            if(Network_Engine::clients[i].connected){
                ping=Network_Engine::peer->GetAveragePing(Network_Engine::clients[i].id);
            }

            bitstream.WriteCompressed(ping);
        }

        Network_Engine::stat_counter_bytes_sent+=bitstream.GetNumberOfBytesUsed();
        Network_Engine::peer->Send(&bitstream,LOW_PRIORITY,RELIABLE_ORDERED,ORDERING_CHANNEL_CHAT,RakNet::UNASSIGNED_RAKNET_GUID,true);
    }
}

void Network_Server::send_paused(){
    if(Network_Engine::status=="server"){
        RakNet::BitStream bitstream;
        bitstream.Write((RakNet::MessageID)ID_GAME_PAUSED);

        Network_Engine::stat_counter_bytes_sent+=bitstream.GetNumberOfBytesUsed();
        Network_Engine::peer->Send(&bitstream,HIGH_PRIORITY,RELIABLE_ORDERED,ORDERING_CHANNEL_PAUSED,RakNet::UNASSIGNED_RAKNET_GUID,true);
    }
}

void Network_Server::send_sound(string sound,RakNet::RakNetGUID target_id,bool broadcast){
    if(Network_Engine::status=="server"){
        RakNet::BitStream bitstream;
        bitstream.Write((RakNet::MessageID)ID_GAME_SOUND);

        bitstream.WriteCompressed((RakNet::RakString)sound.c_str());

        Network_Engine::stat_counter_bytes_sent+=bitstream.GetNumberOfBytesUsed();
        Network_Engine::peer->Send(&bitstream,HIGH_PRIORITY,RELIABLE_ORDERED,ORDERING_CHANNEL_SOUND,target_id,broadcast);
    }
}

void Network_Server::send_server_ready(){
    if(Network_Engine::status=="server"){
        RakNet::BitStream bitstream;
        bitstream.Write((RakNet::MessageID)ID_GAME_SERVER_READY);

        Network_Engine::write_server_ready(&bitstream);

        Network_Engine::stat_counter_bytes_sent+=bitstream.GetNumberOfBytesUsed();
        Network_Engine::peer->Send(&bitstream,HIGH_PRIORITY,RELIABLE_ORDERED,ORDERING_CHANNEL_TURNS,RakNet::UNASSIGNED_RAKNET_GUID,true);
    }
}

void Network_Server::receive_client_ready(){
    RakNet::BitStream bitstream(Network_Engine::packet->data,Network_Engine::packet->length,false);
    Network_Engine::stat_counter_bytes_received+=bitstream.GetNumberOfBytesUsed();

    RakNet::MessageID type_id;
    bitstream.Read(type_id);

    uint32_t turn_completed=0;
    bitstream.ReadCompressed(turn_completed);

    Client_Data* client=Network_Engine::get_packet_client();

    if(client!=0){
        client->completed_turn=turn_completed;
    }
}
