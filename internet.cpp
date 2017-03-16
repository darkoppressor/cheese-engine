/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "internet.h"

#ifdef GAME_OS_WINDOWS
    #include <windows.h>
    #include <shellapi.h>
#endif

#ifdef GAME_OS_MACOS
    #include <ApplicationServices/ApplicationServices.h>
#endif

#ifdef GAME_OS_ANDROID
    #include "android.h"
#endif

using namespace std;

void Internet::open_url(string url){
    #ifdef GAME_OS_WINDOWS
        ShellExecute(NULL,"open",url.c_str(),NULL,NULL,SW_SHOWNORMAL);
    #endif

    #ifdef GAME_OS_LINUX
        string open_command="xdg-open "+url;
        system(open_command.c_str());
    #endif

    #ifdef GAME_OS_MACOS
        CFURLRef mac_url=CFURLCreateWithBytes(NULL,(uint8_t*)url.c_str(),url.length(),kCFStringEncodingASCII,NULL);
        LSOpenCFURLRef(mac_url,NULL);
        CFRelease(mac_url);
    #endif

    #ifdef GAME_OS_ANDROID
        Android::open_url(url);
    #endif
}
