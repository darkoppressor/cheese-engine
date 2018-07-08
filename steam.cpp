/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "steam.h"
#include "engine_data.h"
#include "log.h"

#include <steam_api.h>

using namespace std;

bool Steam::initialize () {
    if (!initialized) {
        if (Engine_Data::steam) {
            if (SteamAPI_RestartAppIfNecessary(Engine_Data::steam_app_id)) {
                Log::add_error("Restarting game through Steam");

                return false;
            }

            if (!SteamAPI_Init()) {
                Log::add_error("Steam failed to initialize");

                return false;
            }
        }

        initialized = true;
    }

    return true;
}

void Steam::deinitialize () {
    if (initialized) {
        initialized = false;

        if (Engine_Data::steam) {
            SteamAPI_Shutdown();
        }
    }
}
