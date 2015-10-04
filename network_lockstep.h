/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef network_lockstep_h
#define network_lockstep_h

#include "timer.h"

#include <stdint.h>

class Network_Lockstep{
private:

    //The minimum network turn
    static const uint32_t TURN_MIN;
    //The maximum network turn
    static const uint32_t TURN_MAX;
    //The exact number of logic updates that must be completed each network turn
    static const uint32_t TURN_UPDATES;

    static Timer turn_timer;

    //The current network turn
    static uint32_t turn;

    //The turn most recently completed by the server
    static uint32_t server_turn_complete;

    //The number of logic updates done this turn
    static uint32_t logic_updates_this_turn;

    //The minimum amount of ms to wait between turns
    ///For now, this doesn't change
    static uint32_t current_turn_limit;

public:

    static void reset();

    static void start();

    static bool logic_update_allowed();

    static void do_logic_update();

    static uint32_t get_turn();

    static void advance_turn_timer();
};

#endif
