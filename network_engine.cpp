/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "network_engine.h"

#include "engine.h"

string Network_Engine::status="off";
RakNet::RakPeerInterface* Network_Engine::peer=0;
RakNet::RakNetGUID Network_Engine::id=RakNet::UNASSIGNED_RAKNET_GUID;
RakNet::SystemAddress Network_Engine::address=RakNet::UNASSIGNED_SYSTEM_ADDRESS;
RakNet::Packet* Network_Engine::packet=0;
vector<Client_Data> Network_Engine::clients;
Timer Network_Engine::timer_tick;
uint64_t Network_Engine::stat_bytes_received=0;
uint64_t Network_Engine::stat_counter_bytes_received=0;
uint64_t Network_Engine::stat_bytes_sent=0;
uint64_t Network_Engine::stat_counter_bytes_sent=0;

RakNet::RakNetGUID Network_Engine::server_id=RakNet::UNASSIGNED_RAKNET_GUID;

void Network_Engine::reset(){
    peer=0;
    id=RakNet::UNASSIGNED_RAKNET_GUID;
    address=RakNet::UNASSIGNED_SYSTEM_ADDRESS;
    packet=0;
    stat_bytes_received=0;
    stat_counter_bytes_received=0;
    stat_bytes_sent=0;
    stat_counter_bytes_sent=0;

    clients.clear();
    timer_tick.stop();

    Network_Client::commands_this_second=0;
    Network_Client::counter_commands=0;
    Network_Client::last_update_time=0;
    Network_Client::command_buffer.clear();

    Network_LAN_Browser::lan_connecting_index=-1;
    Network_LAN_Browser::server_list_connecting_index=-1;

    server_id=RakNet::UNASSIGNED_RAKNET_GUID;
}

void Network_Engine::stop(){
    if(status!="off"){
        peer->DeallocatePacket(packet);

        peer->Shutdown(100);

        RakNet::RakPeerInterface::DestroyInstance(peer);

        reset();

        if(status=="client"){
            Engine::set_logic_update_rate(recall_update_rate);
        }

        status="off";
    }
}

void Network_Engine::tick(){
    if(status!="off" && timer_tick.get_ticks()>=1000){
        if(status=="server"){
            for(int i=0;i<clients.size();i++){
                clients[i].bytes_this_second=0;
                clients[i].updates_this_second=0;
            }

            send_ping_list();
        }
        else if(status=="client"){
            commands_this_second=0;
        }

        stat_bytes_received=stat_counter_bytes_received;
        stat_counter_bytes_received=0;

        stat_bytes_sent=stat_counter_bytes_sent;
        stat_counter_bytes_sent=0;

        timer_tick.start();
    }
}

