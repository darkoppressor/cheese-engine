/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "engine_data.h"

using namespace std;

int Engine_Data::logical_screen_width=0;
int Engine_Data::logical_screen_height=0;

string Engine_Data::resolution_mode="";

double Engine_Data::sound_falloff=0.0;

bool Engine_Data::controller_text_entry_small=false;

bool Engine_Data::cursor_render_always=false;

string Engine_Data::cursor="";
string Engine_Data::cursor_mouse_over="";
string Engine_Data::color_theme="";
string Engine_Data::toast_font="";
string Engine_Data::default_font="";

int Engine_Data::toast_length_short=0;
int Engine_Data::toast_length_medium=0;
int Engine_Data::toast_length_long=0;

int Engine_Data::spaces_per_tab=0;

int Engine_Data::axis_scroll_rate=0;

int Engine_Data::scrolling_buttons_offset=0;

int Engine_Data::cursor_width=0;
int Engine_Data::cursor_height=0;

int Engine_Data::console_height=0;
int Engine_Data::chat_height=0;

double Engine_Data::window_border_thickness=0;
double Engine_Data::gui_border_thickness=0;

bool Engine_Data::drag_and_drop=false;

double Engine_Data::touch_finger_size=0.0;
bool Engine_Data::touch_controller_shoulders=false;
bool Engine_Data::touch_controller_guide=false;
bool Engine_Data::touch_controller_xy=false;

string Engine_Data::game_title="";
string Engine_Data::developer="";

int Engine_Data::controller_dead_zone=0;

vector<string> Engine_Data::starting_windows;
