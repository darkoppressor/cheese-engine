/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "network_client.h"
#include "engine.h"
#include "window_manager.h"
#include "options.h"
#include "network_engine.h"
#include "network_server.h"
#include "log.h"
#include "engine_strings.h"
#include "engine_data.h"
#include "button_events.h"
#include "engine_version.h"
#include "game_manager.h"
#include "sound_manager.h"
#include "network_message_identifiers.h"
#include "network_lockstep.h"

#include "raknet/Source/BitStream.h"
#include "raknet/Source/GetTime.h"

using namespace std;

vector<Server> Network_Client::server_list;
string Network_Client::server_address = "";
unsigned short Network_Client::server_port = 0;
string Network_Client::server_password = "";
uint32_t Network_Client::rate_bytes = 0;
uint32_t Network_Client::rate_updates = 0;
uint32_t Network_Client::rate_commands = 0;
uint32_t Network_Client::commands_this_second = 0;
uint32_t Network_Client::counter_commands = 0;
uint32_t Network_Client::recall_update_rate = Engine::UPDATE_RATE;
RakNet::Time Network_Client::last_update_time = 0;
vector<string> Network_Client::command_buffer;

void Network_Client::set_server_target (string get_address, unsigned short get_port, string get_password) {
    server_address = get_address;
    server_port = get_port;
    server_password = get_password;
}

void Network_Client::set_server_target (int index, string get_password) {
    if (index < server_list.size()) {
        server_address = server_list[index].address;
        server_port = server_list[index].port;

        if (get_password.length() > 0) {
            server_password = get_password;
        } else {
            server_password = server_list[index].password;
        }
    }
}

bool Network_Client::add_server (string get_name, string get_address, unsigned short get_port,
                                 const string* get_password, bool password_required, uint32_t slots_filled,
                                 uint32_t slots_total, string version, int ping) {
    bool added = false;
    int existing_index = -1;

    for (int i = 0; i < server_list.size(); i++) {
        if (server_list[i].matches(get_address, get_port)) {
            existing_index = i;
            break;
        }
    }

    // This server is not already in the list
    if (existing_index == -1) {
        added = true;

        server_list.push_back(Server());
        server_list.back().name = get_name;
        server_list.back().address = get_address;
        server_list.back().port = get_port;

        if (get_password != 0) {
            server_list.back().password = *get_password;
        }

        server_list.back().password_required = password_required;
        server_list.back().slots_filled = slots_filled;
        server_list.back().slots_total = slots_total;
        server_list.back().version = version;
        server_list.back().ping = ping;
    }
    // This server is already in the list
    else {
        server_list[existing_index].name = get_name;

        if (get_password != 0) {
            server_list[existing_index].password = *get_password;
        }

        server_list[existing_index].password_required = password_required;
        server_list[existing_index].slots_filled = slots_filled;
        server_list[existing_index].slots_total = slots_total;
        server_list[existing_index].version = version;
        server_list[existing_index].ping = ping;
    }

    Window_Manager::get_window("server_list")->rebuild_scrolling_buttons();
    Window_Manager::get_window("server_list_delete")->rebuild_scrolling_buttons();
    Window_Manager::get_window("server_list_edit")->rebuild_scrolling_buttons();

    Options::save_servers();

    return added;
}

void Network_Client::update_server (string get_address, unsigned short get_port, bool password_required,
                                    uint32_t slots_filled, uint32_t slots_total, string version, int ping) {
    int existing_index = -1;

    for (int i = 0; i < server_list.size(); i++) {
        if (server_list[i].matches(get_address, get_port)) {
            existing_index = i;
            break;
        }
    }

    if (existing_index != -1) {
        server_list[existing_index].password_required = password_required;
        server_list[existing_index].slots_filled = slots_filled;
        server_list[existing_index].slots_total = slots_total;
        server_list[existing_index].version = version;
        server_list[existing_index].ping = ping;

        Window_Manager::get_window("server_list")->rebuild_scrolling_buttons();
        Window_Manager::get_window("server_list_delete")->rebuild_scrolling_buttons();
        Window_Manager::get_window("server_list_edit")->rebuild_scrolling_buttons();
    }
}