void Network_Engine::receive_packets(){
    if(Network_Engine::status!="off"){
        tick();

        for(packet=peer->Receive();packet!=0;peer->DeallocatePacket(packet),packet=peer->Receive()){
            RakNet::MessageID packet_id;

            //If the packet has a timestamp
            if((RakNet::MessageID)packet->data[0]==ID_TIMESTAMP){
                packet_id=(RakNet::MessageID)packet->data[sizeof(RakNet::MessageID)+sizeof(RakNet::Time)];
            }
            //If the packet has no timestamp
            else{
                packet_id=(RakNet::MessageID)packet->data[0];
            }

            switch(packet_id){
                case ID_CONNECTION_REQUEST_ACCEPTED:
                    if(Network_Engine::status=="client"){
                        server_id=packet->guid;

                        Log::add_log("Connected to server: "+string(packet->systemAddress.ToString(true)));
                    }
                    break;

                case ID_CONNECTION_ATTEMPT_FAILED:
                    if(Network_Engine::status=="client"){
                        Log::add_log("Could not connect to server: "+server_address+"|"+Strings::num_to_string(server_port));

                        Button_Events::handle_button_event("stop_game");
                        Engine::make_notice("Could not connect to server.");
                    }
                    break;

                case ID_ALREADY_CONNECTED:
                    if(Network_Engine::status=="client"){
                        Log::add_log("Already connected to server: "+string(packet->systemAddress.ToString(true)));
                    }
                    break;

                case ID_NEW_INCOMING_CONNECTION:
                    if(Network_Engine::status=="server"){
                        //If this client is not already connected.
                        if(get_packet_client()==0){
                            clients.push_back(Client_Data(packet->guid,packet->systemAddress,"",false));

                            Network_Server::send_version();
                        }
                    }
                    break;

                case ID_NO_FREE_INCOMING_CONNECTIONS:
                    if(Network_Engine::status=="client"){
                        Log::add_log("Server is full: "+string(packet->systemAddress.ToString(true)));

                        Button_Events::handle_button_event("stop_game");
                        Engine::make_notice("Server is full.");
                    }
                    break;

                case ID_DISCONNECTION_NOTIFICATION:
                    if(Network_Engine::status=="server"){
                        Client_Data* client=get_packet_client();

                        if(client!=0){
                            if(client->name.length()>0){
                                string msg=client->name+" ("+packet->systemAddress.ToString(true)+") has disconnected";

                                Engine::add_chat(msg);
                                send_chat_message(msg,packet->guid,true);
                            }

                            int client_index=get_client_index(client);
                            if(client_index!=-1){
                                clients.erase(clients.begin()+client_index);
                                Network_Server::update_offline_ping_response();
                            }

                            Network_Server::send_client_list();
                        }
                    }
                    else{
                        Log::add_log("Disconnected from server: "+string(packet->systemAddress.ToString(true)));

                        Button_Events::handle_button_event("stop_game");
                        Engine::make_notice("Disconnected from server.");
                    }
                    break;

                case ID_CONNECTION_LOST:
                    if(Network_Engine::status=="server"){
                        Client_Data* client=get_packet_client();

                        if(client!=0){
                            string msg=client->name+" ("+packet->systemAddress.ToString(true)+") has dropped";

                            Engine::add_chat(msg);
                            send_chat_message(msg,packet->guid,true);

                            int client_index=get_client_index(client);
                            if(client_index!=-1){
                                clients.erase(clients.begin()+client_index);
                                Network_Server::update_offline_ping_response();
                            }

                            Network_Server::send_client_list();
                        }
                    }
                    else{
                        Log::add_log("Lost connection to server: "+string(packet->systemAddress.ToString(true)));

                        Button_Events::handle_button_event("stop_game");
                        Engine::make_notice("Lost connection to server.");
                    }
                    break;

                case ID_CONNECTION_BANNED:
                    if(Network_Engine::status=="client"){
                        Log::add_log("Banned from: "+string(packet->systemAddress.ToString(true)));

                        Button_Events::handle_button_event("stop_game");
                        Engine::make_notice("Banned from server.");
                    }
                    break;

                case ID_INVALID_PASSWORD:
                    if(Network_Engine::status=="client"){
                        Log::add_log("Invalid password for server: "+string(packet->systemAddress.ToString(true)));

                        Button_Events::handle_button_event("stop_game");
                        Engine::make_notice("Invalid password for server.");
                    }
                    break;

                case ID_INCOMPATIBLE_PROTOCOL_VERSION:
                    Log::add_log("Incompatible protocol version: "+string(packet->systemAddress.ToString(true))+"\nOur protocol: "+Strings::num_to_string((int)RAKNET_PROTOCOL_VERSION)+"\nServer protocol: "+receive_incompatible_protocol());

                    if(Network_Engine::status=="client"){
                        Button_Events::handle_button_event("stop_game");
                        Engine::make_notice("Network protocol does not match server.");
                    }
                    break;

                case ID_IP_RECENTLY_CONNECTED:
                    if(Network_Engine::status=="client"){
                        Log::add_log("Connected too recently to server: "+string(packet->systemAddress.ToString(true)));

                        Button_Events::handle_button_event("stop_game");
                        Engine::make_notice("Connected too recently to server.");
                    }
                    break;

                case ID_UNCONNECTED_PING:
                    //This happens when we are the server and someone pings us for information
                    //This is handled by RakNet, but I don't want unknown message ID logs for this
                    break;

                case ID_UNCONNECTED_PONG:
                    if(Network_Engine::status!="off"){
                        receive_server_browser_info();
                    }
                    break;

                case ID_GAME_VERSION:
                    if(Network_Engine::status=="client"){
                        Network_Client::receive_version();
                    }
                    break;

                case ID_GAME_CLIENT_DATA:
                    if(Network_Engine::status=="server"){
                        Network_Server::receive_client_data();
                    }
                    break;

                case ID_GAME_INITIAL_GAME_DATA:
                    if(Network_Engine::status=="client"){
                        Network_Client::receive_initial_game_data();
                    }
                    break;

                case ID_GAME_CONNECTED:
                    if(Network_Engine::status=="server"){
                        Network_Server::receive_connected();
                    }
                    break;

                case ID_GAME_CLIENT_LIST:
                    if(Network_Engine::status=="client"){
                        Network_Client::receive_client_list();
                    }
                    break;

                case ID_GAME_CHAT_MESSAGE:
                    receive_chat_message();
                    break;

                case ID_GAME_UPDATE:
                    if(Network_Engine::status=="client"){
                        Network_Client::receive_update();
                    }
                    break;

                case ID_GAME_INPUT:
                    if(Network_Engine::status=="server"){
                        Network_Server::receive_input();
                    }
                    break;

                case ID_GAME_PING_LIST:
                    if(Network_Engine::status=="client"){
                        Network_Client::receive_ping_list();
                    }
                    break;

                case ID_GAME_PAUSED:
                    if(Network_Engine::status=="client"){
                        Network_Client::receive_paused();
                    }
                    break;

                case ID_GAME_SOUND:
                    if(Network_Engine::status=="client"){
                        Network_Client::receive_sound();
                    }
                    break;

                default:
                    Log::add_log("Received message with unknown identifier: "+Strings::num_to_string((int)packet_id));
                    break;
            }

            if(Network_Engine::status=="off"){
                break;
            }
        }
    }
}

