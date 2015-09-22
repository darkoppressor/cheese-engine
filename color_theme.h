/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef color_theme_h
#define color_theme_h

#include <string>

class Color_Theme{
public:

    std::string name;

    std::string window_font;
	std::string window_title_bar;
	std::string window_background;
	std::string window_border;
	std::string button_font;
	std::string button_background;
	std::string button_background_moused;
	std::string button_background_click;
	std::string button_border;
	std::string tooltip_font;
	std::string tooltip_background;
	std::string tooltip_border;
	std::string toast_font;
	std::string toast_background;
	std::string toast_border;
	std::string information_font;
	std::string information_background;
	std::string information_border;
	std::string gui_selector_1;
	std::string gui_selector_2;

    Color_Theme();
};

#endif
