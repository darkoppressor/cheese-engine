/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef android_h
#define android_h

#include <string>
#include <vector>
#include <cstdint>

#ifdef GAME_OS_ANDROID
    #include "log.h"

    #include <SDL.h>
    #include <jni.h>

    // Uncomment to enable the Google Play Games SDK
    //#include <gpg/gpg.h>
#endif

//This value should equal the highest numbered sensor type from the Android SENSOR_TYPE constants (which come from Android's Sensor class)
const int SENSOR_TYPE_COUNT=21;

//The highest number of values any sensor has
const int SENSOR_VALUES_MAX=6;

//The number of values we read from GPS
const int GPS_VALUES_MAX=6;

#ifdef GAME_OS_ANDROID
    extern "C"{
        extern void jni_initialize();
        extern int jni_get_sensor_number(const char* sensor_name);
        extern bool jni_get_sensor_available(const char* sensor_name);
        extern bool jni_get_sensor_enabled(const char* sensor_name);
        extern int jni_get_sensor_value_count_actual(const char* sensor_name);
        extern void jni_get_sensor_values(const char* sensor_name,float values[SENSOR_VALUES_MAX]);
        extern bool jni_get_gps_available();
        extern bool jni_get_gps_accessible();
        extern bool jni_get_gps_enabled();
        extern void jni_get_gps_values(double values[GPS_VALUES_MAX]);
    }
#endif

class Android_Sensor{
public:

    int value_count;
    std::string units;
	std::vector<std::string> value_labels;

    Android_Sensor();

    void setup(int get_value_count,std::string get_units,const std::vector<std::string>& get_value_labels);
    void reset();
};

class Android_GPS{
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

    Android_GPS();
};

#ifdef GAME_OS_ANDROID
    // Uncomment to enable the Google Play Games SDK
    /*class Android_Google_Play_Games{
    private:

        bool initialized;

        bool auth_in_progress;
        std::unique_ptr<gpg::GameServices> game_services;

        bool silent_sign_in_attempt_complete;
        bool signed_in;

    public:

        Android_Google_Play_Games();

        void reset();

        void initialize(gpg::PlatformConfiguration const& pc,gpg::GameServices::Builder::OnAuthActionStartedCallback started_callback,gpg::GameServices::Builder::OnAuthActionFinishedCallback finished_callback);
        void deinitialize();

        bool is_auth_in_progress();

        bool is_silent_sign_in_attempt_complete() const;
        bool is_signed_in() const;

        gpg::GameServices* get_game_services();
        void sign_in();
        void sign_out();
        ///void unlock_achievement(const char* achievement_id);
        void submit_highscore(const char* leaderboard_id,uint64_t score);
        ///void show_achievements();
        void show_leaderboard(const char* leaderboard_id);
        void show_all_leaderboards();
    };*/
#endif

class Android{
private:

    static bool initialized;

    static Android_Sensor sensors[SENSOR_TYPE_COUNT];

    #ifdef GAME_OS_ANDROID
        // Uncomment to enable the Google Play Games SDK
        //static Android_Google_Play_Games google_play_games;
    #endif

    template<typename... Args>
    static void call_android_method(std::string method_name,std::string signature,Args... args){
        #ifdef GAME_OS_ANDROID
            JNIEnv* env=(JNIEnv*)SDL_AndroidGetJNIEnv();

            if(env!=0){
                jobject activity=(jobject)SDL_AndroidGetActivity();

                if(activity!=NULL){
                    jclass clazz(env->GetObjectClass(activity));
                    jmethodID method_id=env->GetStaticMethodID(clazz,method_name.c_str(),signature.c_str());
                    env->CallStaticVoidMethod(clazz,method_id,args...);
                    env->DeleteLocalRef(activity);
                    env->DeleteLocalRef(clazz);
                }
                else{
                    Log::add_error("Error calling Android method '"+method_name+"': SDL_AndroidGetActivity returned NULL");
                }
            }
            else{
                Log::add_error("Error calling Android method '"+method_name+"': SDL_AndroidGetJNIEnv returned 0");
            }
        #endif
    }

    static void set_sensor_enabled(std::string sensor_type,bool enabled);

    static void set_gps_enabled(bool enabled,std::uint32_t minimum_update_time,float minimum_update_distance);

public:

    static const int SENSOR_TYPE_ACCELEROMETER;
    static const int SENSOR_TYPE_AMBIENT_TEMPERATURE;
    static const int SENSOR_TYPE_GAME_ROTATION_VECTOR;
    static const int SENSOR_TYPE_GEOMAGNETIC_ROTATION_VECTOR;
    static const int SENSOR_TYPE_GRAVITY;
    static const int SENSOR_TYPE_GYROSCOPE;
    static const int SENSOR_TYPE_GYROSCOPE_UNCALIBRATED;
    static const int SENSOR_TYPE_HEART_RATE;
    static const int SENSOR_TYPE_LIGHT;
    static const int SENSOR_TYPE_LINEAR_ACCELERATION;
    static const int SENSOR_TYPE_MAGNETIC_FIELD;
    static const int SENSOR_TYPE_MAGNETIC_FIELD_UNCALIBRATED;
    static const int SENSOR_TYPE_PRESSURE;
    static const int SENSOR_TYPE_PROXIMITY;
    static const int SENSOR_TYPE_RELATIVE_HUMIDITY;
    static const int SENSOR_TYPE_ROTATION_VECTOR;
    static const int SENSOR_TYPE_SIGNIFICANT_MOTION;
    static const int SENSOR_TYPE_STEP_COUNTER;
    static const int SENSOR_TYPE_STEP_DETECTOR;

    static void initialize();
    static void deinitialize();

    static bool get_sensor_availability(std::string sensor_type);
    static bool get_sensor_state(std::string sensor_type);
    static int get_sensor_value_count_actual(std::string sensor_type);
    static int get_sensor_value_count(std::string sensor_type);
    static std::string get_sensor_units(std::string sensor_type);
	static void get_sensor_value_labels(std::string sensor_type,std::string value_labels[SENSOR_VALUES_MAX]);
    static void get_sensor_values(std::string sensor_type,float values[SENSOR_VALUES_MAX]);
    static void enable_sensor(std::string sensor_type);
    static void disable_sensor(std::string sensor_type);

    static void vibrate(std::uint32_t length);
    static void vibrate_stop();

    static void open_url(std::string url);

    static bool get_gps_availability();
    static bool get_gps_accessibility();
    static bool get_gps_state();
    static Android_GPS get_gps_readout();
    static void enable_gps(std::uint32_t minimum_update_time,float minimum_update_distance);
    static void disable_gps();

    static bool gpg_is_silent_sign_in_attempt_complete();
    static bool gpg_is_signed_in();
    static void gpg_sign_in();
    static void gpg_sign_out();
    ///static void gpg_unlock_achievement(const char* achievement_id);
    static void gpg_submit_highscore(const char* leaderboard_id,uint64_t score);
    ///static void gpg_show_achievements();
    static void gpg_show_leaderboard(const char* leaderboard_id);
    static void gpg_show_all_leaderboards();
};

#endif