string Network_Engine::translate_startup_error(RakNet::StartupResult result){
    if(result==RakNet::RAKNET_ALREADY_STARTED){
        return "RakNet already started";
    }
    else if(result==RakNet::INVALID_SOCKET_DESCRIPTORS){
        return "invalid socket descriptors";
    }
    else if(result==RakNet::INVALID_MAX_CONNECTIONS){
        return "invalid max connections";
    }
    else if(result==RakNet::SOCKET_FAMILY_NOT_SUPPORTED){
        return "socket family not supported";
    }
    else if(result==RakNet::SOCKET_PORT_ALREADY_IN_USE){
        return "socket port already in use";
    }
    else if(result==RakNet::SOCKET_FAILED_TO_BIND){
        return "socket failed to bind";
    }
    else if(result==RakNet::SOCKET_FAILED_TEST_SEND){
        return "socket failed test send";
    }
    else if(result==RakNet::PORT_CANNOT_BE_ZERO){
        return "port cannot be zero";
    }
    else if(result==RakNet::FAILED_TO_CREATE_NETWORK_THREAD){
        return "failed to create network thread";
    }
    else if(result==RakNet::COULD_NOT_GENERATE_GUID){
        return "could not generate GUID";
    }
    else if(result==RakNet::STARTUP_OTHER_FAILURE){
        return "other failure";
    }
    else{
        return "unknown failure";
    }
}

string Network_Engine::get_name_list(){
    string str_list="Name\n----\n";

    for(int i=0;i<clients.size();i++){
        str_list+=clients[i].name+"\n";
    }

    return str_list;
}

