/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "options.h"

using namespace std;

string Options::save_location="";

string Options::version="";

int Options::screen_width=0;
int Options::screen_height=0;
int Options::display_number=-1;
bool Options::fullscreen=false;
string Options::fullscreen_mode="desktop";

bool Options::vsync=false;
bool Options::accelerometer_controller=false;
bool Options::touch_controller_state=false;
double Options::touch_controller_opacity=0.0;
bool Options::font_shadows=false;
bool Options::screen_keyboard=false;
bool Options::hw_cursor=false;
bool Options::bind_cursor=false;

bool Options::fps=false;
bool Options::dev=false;

double Options::volume_global=1.0;
double Options::volume_sound=1.0;
double Options::volume_music=1.0;
bool Options::mute_global=false;
bool Options::mute_sound=false;
bool Options::mute_music=false;

uint32_t effect_limit=0;
double camera_speed=0.0;
double camera_zoom=0.0;
string name="";
bool chat_timestamps=false;
uint32_t max_players=0;
bool screen_shake=false;
bool rumble=false;
bool screensaver=false;
