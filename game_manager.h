/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef game_manager_h
#define game_manager_h

#include <rng.h>
#include <collision.h>

#include <string>
#include <vector>

class Game_Manager{
public:

    static bool display_scoreboard;
    //If true, a game is currently in progress
    //If false, a game is not in progress
    static bool in_progress;
    //If true, the game is paused
    //If false, the game is not paused
    static bool paused;

    //Current movement state of the camera
    static std::string cam_state;
    static double camera_delta_x;
    static double camera_delta_y;
    static double camera_speed;
    static double camera_zoom;

    static std::string current_music;

    //Holds a string representing each command that is currently in the on state, and is relevant for networking
    //This is updated each frame
    static std::vector<std::string> command_states;
    static Collision_Rect camera;
    static RNG rng;

    static void reset();
    static void reset_camera_dimensions();

    static std::string get_random_direction_cardinal();
    static std::string get_random_direction_cardinal_ordinal();

    //Returns true if the number of effects does not exceed the effect limit
    static bool effect_allowed();

    static void manage_music();

    static void toggle_pause();

    static void start();
    static void start_client();
    static void stop();

    static void center_camera(Collision_Rect box);
    static void center_camera(Collision_Circ circle);

    static void zoom_camera_in(Collision_Rect box);
    static void zoom_camera_in(Collision_Circ circle);

    static void zoom_camera_out(Collision_Rect box);
    static void zoom_camera_out(Collision_Circ circle);

    static void set_camera();

    static void render_scoreboard();

    static void prepare_for_input();

    static void handle_command_states_multiplayer();
    static void handle_game_commands_multiplayer();

    static void handle_input_states_gui();
    static void handle_input_states();
    static bool handle_game_command_gui(std::string command_name);
    static bool handle_game_command(std::string command_name);
    static bool handle_input_events_gui();
    static bool handle_input_events();
};

#endif
