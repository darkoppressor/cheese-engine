/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "progress_bar.h"
#include "log.h"
#include "engine_strings.h"
#include "game_manager.h"
#include "engine_math.h"

using namespace std;

bool Progress_Bar::is_done() const{
    return items_done==items;
}

void Progress_Bar::finish(){
    items_done=items;

    timer.stop();
}

Progress_Bar::Progress_Bar(int get_items){
    items=get_items;

    if(items<=0){
        items=1;
    }

    items_done=0;

    time_of_previous_step=0;

    timer.start();
}

void Progress_Bar::progress(string message){
    if(!is_done()){
        items_done++;

        uint32_t ticks=get_time_elapsed();

        uint32_t time_elapsed_this_step=Math::abs(ticks,time_of_previous_step);

        string time_string="<1";
        if(time_elapsed_this_step>0){
            time_string=Strings::num_to_string(time_elapsed_this_step);
        }

        Log::add_log("Done in "+time_string+" ms\n"+message);

        Game_Manager::render_loading_screen(*this,message);

        time_of_previous_step=ticks;

        if(items_done>=items){
            finish();
        }
    }
}

double Progress_Bar::get_percentage_done() const{
    return (double)items_done/(double)items;
}

uint32_t Progress_Bar::get_time_elapsed() const{
    if(timer.is_started()){
        return timer.get_ticks();
    }
    else{
        return time_of_previous_step;
    }
}
