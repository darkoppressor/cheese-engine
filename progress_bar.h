/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef progress_bar_h
#define progress_bar_h

#include "timer.h"

#include <string>
#include <stdint.h>

class Progress_Bar{
private:

    int items;
    int items_done;

    //The time that the previous step was completed
    uint32_t time_of_previous_step;

    Timer timer;

    bool is_done() const;

    void finish();

public:

    Progress_Bar(int get_items);

    void progress(std::string message="",int items_completed=1);

    double get_percentage_done() const;

    //in milliseconds
    uint32_t get_time_elapsed() const;
};

#endif