void Network_Client::remove_server (int index) {
    if (index < server_list.size()) {
        server_list.erase(server_list.begin() + index);

        Options::save_servers();
    }
}

void Network_Client::edit_server (int index, string get_name, string get_address, unsigned short get_port,
                                  string get_password) {
    if (index < server_list.size()) {
        Server* ptr_server = &server_list[index];

        ptr_server->name = get_name;
        ptr_server->address = get_address;
        ptr_server->port = get_port;
        ptr_server->password = get_password;

        Options::save_servers();
    }
}

Server* Network_Client::get_server (int index) {
    Server* ptr_server = 0;

    if (index < server_list.size()) {
        ptr_server = &server_list[index];
    }

    return ptr_server;
}

void Network_Client::refresh_server_list () {
    if (Network_Engine::status == "off") {
        Network_Server::start_as_server(false);
    }

    for (int i = 0; i < server_list.size(); i++) {
        Network_Engine::peer->Ping(server_list[i].address.c_str(), server_list[i].port, false);
    }
}

bool Network_Client::start_as_client () {
    if (Network_Engine::status == "off") {
        Network_Engine::peer = RakNet::RakPeerInterface::GetInstance();

        RakNet::SocketDescriptor sd;
        RakNet::StartupResult startup = Network_Engine::peer->Startup(1, &sd, 1);

        if (startup == RakNet::RAKNET_STARTED) {
            Network_Engine::status = "client";
            Network_Engine::id = Network_Engine::peer->GetGuidFromSystemAddress(RakNet::UNASSIGNED_SYSTEM_ADDRESS);
            Network_Engine::address = Network_Engine::peer->GetSystemAddressFromGuid(Network_Engine::id);

            connect_to_server();

            return true;
        } else {
            string error_message = "Error initializing client: " + Network_Engine::translate_startup_error(startup);

            Log::add_error(error_message);

            Window_Manager::get_window("main_menu")->toggle_on("on", "on");

            Engine::make_notice(error_message);
        }
    }

    return false;
}

void Network_Client::connect_to_server () {
    if (Network_Engine::status == "client") {
        if (server_password.length() == 0) {
            Network_Engine::peer->Connect(server_address.c_str(), server_port, 0, 0);
        } else {
            Network_Engine::peer->Connect(server_address.c_str(), server_port, server_password.c_str(),
                                          server_password.length());
        }

        Log::add_log("Attempting to connect to server: " + server_address + "|" + Strings::num_to_string(server_port));

        Window_Manager::get_window("network_connecting")->toggle_on(true, true);
    }
}

void Network_Client::receive_version () {
    RakNet::BitStream bitstream(Network_Engine::packet->data, Network_Engine::packet->length, false);

    Network_Engine::stat_counter_bytes_received += bitstream.GetNumberOfBytesUsed();

    RakNet::MessageID type_id;

    bitstream.Read(type_id);

    RakNet::RakString rstring;

    bitstream.ReadCompressed(rstring);

    string game_title = rstring.C_String();

    bitstream.ReadCompressed(rstring);

    string version = rstring.C_String();

    bitstream.ReadCompressed(rstring);

    string checksum = rstring.C_String();

    bitstream.ReadCompressed(rstring);

    string allow_new_connection = rstring.C_String();
    string our_game_title = Engine_Data::game_title;

    if (game_title != our_game_title) {
        Log::add_log("Game mismatch: " + string(Network_Engine::packet->systemAddress.ToString(true)) + "\nOur game: " +
                     our_game_title + "\nServer game: " + game_title);

        Button_Events::handle_button_event("stop_game");
        Engine::make_notice("Server is running a different game.");
    } else {
        string our_version = Engine_Version::get_version();

        if (version != our_version) {
            Log::add_log("Version mismatch: " + string(Network_Engine::packet->systemAddress.ToString(true)) +
                         "\nOur version: " + our_version + "\nServer version: " + version);

            Button_Events::handle_button_event("stop_game");
            Engine::make_notice("Version mismatch with server.");
        } else {
            if (checksum.length() > 0 && checksum != Engine::CHECKSUM) {
                Log::add_log("Checksum mismatch: " + string(Network_Engine::packet->systemAddress.ToString(true)) +
                             "\nOur checksum: " + Engine::CHECKSUM + "\nServer checksum: " + checksum);

                Button_Events::handle_button_event("stop_game");
                Engine::make_notice("Checksum mismatch with server.");
            } else {
                if (allow_new_connection.length() > 0) {
                    Log::add_log("Server rejected connection attempt: " + allow_new_connection);

                    Button_Events::handle_button_event("stop_game");
                    Engine::make_notice("Server rejected connection attempt: " + allow_new_connection);
                } else {
                    Window_Manager::get_window("network_connecting")->toggle_on(true, false);

                    send_client_data(true);
                }
            }
        }
    }
}

