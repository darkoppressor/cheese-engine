/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "engine_mailman.h"

using namespace std;

queue<string> Engine_Mailman::mail_out;

void Engine_Mailman::send_letter(string letter){
    mail_out.push(letter);
}

bool Engine_Mailman::has_mail(){
    return !mail_out.empty();
}

string Engine_Mailman::deliver_letter(){
    if(has_mail()){
        string letter=mail_out.front();

        mail_out.pop();

        return letter;
    }
    else{
        return "";
    }
}
