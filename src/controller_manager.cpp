/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "controller_manager.h"
#include "options.h"
#include "engine_strings.h"
#include "log.h"
#include "android.h"

#include <boost/algorithm/string.hpp>

using namespace std;

bool Controller_Manager::initialized = false;
vector<Controller> Controller_Manager::controllers;
vector<SDL_Event> Controller_Manager::touch_controller_events;
Touch_Controller Controller_Manager::touch_controller;
const int Controller_Manager::CONTROLLER_ID_ALL = -1;
const int Controller_Manager::CONTROLLER_ID_TOUCH = -2;
bool Controller_Manager::touch_controls = false;

void Controller_Manager::remove_controllers () {
    for (size_t i = 0; i < controllers.size(); i++) {
        if (controllers[i].haptic != 0 && SDL_HapticOpened(SDL_HapticIndex(controllers[i].haptic))) {
            SDL_HapticClose(controllers[i].haptic);
        }

        SDL_GameControllerClose(controllers[i].controller);
    }

    controllers.clear();
}

void Controller_Manager::initialize () {
    if (!initialized) {
        for (int i = 0; i < SDL_NumJoysticks(); i++) {
            string msg = "";

            if (SDL_IsGameController(i)) {
                if (Options::accelerometer_controller || !boost::algorithm::icontains(SDL_JoystickNameForIndex(i),
                                                                                      "accelerometer")) {
                    controllers.push_back(Controller(SDL_GameControllerOpen(i)));

                    Controller* controller_object = &controllers.back();
                    SDL_GameController* controller = controller_object->controller;

                    if (controller != 0) {
                        controller_object->instance_id =
                            SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller));

                        if (SDL_JoystickIsHaptic(SDL_GameControllerGetJoystick(controller))) {
                            controller_object->haptic =
                                SDL_HapticOpenFromJoystick(SDL_GameControllerGetJoystick(controller));

                            if (controller_object->haptic != 0) {
                                if (SDL_HapticRumbleSupported(controller_object->haptic)) {
                                    if (SDL_HapticRumbleInit(controller_object->haptic) != 0) {
                                        msg = "Error initializing rumble for haptic on controller " +
                                              Strings::num_to_string(i) + ": ";
                                        msg += SDL_GetError();
                                        Log::add_error(msg);

                                        SDL_HapticClose(controller_object->haptic);
                                        controller_object->haptic = 0;
                                    }
                                }
                            } else {
                                msg = "Error opening haptic for controller " + Strings::num_to_string(i) + ": ";
                                msg += SDL_GetError();
                                Log::add_error(msg);
                            }
                        }
                    } else {
                        msg = "Error opening controller " + Strings::num_to_string(i) + ": ";
                        msg += SDL_GetError();
                        Log::add_error(msg);

                        controllers.pop_back();
                    }
                }
            } else {
                SDL_Joystick* joystick = 0;

                if ((joystick = SDL_JoystickOpen(i)) != 0) {
                    string joystick_name = SDL_JoystickName(joystick);
                    char ch_joystick_guid[64];

                    SDL_JoystickGetGUIDString(SDL_JoystickGetGUID(joystick), ch_joystick_guid, 64);

                    string joystick_guid = ch_joystick_guid;
                    string joystick_axes = Strings::num_to_string(SDL_JoystickNumAxes(joystick));
                    string joystick_balls = Strings::num_to_string(SDL_JoystickNumBalls(joystick));
                    string joystick_buttons = Strings::num_to_string(SDL_JoystickNumButtons(joystick));
                    string joystick_hats = Strings::num_to_string(SDL_JoystickNumHats(joystick));

                    SDL_JoystickClose(joystick);

                    msg = "Joystick detected, but not supported by the game controller interface:\n";
                    msg += "Name: " + joystick_name + "\n";
                    msg += "GUID: " + joystick_guid + "\n";
                    msg += "Number of Axis Controls: " + joystick_axes + "\n";
                    msg += "Number of Trackballs: " + joystick_balls + "\n";
                    msg += "Number of Buttons: " + joystick_buttons + "\n";
                    msg += "Number of POV Hats: " + joystick_hats + "\n";

                    Log::add_error(msg);
                } else {
                    Log::add_error("Joystick detected, but not supported by the game controller interface.");
                }
            }
        }

        initialized = true;
    }
}

