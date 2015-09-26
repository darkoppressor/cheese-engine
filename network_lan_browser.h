/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef network_lan_browser_h
#define network_lan_browser_h

#include "server.h"

#include <vector>
#include <string>

class Network_LAN_Browser{
public:

    static std::vector<Server> lan_server_list;
    //Used to keep track of the LAN server we are connecting to when a password prompt is needed before connecting to the server
    static int lan_connecting_index;
    //Ditto, but for a server in the server list
    static int server_list_connecting_index;

    static void set_lan_server_target(int index,std::string get_password="");
    static void add_lan_server(std::string get_name,std::string get_address,unsigned short get_port,bool password_required,uint32_t slots_filled,uint32_t slots_total,std::string version,int ping);
    static Server* get_lan_server(int index);

    static void lan_refresh();
    static void lan_refresh_quick();

    static void receive_server_browser_info();
};

#endif
