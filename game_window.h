/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef game_window_h
#define game_window_h

#include "coords.h"

#include <SDL.h>

class Game_Window{
public:

    Game_Window();

    void cleanup_video();

    bool set_resolution(int* desired_resolution_x,int* desired_resolution_y);

    Engine_Rect get_display_resolution();
    //Returns the resolution dimensions' maximum values for a display.
    //If display_number>=0, looks at display number display_number.
    //If display_number<0, looks at the display the window is currently on.
    Engine_Rect get_display_resolution_max(int display_number=-1);

    //Returns true if the position was adjusted based on the display_number option.
    //Returns false if the position was not adjusted.
    bool set_position(int* desired_resolution_x,int* desired_resolution_y,int* position_x,int* position_y);

    bool initialize_video();

    bool init_sdl();

    void set_sdl_hints();

    //Returns true if everything initialized properly.
    //Returns false if initializing failed.
    bool init();

    void screenshot();

    void update_display_number();

    void reinitialize();

    //The logical resolution of the game window.
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;

    SDL_Window* screen;

    SDL_Renderer* renderer;

    SDL_Surface* icon;
    Uint32 icon_colorkey;
};

#endif