void Controller_Manager::deinitialize () {
    if (initialized) {
        initialized = false;

        remove_controllers();
    }
}

void Controller_Manager::add_controller (const SDL_Event& event) {
    if (SDL_IsGameController(event.cdevice.which)) {
        controllers.push_back(Controller(SDL_GameControllerOpen(event.cdevice.which)));

        Controller* controller_object = &controllers.back();
        SDL_GameController* controller = controller_object->controller;

        if (controller != 0) {
            controller_object->instance_id = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller));

            if (SDL_JoystickIsHaptic(SDL_GameControllerGetJoystick(controller))) {
                controller_object->haptic = SDL_HapticOpenFromJoystick(SDL_GameControllerGetJoystick(controller));

                if (controller_object->haptic != 0) {
                    if (SDL_HapticRumbleSupported(controller_object->haptic)) {
                        if (SDL_HapticRumbleInit(controller_object->haptic) != 0) {
                            string msg = "Error initializing rumble for haptic on controller " +
                                         Strings::num_to_string(event.cdevice.which) + ": ";

                            msg += SDL_GetError();
                            Log::add_error(msg);

                            SDL_HapticClose(controller_object->haptic);
                            controller_object->haptic = 0;
                        }
                    }
                } else {
                    string msg = "Error opening haptic for controller " + Strings::num_to_string(event.cdevice.which) +
                                 ": ";

                    msg += SDL_GetError();
                    Log::add_error(msg);
                }
            }
        } else {
            string msg = "Error opening controller " + Strings::num_to_string(event.cdevice.which) + ": ";

            msg += SDL_GetError();
            Log::add_error(msg);

            controllers.pop_back();
        }
    } else {
        string joystick_name = SDL_JoystickNameForIndex(event.cdevice.which);

        Log::add_error("Joystick \"" + joystick_name +
                       "\" detected, but not supported by the game controller interface.");
    }
}

void Controller_Manager::remove_controller (const SDL_Event& event) {
    for (size_t i = 0; i < controllers.size(); i++) {
        if (event.cdevice.which == controllers[i].instance_id) {
            if (controllers[i].haptic != 0 && SDL_HapticOpened(SDL_HapticIndex(controllers[i].haptic))) {
                SDL_HapticClose(controllers[i].haptic);
            }

            SDL_GameControllerClose(controllers[i].controller);

            controllers.erase(controllers.begin() + i);

            break;
        }
    }
}

bool Controller_Manager::poll_event (SDL_Event* event) {
    if (touch_controller_events.size() > 0) {
        *event = touch_controller_events[0];

        touch_controller_events.erase(touch_controller_events.begin());

        return true;
    } else {
        return false;
    }
}

void Controller_Manager::finger_down (const SDL_Event& event) {
    vector<SDL_GameControllerButton> touch_buttons = touch_controller.check_for_button_press(event.tfinger.x,
                                                                                             event.tfinger.y);

    for (int i = 0; i < touch_buttons.size(); i++) {
        SDL_Event touch_controller_event;

        touch_controller_event.type = SDL_CONTROLLERBUTTONDOWN;
        touch_controller_event.common.type = touch_controller_event.type;
        touch_controller_event.common.timestamp = SDL_GetTicks();
        touch_controller_event.cbutton.type = touch_controller_event.type;
        touch_controller_event.cbutton.timestamp = touch_controller_event.common.timestamp;
        touch_controller_event.cbutton.which = CONTROLLER_ID_TOUCH;
        touch_controller_event.cbutton.button = touch_buttons[i];
        touch_controller_event.cbutton.state = SDL_PRESSED;

        touch_controller_events.push_back(touch_controller_event);
    }
}

void Controller_Manager::scale_touch_controller (int width, int height) {
    touch_controller.scale(width, height);
}

void Controller_Manager::render_touch_controller () {
    if (touch_controls) {
        touch_controller.render();
    }
}