void Network_Client::send_client_data (bool first_send) {
    if (Network_Engine::status == "client") {
        char ordering_channel = ORDERING_CHANNEL_CLIENT_DATA;

        if (first_send) {
            ordering_channel = ORDERING_CHANNEL_CONNECTION;
        }

        RakNet::BitStream bitstream;

        bitstream.Write((RakNet::MessageID) ID_GAME_CLIENT_DATA);

        bitstream.WriteCompressed(first_send);
        bitstream.WriteCompressed((RakNet::RakString) Options::name.c_str());
        bitstream.WriteCompressed(rate_bytes);
        bitstream.WriteCompressed(rate_updates);

        Network_Engine::stat_counter_bytes_sent += bitstream.GetNumberOfBytesUsed();
        Network_Engine::peer->Send(&bitstream, MEDIUM_PRIORITY, RELIABLE_ORDERED, ordering_channel,
                                   Network_Engine::server_id, false);
    }
}

void Network_Client::receive_initial_game_data () {
    RakNet::BitStream bitstream(Network_Engine::packet->data, Network_Engine::packet->length, false);

    Network_Engine::stat_counter_bytes_received += bitstream.GetNumberOfBytesUsed();

    RakNet::MessageID type_id;

    bitstream.Read(type_id);

    uint32_t update_rate = 0;

    bitstream.ReadCompressed(update_rate);

    Engine::set_logic_update_rate(update_rate);

    Game_Manager::start_client();

    Network_Engine::read_initial_game_data(bitstream);

    Network_Engine::timer_tick.start();

    send_connected();
}

void Network_Client::send_connected () {
    if (Network_Engine::status == "client") {
        RakNet::BitStream bitstream;

        bitstream.Write((RakNet::MessageID) ID_GAME_CONNECTED);

        Network_Engine::stat_counter_bytes_sent += bitstream.GetNumberOfBytesUsed();
        Network_Engine::peer->Send(&bitstream, MEDIUM_PRIORITY, RELIABLE_ORDERED, ORDERING_CHANNEL_CONNECTION,
                                   Network_Engine::server_id, false);
    }
}

void Network_Client::receive_client_list () {
    Network_Engine::clients.clear();

    RakNet::BitStream bitstream(Network_Engine::packet->data, Network_Engine::packet->length, false);

    Network_Engine::stat_counter_bytes_received += bitstream.GetNumberOfBytesUsed();

    RakNet::MessageID type_id;

    bitstream.Read(type_id);

    int clients_size = 0;

    bitstream.ReadCompressed(clients_size);

    for (int i = 0; i < clients_size; i++) {
        RakNet::RakString rstring;

        bitstream.ReadCompressed(rstring);

        bool spectator = false;

        bitstream.ReadCompressed(spectator);

        Network_Engine::clients.push_back(Client_Data(RakNet::UNASSIGNED_RAKNET_GUID, RakNet::UNASSIGNED_SYSTEM_ADDRESS,
                                                      rstring.C_String(), false));
        Network_Engine::clients.back().spectator = spectator;
        Network_Engine::clients.back().connected = true;
    }

    int client_index = 0;

    bitstream.ReadCompressed(client_index);

    if (client_index >= 0 && Network_Engine::clients.size() > client_index) {
        Network_Engine::clients[client_index].is_us = true;
    }
}

void Network_Client::receive_update () {
    RakNet::BitStream bitstream(Network_Engine::packet->data, Network_Engine::packet->length, false);

    Network_Engine::stat_counter_bytes_received += bitstream.GetNumberOfBytesUsed();

    RakNet::MessageID timestamp_id;

    bitstream.Read(timestamp_id);

    RakNet::Time timestamp;

    bitstream.Read(timestamp);

    RakNet::MessageID type_id;

    bitstream.Read(type_id);

    if (RakNet::LessThan(last_update_time, timestamp)) {
        last_update_time = timestamp;

        Network_Engine::read_update(bitstream);
    }
}

