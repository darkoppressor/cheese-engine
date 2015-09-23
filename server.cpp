/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "server.h"
#include "strings.h"

using namespace std;

Server::Server(){
    name="";
    address="";
    port=0;
    password="";

    password_required=false;
    slots_filled=0;
    slots_total=0;
    version="";
    ping=0;
}

bool Server::matches(string get_address,unsigned short get_port){
    return address==get_address && port==get_port;
}

string Server::get_button_text(){
    string text=name+"\n";

    text+=address+"|"+Strings::num_to_string(port)+"\n";

    //Note that this is actually the connected clients, including non-players (although those are not implemented yet)
    text+="Players: "+Strings::num_to_string(slots_filled)+"/"+Strings::num_to_string(slots_total);
    text+=" Ping: "+Strings::num_to_string(ping)+"\n";

    if(password_required){
        text+="Password: Yes\n";
    }
    else{
        text+="Password: No\n";
    }

    text+="Game Version: "+version;

    return text;
}
