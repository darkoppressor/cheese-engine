/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef android_h
#define android_h

#include <string>
#include <vector>
#include <cstdint>
#include <map>

#include "log.h"

#include <SDL.h>
#include <SDL_sensor.h>

#ifdef GAME_OS_ANDROID
    #include <jni.h>
#endif

// This value should equal the highest numbered sensor type from the Android TYPE_ constants (which come from
// Android's Sensor class)
const int SENSOR_TYPE_COUNT = 35;

// The highest number of values any sensor has
const int SENSOR_VALUES_MAX = 15;

// The number of values we read from GPS
const int GPS_VALUES_MAX = 6;

#ifdef GAME_OS_ANDROID
    extern "C" {
        extern void jni_reset();
        extern bool get_gps_available();
        extern bool get_gps_accessible();
        extern bool get_gps_enabled();
        extern void get_gps_values(double values[GPS_VALUES_MAX]);
        extern bool getGooglePlayServicesSilentSignInAttemptComplete();
        extern bool getGooglePlayServicesSignedIn();
    }
#endif

class Android_Sensor {
    public:
        int value_count;
        std::string units;
        std::vector<std::string> value_labels;
        bool enabled;
        SDL_Sensor* sdlSensor;

        Android_Sensor ();

        void setup(int get_value_count, std::string get_units, const std::vector<std::string>& get_value_labels);
        void reset();
};

class Android_GPS {
    public:
        static const std::string UNITS_ACCURACY;
        static const std::string UNITS_ALTITUDE;
        static const std::string UNITS_BEARING;
        static const std::string UNITS_LATITUDE;
        static const std::string UNITS_LONGITUDE;
        static const std::string UNITS_SPEED;
        double accuracy;
        double altitude;
        double bearing;
        double latitude;
        double longitude;
        double speed;

        Android_GPS ();
};

class Android {
    private:
        static bool initialized;
        static int sdlSensorCount;
        // A map of SENSOR_TYPE_ constants paired to their corresponding SDL sensor device indices
        static std::map<int, int> sdlSensors;
        static Android_Sensor sensors[SENSOR_TYPE_COUNT];

        template<typename... Args>
        static void call_android_method (std::string method_name, std::string signature, Args... args) {
            #ifdef GAME_OS_ANDROID
                JNIEnv* env = (JNIEnv*) SDL_AndroidGetJNIEnv();

                if (env != 0) {
                    jobject activity = (jobject) SDL_AndroidGetActivity();

                    if (activity != NULL) {
                        jclass clazz(env->GetObjectClass(activity));
                        jmethodID method_id = env->GetMethodID(clazz, method_name.c_str(), signature.c_str());
                        env->CallVoidMethod(activity, method_id, args...);
                        env->DeleteLocalRef(activity);
                        env->DeleteLocalRef(clazz);
                    } else {
                        Log::add_error(
                            "Error calling Android method '" + method_name + "': SDL_AndroidGetActivity returned NULL");
                    }
                } else {
                    Log::add_error("Error calling Android method '" + method_name +
                                   "': SDL_AndroidGetJNIEnv returned 0");
                }

            #endif
        }

        template<typename... Args>
        static void call_android_method_static (std::string method_name, std::string signature, Args... args) {
            #ifdef GAME_OS_ANDROID
                JNIEnv* env = (JNIEnv*) SDL_AndroidGetJNIEnv();

                if (env != 0) {
                    jobject activity = (jobject) SDL_AndroidGetActivity();

                    if (activity != NULL) {
                        jclass clazz(env->GetObjectClass(activity));
                        jmethodID method_id = env->GetStaticMethodID(clazz, method_name.c_str(), signature.c_str());
                        env->CallStaticVoidMethod(clazz, method_id, args...);
                        env->DeleteLocalRef(activity);
                        env->DeleteLocalRef(clazz);
                    } else {
                        Log::add_error(
                            "Error calling Android method '" + method_name + "': SDL_AndroidGetActivity returned NULL");
                    }
                } else {
                    Log::add_error("Error calling Android method '" + method_name +
                                   "': SDL_AndroidGetJNIEnv returned 0");
                }

            #endif
        }

