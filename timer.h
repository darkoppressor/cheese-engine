/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef timer_h
#define timer_h

#include <cstdint>

class Timer{
private:

    //The clock time when the timer started
    std::uint32_t start_ticks;

    //The ticks stored when the timer was paused
    std::uint32_t paused_ticks;

    //Timer status
    bool paused;
    bool started;

public:

    Timer();

    void start();
    void stop();
    void pause();
    void unpause();

    bool is_started() const;
    bool is_paused() const;

    std::uint32_t get_ticks() const;
};

#endif