void Controller_Manager::get_controller_info (string& msg) {
    if (controllers.size() > 0) {
        msg += "Controllers (" + Strings::num_to_string(controllers.size()) + "):\n\n";

        for (int i = 0; i < controllers.size(); i++) {
            if (SDL_GameControllerGetAttached(controllers[i].controller)) {
                bool haptic = false;

                if (controllers[i].haptic != 0 && SDL_HapticOpened(SDL_HapticIndex(controllers[i].haptic))) {
                    haptic = true;
                }

                msg += Strings::num_to_string(i) + ".\n";
                msg += "Controller Name: ";
                msg += SDL_GameControllerName(controllers[i].controller);
                msg += "\n";
                msg += "Joystick Name: ";
                msg += SDL_JoystickName(SDL_GameControllerGetJoystick(controllers[i].controller));
                msg += "\n";
                msg += "Instance ID: ";
                msg += Strings::num_to_string(controllers[i].instance_id);
                msg += "\n";
                msg += "Has Rumble: " + Strings::bool_to_string(haptic);
                msg += "\n\n";
            }
        }
    }
}

void Controller_Manager::make_rumble (int controller_number, float strength, uint32_t length) {
    if (Options::rumble) {
        if (controller_number == CONTROLLER_ID_ALL || controller_number == CONTROLLER_ID_TOUCH) {
            // Play the rumble on the device with the touch controller, if possible.
            Android::vibrate(length);
        }

        if (controller_number != CONTROLLER_ID_TOUCH) {
            for (int i = 0; i < controllers.size(); i++) {
                if (SDL_GameControllerGetAttached(controllers[i].controller) && controllers[i].haptic != 0 &&
                    SDL_HapticOpened(SDL_HapticIndex(controllers[i].haptic))) {
                    if (controller_number == CONTROLLER_ID_ALL || controller_number == i) {
                        SDL_HapticRumblePlay(controllers[i].haptic, strength, length);
                    }
                }
            }
        }
    }
}

void Controller_Manager::stop_rumble (int controller_number) {
    if (controller_number == CONTROLLER_ID_ALL || controller_number == CONTROLLER_ID_TOUCH) {
        Android::vibrate_stop();
    }

    if (controller_number != CONTROLLER_ID_TOUCH) {
        for (int i = 0; i < controllers.size(); i++) {
            if (SDL_GameControllerGetAttached(controllers[i].controller) && controllers[i].haptic != 0 &&
                SDL_HapticOpened(SDL_HapticIndex(controllers[i].haptic))) {
                if (controller_number == CONTROLLER_ID_ALL || controller_number == i) {
                    SDL_HapticRumbleStop(controllers[i].haptic);
                }
            }
        }
    }
}

bool Controller_Manager::controller_state (int controller_number, SDL_GameControllerButton button) {
    if (controller_number == CONTROLLER_ID_ALL || controller_number == CONTROLLER_ID_TOUCH) {
        if (touch_controls && touch_controller.check_button_state(button)) {
            return true;
        }
    }

    for (int i = 0; i < controllers.size(); i++) {
        if (SDL_GameControllerGetAttached(controllers[i].controller)) {
            if (controller_number == CONTROLLER_ID_ALL || controller_number == i) {
                if (SDL_GameControllerGetButton(controllers[i].controller, button)) {
                    return true;
                }
            }
        }
    }

    return false;
}

int Controller_Manager::controller_state (int controller_number, SDL_GameControllerAxis axis) {
    if (controller_number == CONTROLLER_ID_ALL || controller_number == CONTROLLER_ID_TOUCH) {
        // Check the touch controller for the passed axis state.
        // ... but the touch controller doesn't currently have any axes.
    }

    for (int i = 0; i < controllers.size(); i++) {
        if (SDL_GameControllerGetAttached(controllers[i].controller)) {
            if (controller_number == CONTROLLER_ID_ALL || controller_number == i) {
                int state = SDL_GameControllerGetAxis(controllers[i].controller, axis);

                if (state != 0) {
                    return state;
                }
            }
        }
    }

    return 0;
}