string Network_Engine::get_ping_list(){
    string str_list="Ping\n----\n";

    if(status=="server"){
        str_list+="\n";

        for(int i=1;i<clients.size();i++){
            str_list+=Strings::num_to_string(peer->GetAveragePing(clients[i].id))+"\n";
        }
    }
    else if(status=="client"){
        str_list+=Strings::num_to_string(peer->GetAveragePing(server_id))+"\n";

        for(int i=1;i<clients.size();i++){
            str_list+=Strings::num_to_string(clients[i].ping)+"\n";
        }
    }

    return str_list;
}

string Network_Engine::get_stats(){
    string str_stats="";

    if(status!="off"){
        str_stats+="In: "+Strings::num_to_string((long double)stat_bytes_received/(long double)1000.0)+" kB/s\n";
        str_stats+="Out: "+Strings::num_to_string((long double)stat_bytes_sent/(long double)1000.0)+" kB/s\n";
    }

    return str_stats;
}

Client_Data* Network_Engine::get_packet_client(){
    for(int i=0;i<clients.size();i++){
        if(clients[i].id==packet->guid){
            return &clients[i];
        }
    }

    return 0;
}

int Network_Engine::get_client_index(Client_Data* client){
    for(int i=0;i<clients.size();i++){
        if(&clients[i]==client){
            return i;
        }
    }

    return -1;
}

vector<Client_Data*> Network_Engine::get_players(){
    vector<Client_Data*> players;

    for(uint32_t i=0,player_count=0;i<clients.size() && player_count<Options::max_players;i++){
        Client_Data* client=&clients[i];

        if(client->connected){
            players.push_back(client);

            player_count++;
        }
    }

    return players;
}

uint32_t Network_Engine::get_player_count(){
    return get_players().size();
}

int Network_Engine::get_our_player_number(){
    for(uint32_t i=0,player_count=0;i<clients.size() && player_count<Options::max_players;i++){
        Client_Data* client=&clients[i];

        if(client->connected){
            if(client->is_us){
                return player_count;
            }

            player_count++;
        }
    }

    return -1;
}

bool Network_Engine::client_name_taken(string name){
    for(int i=0;i<clients.size();i++){
        if(clients[i].name==name){
            return true;
        }
    }

    return false;
}

string Network_Engine::receive_incompatible_protocol(){
    RakNet::BitStream bitstream(packet->data,packet->length,false);
    stat_counter_bytes_received+=bitstream.GetNumberOfBytesUsed();

    RakNet::MessageID type_id;
    bitstream.Read(type_id);

    unsigned char version;
    bitstream.Read(version);

    return Strings::num_to_string((int)version);
}

void Network_Engine::receive_chat_message(){
    RakNet::BitStream bitstream(packet->data,packet->length,false);
    stat_counter_bytes_received+=bitstream.GetNumberOfBytesUsed();

    RakNet::MessageID type_id;
    bitstream.Read(type_id);

    RakNet::RakString rstring;
    bitstream.ReadCompressed(rstring);

    Engine::add_chat(rstring.C_String());

    if(status=="server"){
        send_chat_message(rstring.C_String(),packet->guid,true);
    }
}

void Network_Engine::send_chat_message(string message,RakNet::RakNetGUID target_id,bool broadcast){
    if(status!="off"){
        RakNet::BitStream bitstream;
        bitstream.Write((RakNet::MessageID)ID_GAME_CHAT_MESSAGE);

        bitstream.WriteCompressed((RakNet::RakString)message.c_str());

        stat_counter_bytes_sent+=bitstream.GetNumberOfBytesUsed();
        peer->Send(&bitstream,LOW_PRIORITY,RELIABLE_ORDERED,ORDERING_CHANNEL_CHAT,target_id,broadcast);
    }
}

void Network_Engine::add_command(string command){
    if(status=="client"){
        command_buffer.push_back(command);
    }
    else if(status=="server"){
        clients[0].command_buffer.push_back(command);
    }
}
