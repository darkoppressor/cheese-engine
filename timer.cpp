/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "timer.h"

#include <SDL.h>

using namespace std;

Timer::Timer(){
    start_ticks=0;
    paused_ticks=0;
    paused=false;
    started=false;
}

void Timer::start(){
    started=true;

    paused=false;

    start_ticks=SDL_GetTicks();
}

void Timer::stop(){
    started=false;

    paused=false;
}

uint32_t Timer::get_ticks(){
    if(started){
        if(paused){
            return paused_ticks;
        }
        else{
            return SDL_GetTicks()-start_ticks;
        }
    }

    return 0;
}

void Timer::pause(){
    if(started && !paused){
        paused=true;

        paused_ticks=SDL_GetTicks()-start_ticks;
    }
}

void Timer::unpause(){
    if(paused){
        paused=false;

        start_ticks=SDL_GetTicks()-paused_ticks;

        paused_ticks=0;
    }
}

bool Timer::is_started(){
    return started;
}

bool Timer::is_paused(){
    return paused;
}
