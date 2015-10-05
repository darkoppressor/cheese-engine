/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef network_message_identifiers_h
#define network_message_identifiers_h

#include "raknet/Source/MessageIdentifiers.h"

enum{
    ID_GAME_VERSION=ID_USER_PACKET_ENUM,
    ID_GAME_CLIENT_DATA,
    ID_GAME_INITIAL_GAME_DATA,
    ID_GAME_CONNECTED,
    ID_GAME_CLIENT_LIST,
    ID_GAME_CHAT_MESSAGE,
    ID_GAME_UPDATE,
    ID_GAME_INPUT,
    ID_GAME_PING_LIST,
    ID_GAME_PAUSED,
    ID_GAME_SOUND,
    ID_GAME_SERVER_READY,
    ID_GAME_CLIENT_READY,
    ID_GAME_PACKET_ENUM
};

enum{
    ORDERING_CHANNEL_UNORDERED,
    ORDERING_CHANNEL_CONNECTION,
    ORDERING_CHANNEL_CLIENT_DATA,
    ORDERING_CHANNEL_CHAT,
    ORDERING_CHANNEL_CLIENT_LIST,
    ORDERING_CHANNEL_INPUT,
    ORDERING_CHANNEL_PAUSED,
    ORDERING_CHANNEL_SOUND,
    ORDERING_CHANNEL_TURNS,
    ORDERING_CHANNEL_GAME_PACKET_ENUM
};

#endif
