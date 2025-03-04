/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "steam.h"
#include "engine_data.h"
#include "log.h"

#ifndef GAME_OS_ANDROID
    #include <steam_api.h>
#endif

using namespace std;

bool Steam::initialized = false;

bool Steam::initialize () {
    if (!initialized) {
        if (Engine_Data::steam) {
            #ifndef GAME_OS_ANDROID

                if (SteamAPI_RestartAppIfNecessary(Engine_Data::steam_app_id)) {
                    Log::add_error("Restarting game through Steam");

                    return false;
                }

                if (!SteamAPI_Init()) {
                    Log::add_error("Steam failed to initialize");

                    return false;
                }

            #else

                return false;
            #endif
        }

        initialized = true;
    }

    return true;
}

void Steam::deinitialize () {
    if (initialized) {
        initialized = false;

        if (Engine_Data::steam) {
            #ifndef GAME_OS_ANDROID
                SteamAPI_Shutdown();
            #endif
        }
    }
}
