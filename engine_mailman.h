/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef engine_mailman_h
#define engine_mailman_h

#include <queue>
#include <string>

class Engine_Mailman{
private:

    static std::queue<std::string> mail_out;

public:

    static void send_letter(std::string letter);

    static bool has_mail();
    static std::string deliver_letter();
};

#endif
