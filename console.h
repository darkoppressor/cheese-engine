/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef console_h
#define console_h

#include "information.h"
#include "timer.h"

#include <string>
#include <vector>

class Console{
public:

    //If true, this is a chat box
    //If false, it is the main console
    bool chat;

    Information info_display;
    Information info_input;

    int y;

    bool on;

    std::string last_tab_complete_command;

    //0 or below means instant
    int move_speed;

    int max_command_length;

    //The maximum lines to recall in the console log
    int max_log_recall;

    //The maximum number of commands to recall
    int max_command_recall;

    //The last command strings entered
    std::vector<std::string> recalled_command_strings;

    //The currently recalled string
    //Used for scrolling around in the recalled strings
    int current_recalled_command_string;

    std::string font;
    std::string font_color;

    double background_opacity;

    std::vector<std::string> commands;

    //The time it takes for a chat line to disappear, in milliseconds
    int line_timeout;

    //Used to determine if a chat message should be showed even if the chat window is closed
    std::vector<Timer> text_timers;

    Console();

    void setup(bool get_chat);

    void reset_current_recalled_command_string();

    void reset_text_timers();

    void clear_text();

    void toggle_on();

    void add_text(std::string text);

    void send_chat();

    void movement();

    void recall_up();
    void recall_down();
    void tab_complete();

    void handle_input_states();

    //Returns true if the event was consumed,
    //false otherwise
    bool handle_input_events();

    void animate();
    void render();

    void setup_commands();

    std::vector<std::string> parse_input(std::string str_input);

    //Pass an option to check for and an options string from parsed input data
    //Returns whether or not the option is in the options list
    bool input_has_option(std::string option,std::string options);

    void exec_file(std::string filename);

    void do_command();

    void run_commands(const std::vector<std::string>& command_list);

    //Returns true if the passed command was handled by the game
    bool handle_game_command(const std::string& command,const std::vector<std::string>& command_input);
};

#endif