void Network_Client::send_input () {
    if (Network_Engine::status == "client") {
        if (Game_Manager::in_progress) {
            if (commands_this_second < rate_commands && ++counter_commands >= Engine::UPDATE_RATE / rate_commands) {
                counter_commands = 0;

                commands_this_second++;

                RakNet::BitStream bitstream;

                bitstream.Write((RakNet::MessageID) ID_GAME_INPUT);

                bitstream.WriteCompressed((int) command_buffer.size());

                for (int i = 0; i < command_buffer.size(); i++) {
                    bitstream.WriteCompressed((RakNet::RakString) command_buffer[i].c_str());
                }

                command_buffer.clear();

                bitstream.WriteCompressed((int) Game_Manager::command_states.size());

                for (int i = 0; i < Game_Manager::command_states.size(); i++) {
                    bitstream.WriteCompressed((RakNet::RakString) Game_Manager::command_states[i].c_str());
                }

                Network_Engine::stat_counter_bytes_sent += bitstream.GetNumberOfBytesUsed();
                Network_Engine::peer->Send(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, ORDERING_CHANNEL_INPUT,
                                           Network_Engine::server_id, false);
            }
        }
    }
}

void Network_Client::receive_ping_list () {
    RakNet::BitStream bitstream(Network_Engine::packet->data, Network_Engine::packet->length, false);

    Network_Engine::stat_counter_bytes_received += bitstream.GetNumberOfBytesUsed();

    RakNet::MessageID type_id;

    bitstream.Read(type_id);

    int clients_size = 0;

    bitstream.ReadCompressed(clients_size);

    for (int i = 1; i < clients_size; i++) {
        int ping = 0;

        bitstream.ReadCompressed(ping);

        if (i < Network_Engine::clients.size()) {
            Network_Engine::clients[i].ping = ping;
        }
    }

    if (Network_Engine::clients.size() > 0) {
        Network_Engine::clients[0].ping = Network_Engine::peer->GetAveragePing(Network_Engine::server_id);
    }
}

void Network_Client::receive_paused () {
    RakNet::BitStream bitstream(Network_Engine::packet->data, Network_Engine::packet->length, false);

    Network_Engine::stat_counter_bytes_received += bitstream.GetNumberOfBytesUsed();

    RakNet::MessageID type_id;

    bitstream.Read(type_id);

    Game_Manager::toggle_pause();
}

void Network_Client::receive_sound () {
    RakNet::BitStream bitstream(Network_Engine::packet->data, Network_Engine::packet->length, false);

    Network_Engine::stat_counter_bytes_received += bitstream.GetNumberOfBytesUsed();

    RakNet::MessageID type_id;

    bitstream.Read(type_id);

    RakNet::RakString rstring;

    bitstream.ReadCompressed(rstring);

    Sound_Manager::play_sound(rstring.C_String());
}

void Network_Client::receive_server_ready () {
    RakNet::BitStream bitstream(Network_Engine::packet->data, Network_Engine::packet->length, false);

    Network_Engine::stat_counter_bytes_received += bitstream.GetNumberOfBytesUsed();

    RakNet::MessageID type_id;

    bitstream.Read(type_id);

    uint32_t server_completed_turn = 0;

    bitstream.ReadCompressed(server_completed_turn);
    Network_Lockstep::set_server_completed_turn(server_completed_turn);

    Network_Engine::read_server_ready(bitstream);
}

void Network_Client::send_client_ready () {
    if (Network_Engine::status == "client") {
        RakNet::BitStream bitstream;

        bitstream.Write((RakNet::MessageID) ID_GAME_CLIENT_READY);

        bitstream.WriteCompressed(Network_Lockstep::get_turn());

        Network_Engine::write_client_ready(bitstream);

        Network_Engine::stat_counter_bytes_sent += bitstream.GetNumberOfBytesUsed();
        Network_Engine::peer->Send(&bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, ORDERING_CHANNEL_TURNS,
                                   Network_Engine::server_id, false);
    }
}
