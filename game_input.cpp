/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game_manager.h"

using namespace std;

void Game_Manager::prepare_for_input(){
    if(in_progress){
        command_states.clear();

        display_scoreboard=false;
    }
}
