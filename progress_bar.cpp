/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "progress_bar.h"

using namespace std;

Progress_Step::Progress_Step(uint32_t get_time_taken,string get_message){
    time_taken=get_time_taken;
    message=get_message;
}

bool Progress_Bar::is_done() const{
    return items_done==items;
}

void Progress_Bar::finish(){
    items_done=items;

    timer.stop();
}

Progress_Bar::Progress_Bar(int get_items){
    items=get_items

    if(items<=0){
        items=1;
    }

    items_done=0;

    time_of_previous_step=0;

    timer.start();
}

void Progress_Bar::progress(string message,int items_completed){
    if(!is_done()){
        items_done+=items_completed;

        if(items_done>=items){
            finish();
        }

        steps.push_back(Progress_Step(get_time_elapsed()-time_of_previous_step,message));
        time_of_previous_step=get_time_elapsed();

        Game_Manager::render_loading_screen(*this,message);
    }
}

double Progress_Bar::get_percentage_done() const{
    return (double)items_done/(double)items;
}

uint32_t Progress_Bar::get_time_elapsed() const{
    return timer.get_ticks();
}
