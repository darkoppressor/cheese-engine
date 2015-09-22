/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "engine_mailman.h"

using namespace std;

void Engine_Mailman::send_letter(string letter){
    mail_out.push(letter);
}
