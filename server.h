/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef server_h
#define server_h

#include <string>

class Server{
public:

    std::string name;
    std::string address;
    unsigned short port;
    std::string password;

    bool password_required;
    uint32_t slots_filled;
    uint32_t slots_total;
    std::string version;
    int ping;

    Server();

    bool matches(std::string get_address,unsigned short get_port);

    //Returns a text description of the server for use in menu buttons
    std::string get_button_text();
};

#endif
