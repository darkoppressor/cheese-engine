/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef server_h
#define server_h

#include <string>
#include <cstdint>

class Server {
    public:
        std::string name;
        std::string address;
        unsigned short port;
        std::string password;
        bool password_required;
        std::uint32_t slots_filled;
        std::uint32_t slots_total;
        std::string version;
        int ping;

        Server ();

        bool matches(std::string get_address, unsigned short get_port);

        // Returns a text description of the server for use in menu buttons
        std::string get_button_text();
};

#endif