        static int getSensorNumber(std::string sensorName);
        static bool isSensorAvailable(std::string sensorType);
        static void setSensorEnabled(std::string sensorType, bool enabled);
        static void set_gps_enabled(bool enabled, std::uint32_t minimum_update_time, float minimum_update_distance);

    public:
        static const int SENSOR_TYPE_ACCELEROMETER;
        static const int SENSOR_TYPE_ACCELEROMETER_UNCALIBRATED;
        static const int SENSOR_TYPE_AMBIENT_TEMPERATURE;
        static const int SENSOR_TYPE_GAME_ROTATION_VECTOR;
        static const int SENSOR_TYPE_GEOMAGNETIC_ROTATION_VECTOR;
        static const int SENSOR_TYPE_GRAVITY;
        static const int SENSOR_TYPE_GYROSCOPE;
        static const int SENSOR_TYPE_GYROSCOPE_UNCALIBRATED;
        static const int SENSOR_TYPE_HEART_BEAT;
        static const int SENSOR_TYPE_HEART_RATE;
        static const int SENSOR_TYPE_LIGHT;
        static const int SENSOR_TYPE_LINEAR_ACCELERATION;
        static const int SENSOR_TYPE_LOW_LATENCY_OFFBODY_DETECT;
        static const int SENSOR_TYPE_MAGNETIC_FIELD;
        static const int SENSOR_TYPE_MAGNETIC_FIELD_UNCALIBRATED;
        static const int SENSOR_TYPE_MOTION_DETECT;
        static const int SENSOR_TYPE_POSE_6DOF;
        static const int SENSOR_TYPE_PRESSURE;
        static const int SENSOR_TYPE_PROXIMITY;
        static const int SENSOR_TYPE_RELATIVE_HUMIDITY;
        static const int SENSOR_TYPE_ROTATION_VECTOR;
        static const int SENSOR_TYPE_SIGNIFICANT_MOTION;
        static const int SENSOR_TYPE_STATIONARY_DETECT;
        static const int SENSOR_TYPE_STEP_COUNTER;
        static const int SENSOR_TYPE_STEP_DETECTOR;
        static void initialize();
        static void deinitialize();
        static std::string getSensorsString();
        static bool get_sensor_availability(std::string sensorType);
        static bool get_sensor_state(std::string sensorType);
        static int get_sensor_value_count(std::string sensorType);
        static std::string get_sensor_units(std::string sensorType);
        static void get_sensor_value_labels(std::string sensorType, std::string valueLabels[SENSOR_VALUES_MAX]);
        // Returns true if values were retrieved successfully
        static bool get_sensor_values(std::string sensorType, float values[SENSOR_VALUES_MAX]);
        static void enable_sensor(std::string sensorType);
        static void disable_sensor(std::string sensorType);
        static void vibrate(std::uint32_t length);
        static void vibrate_stop();
        static void open_url(std::string url);
        static bool get_gps_availability();
        static bool get_gps_accessibility();
        static bool get_gps_state();
        static Android_GPS get_gps_readout();
        static void enable_gps(std::uint32_t minimum_update_time, float minimum_update_distance);
        static void disable_gps();
        static bool gpg_is_silent_sign_in_attempt_complete();
        static bool gpg_is_signed_in();
        static void gpg_sign_in();
        static void gpg_sign_out();
        static void gpg_unlock_achievement(std::string achievement_id);
        static void gpg_submit_highscore(std::string leaderboard_id, uint64_t score);
        static void gpg_show_achievements();
        static void gpg_show_leaderboard(std::string leaderboard_id);
        static void gpg_show_all_leaderboards();
};

#endif
