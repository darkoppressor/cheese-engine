/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "options.h"

using namespace std;

string Options::save_location="";

int Options::option_screen_width=0;
int Options::option_screen_height=0;
int Options::option_display_number=-1;
bool Options::option_fullscreen=false;
string Options::option_fullscreen_mode="desktop";

bool Options::option_vsync=false;
bool Options::option_accelerometer_controller=false;
bool Options::option_touch_controller_state=false;
double Options::option_touch_controller_opacity=0.0;
bool Options::font_shadows=false;
bool Options::option_screen_keyboard=false;
bool Options::option_hw_cursor=false;
bool Options::option_bind_cursor=false;

bool Options::option_fps=false;
bool Options::option_dev=false;

double Options::volume_global=1.0;
double Options::volume_sound=1.0;
double Options::volume_music=1.0;
bool Options::mute_global=false;
bool Options::mute_sound=false;
bool Options::mute_music=false;
