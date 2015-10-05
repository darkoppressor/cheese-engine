/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "network_lockstep.h"
#include "network_engine.h"
#include "network_server.h"
#include "network_client.h"
#include "engine_data.h"
#include "game_manager.h"

using namespace std;

const uint32_t Network_Lockstep::TURN_MIN=3;
const uint32_t Network_Lockstep::TURN_MAX=50;
const uint32_t Network_Lockstep::TURN_UPDATES=6;

Timer Network_Lockstep::turn_timer;

uint32_t Network_Lockstep::turn=TURN_MIN;

uint32_t Network_Lockstep::server_turn_complete=0;

uint32_t Network_Lockstep::logic_updates_this_turn=0;

uint32_t Network_Lockstep::current_turn_limit=200;

void Network_Lockstep::reset(){
    if(Engine_Data::network_lockstep){
        turn_timer.stop();

        turn=TURN_MIN;

        server_turn_complete=0;

        logic_updates_this_turn=0;

        current_turn_limit=200;
    }
}

void Network_Lockstep::start(){
    if(Engine_Data::network_lockstep){
        turn_timer.start();
    }
}

bool Network_Lockstep::logic_update_allowed(){
    if(!Engine_Data::network_lockstep || logic_updates_this_turn<TURN_UPDATES){
        return true;
    }
    else{
        return false;
    }
}

void Network_Lockstep::do_logic_update(){
    if(Engine_Data::network_lockstep){
        logic_updates_this_turn++;
    }
}

uint32_t Network_Lockstep::get_turn(){
    return turn;
}

uint32_t Network_Lockstep::get_server_turn_complete(){
    return server_turn_complete;
}

void Network_Lockstep::advance_turn_timer(){
    if(Engine_Data::network_lockstep){
        if(Network_Engine::status=="server" && !Game_Manager::paused){
            //If we have waited long enough for this turn, and all logic updates for this turn are done
            if(turn_timer.is_started() && turn_timer.get_ticks()>=current_turn_limit && logic_updates_this_turn==TURN_UPDATES && server_turn_complete!=turn){
                turn_timer.stop();

                //Set our own client to ready
                Network_Engine::clients[0].completed_turn=turn;
            }

            //After stopping the clock above
            if(!turn_timer.is_started() && server_turn_complete!=turn){
                //The number of clients ready to move on
                size_t clients_ready=0;

                for(size_t i=0;i<Network_Engine::clients.size();i++){
                    //If this client has completed the current turn
                    if(Network_Engine::clients[i].completed_turn==turn){
                        clients_ready++;
                    }
                }

                //If all clients are ready
                if(Network_Engine::clients.size()==clients_ready){
                    //This turn is now the most recently completed turn on the server
                    server_turn_complete=turn;

                    //Inform all of the clients that they may proceed to the next turn
                    Network_Server::send_server_ready();
                }
            }

            //If we have completed the current turn, we are ready to begin the next turn
            if(server_turn_complete==turn){
                //Increment the turn counter
                if(turn<TURN_MAX){
                    turn++;
                }
                else{
                    turn=TURN_MIN;
                }

                //Reset the logic updates counter
                logic_updates_this_turn=0;

                //Restart the turn timer
                turn_timer.start();
            }
        }
        else if(Network_Engine::status=="client"){
            //If we are done processing the current turn, but we haven't received authorization from the server to move on to the next turn
            if(turn_timer.is_started() && turn_timer.get_ticks()>=current_turn_limit && logic_updates_this_turn==TURN_UPDATES && server_turn_complete!=turn){
                turn_timer.stop();

                Network_Client::send_client_ready();
            }

            //If we have completed the server's most recently completed turn, we are ready to begin the next turn
            if(server_turn_complete==turn){
                //Increment the turn counter
                if(turn<TURN_MAX){
                    turn++;
                }
                else{
                    turn=TURN_MIN;
                }

                //Reset the logic updates counter
                logic_updates_this_turn=0;

                //Restart the turn timer
                turn_timer.start();
            }
        }
    }
}
