/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "android.h"
#include "symbols.h"
#include "engine_strings.h"
#include "engine_data.h"
#include "object_manager.h"

using namespace std;

#ifdef GAME_OS_ANDROID
    extern "C" {
        static bool gps_available = false;
        static bool gps_accessible = false;
        static bool gps_enabled = false;
        static double gps_values[GPS_VALUES_MAX];
        static bool googlePlayServicesSilentSignInAttemptComplete = false;
        static bool googlePlayServicesSignedIn = false;

        JNIEXPORT void JNICALL Java_org_libsdl_app_SDLActivity_nativeUpdateGPSAvailable (JNIEnv* env, jclass jcls,
                                                                                         jboolean available) {
            gps_available = available;
        }

        JNIEXPORT void JNICALL Java_org_libsdl_app_SDLActivity_nativeUpdateGPSAccessible (JNIEnv* env, jclass jcls,
                                                                                          jboolean accessible) {
            gps_accessible = accessible;
        }

        JNIEXPORT void JNICALL Java_org_libsdl_app_SDLActivity_nativeUpdateGPSEnabled (JNIEnv* env, jclass jcls,
                                                                                       jboolean enabled) {
            gps_enabled = enabled;
        }

        JNIEXPORT void JNICALL Java_org_libsdl_app_SDLActivity_nativeGPSUpdate (JNIEnv* env, jclass jcls,
                                                                                jfloat accuracy, jdouble altitude,
                                                                                jfloat bearing, jdouble latitude,
                                                                                jdouble longitude, jfloat speed) {
            gps_values[0] = accuracy;
            gps_values[1] = altitude;
            gps_values[2] = bearing;
            gps_values[3] = latitude;
            gps_values[4] = longitude;
            gps_values[5] = speed;
        }

        JNIEXPORT void JNICALL Java_org_libsdl_app_SDLActivity_nativeGooglePlayServicesSilentSignInAttemptComplete (
            JNIEnv* env, jclass jcls, jboolean silentSignInAttemptComplete) {
            googlePlayServicesSilentSignInAttemptComplete = silentSignInAttemptComplete;
        }

        JNIEXPORT void JNICALL Java_org_libsdl_app_SDLActivity_nativeGooglePlayServicesSignedIn (JNIEnv* env,
                                                                                                 jclass jcls,
                                                                                                 jboolean signedIn) {
            googlePlayServicesSignedIn = signedIn;
        }

        void jni_reset () {
            gps_available = false;
            gps_accessible = false;
            gps_enabled = false;

            for (int i = 0; i < GPS_VALUES_MAX; i++) {
                gps_values[i] = 0.0;
            }

            googlePlayServicesSilentSignInAttemptComplete = false;
            googlePlayServicesSignedIn = false;
        }

        bool get_gps_available () {
            return gps_available;
        }

        bool get_gps_accessible () {
            return gps_accessible;
        }

        bool get_gps_enabled () {
            return gps_enabled;
        }

        void get_gps_values (double values[GPS_VALUES_MAX]) {
            for (int i = 0; i < GPS_VALUES_MAX; i++) {
                values[i] = gps_values[i];
            }
        }

        bool getGooglePlayServicesSilentSignInAttemptComplete () {
            return googlePlayServicesSilentSignInAttemptComplete;
        }

        bool getGooglePlayServicesSignedIn () {
            return googlePlayServicesSignedIn;
        }
    }
#endif

Android_Sensor::Android_Sensor () {
    value_count = 0;
    units = "";
    enabled = false;
    sdlSensor = 0;
}

void Android_Sensor::setup (int get_value_count, string get_units, const vector<string>& get_value_labels) {
    value_count = get_value_count;
    units = get_units;

    value_labels = get_value_labels;

    while (value_labels.size() < SENSOR_VALUES_MAX) {
        value_labels.push_back("");
    }
}

void Android_Sensor::reset () {
    value_count = 0;
    units = "";
    enabled = false;
    sdlSensor = 0;

    value_labels.clear();
}

const string Android_GPS::UNITS_ACCURACY = "m";
const string Android_GPS::UNITS_ALTITUDE = "m";
const string Android_GPS::UNITS_BEARING = Symbols::degrees();
const string Android_GPS::UNITS_LATITUDE = Symbols::degrees();
const string Android_GPS::UNITS_LONGITUDE = Symbols::degrees();
const string Android_GPS::UNITS_SPEED = "m/s";

Android_GPS::Android_GPS () {
    accuracy = 0.0;
    altitude = 0.0;
    bearing = 0.0;
    latitude = 0.0;
    longitude = 0.0;
    speed = 0.0;
}

const int Android::SENSOR_TYPE_ACCELEROMETER = 1;
const int Android::SENSOR_TYPE_ACCELEROMETER_UNCALIBRATED = 35;
const int Android::SENSOR_TYPE_AMBIENT_TEMPERATURE = 13;
const int Android::SENSOR_TYPE_GAME_ROTATION_VECTOR = 15;
const int Android::SENSOR_TYPE_GEOMAGNETIC_ROTATION_VECTOR = 20;
const int Android::SENSOR_TYPE_GRAVITY = 9;
const int Android::SENSOR_TYPE_GYROSCOPE = 4;
const int Android::SENSOR_TYPE_GYROSCOPE_UNCALIBRATED = 16;
const int Android::SENSOR_TYPE_HEART_BEAT = 31;
const int Android::SENSOR_TYPE_HEART_RATE = 21;
const int Android::SENSOR_TYPE_LIGHT = 5;
const int Android::SENSOR_TYPE_LINEAR_ACCELERATION = 10;
const int Android::SENSOR_TYPE_LOW_LATENCY_OFFBODY_DETECT = 34;
const int Android::SENSOR_TYPE_MAGNETIC_FIELD = 2;
const int Android::SENSOR_TYPE_MAGNETIC_FIELD_UNCALIBRATED = 14;
const int Android::SENSOR_TYPE_MOTION_DETECT = 30;
const int Android::SENSOR_TYPE_POSE_6DOF = 28;
const int Android::SENSOR_TYPE_PRESSURE = 6;
const int Android::SENSOR_TYPE_PROXIMITY = 8;
const int Android::SENSOR_TYPE_RELATIVE_HUMIDITY = 12;
const int Android::SENSOR_TYPE_ROTATION_VECTOR = 11;
const int Android::SENSOR_TYPE_SIGNIFICANT_MOTION = 17;
const int Android::SENSOR_TYPE_STATIONARY_DETECT = 29;
const int Android::SENSOR_TYPE_STEP_COUNTER = 19;
const int Android::SENSOR_TYPE_STEP_DETECTOR = 18;
bool Android::initialized = false;
int Android::sdlSensorCount = 0;
map<int, int> Android::sdlSensors;
Android_Sensor Android::sensors[SENSOR_TYPE_COUNT];

int Android::getSensorNumber (string sensorName) {
    if (sensorName == "accelerometer") {
        return Android::SENSOR_TYPE_ACCELEROMETER;
    } else if (sensorName == "accelerometer_uncalibrated") {
        return Android::SENSOR_TYPE_ACCELEROMETER_UNCALIBRATED;
    } else if (sensorName == "ambient_temperature") {
        return Android::SENSOR_TYPE_AMBIENT_TEMPERATURE;
    } else if (sensorName == "game_rotation_vector") {
        return Android::SENSOR_TYPE_GAME_ROTATION_VECTOR;
    } else if (sensorName == "geomagnetic_rotation_vector") {
        return Android::SENSOR_TYPE_GEOMAGNETIC_ROTATION_VECTOR;
    } else if (sensorName == "gravity") {
        return Android::SENSOR_TYPE_GRAVITY;
    } else if (sensorName == "gyroscope") {
        return Android::SENSOR_TYPE_GYROSCOPE;
    } else if (sensorName == "gyroscope_uncalibrated") {
        return Android::SENSOR_TYPE_GYROSCOPE_UNCALIBRATED;
    } else if (sensorName == "heart_beat") {
        return Android::SENSOR_TYPE_HEART_BEAT;
    } else if (sensorName == "heart_rate") {
        return Android::SENSOR_TYPE_HEART_RATE;
    } else if (sensorName == "light") {
        return Android::SENSOR_TYPE_LIGHT;
    } else if (sensorName == "linear_acceleration") {
        return Android::SENSOR_TYPE_LINEAR_ACCELERATION;
    } else if (sensorName == "low_latency_off_body_detect") {
        return Android::SENSOR_TYPE_LOW_LATENCY_OFFBODY_DETECT;
    } else if (sensorName == "magnetic_field") {
        return Android::SENSOR_TYPE_MAGNETIC_FIELD;
    } else if (sensorName == "magnetic_field_uncalibrated") {
        return Android::SENSOR_TYPE_MAGNETIC_FIELD_UNCALIBRATED;
    } else if (sensorName == "motion_detect") {
        return Android::SENSOR_TYPE_MOTION_DETECT;
    } else if (sensorName == "pose_6dof") {
        return Android::SENSOR_TYPE_POSE_6DOF;
    } else if (sensorName == "pressure") {
        return Android::SENSOR_TYPE_PRESSURE;
    } else if (sensorName == "proximity") {
        return Android::SENSOR_TYPE_PROXIMITY;
    } else if (sensorName == "relative_humidity") {
        return Android::SENSOR_TYPE_RELATIVE_HUMIDITY;
    } else if (sensorName == "rotation_vector") {
        return Android::SENSOR_TYPE_ROTATION_VECTOR;
    } else if (sensorName == "significant_motion") {
        return Android::SENSOR_TYPE_SIGNIFICANT_MOTION;
    } else if (sensorName == "stationary_detect") {
        return Android::SENSOR_TYPE_STATIONARY_DETECT;
    } else if (sensorName == "step_counter") {
        return Android::SENSOR_TYPE_STEP_COUNTER;
    } else if (sensorName == "step_detector") {
        return Android::SENSOR_TYPE_STEP_DETECTOR;
    } else {
        return 0;
    }
}

bool Android::isSensorAvailable (string sensorType) {
    int sensorNumber = getSensorNumber(sensorType);

    if (sensorNumber > 0 && sensorNumber <= SENSOR_TYPE_COUNT) {
        return sdlSensors.count(sensorNumber);
    } else {
        return false;
    }
}

void Android::setSensorEnabled (string sensorType, bool enabled) {
    int sensorNumber = getSensorNumber(sensorType);

    if (sensorNumber > 0 && sensorNumber <= SENSOR_TYPE_COUNT) {
        if (isSensorAvailable(sensorType)) {
            if (!sensors[sensorNumber - 1].enabled) {
                int sdlDeviceIndex = sdlSensors.at(sensorNumber);
                SDL_Sensor* sdlSensor = SDL_SensorOpen(sdlDeviceIndex);

                if (sdlSensor != 0) {
                    sensors[sensorNumber - 1].enabled = true;
                    sensors[sensorNumber - 1].sdlSensor = sdlSensor;
                }
            } else {
                sensors[sensorNumber - 1].enabled = false;

                if (sensors[sensorNumber - 1].sdlSensor != 0) {
                    SDL_SensorClose(sensors[sensorNumber - 1].sdlSensor);
                    sensors[sensorNumber - 1].sdlSensor = 0;
                }
            }
        }
    }
}

void Android::set_gps_enabled (bool enabled, uint32_t minimum_update_time, float minimum_update_distance) {
    call_android_method_static("enableGPS", "(ZIF)V", enabled, minimum_update_time, minimum_update_distance);
}

void Android::initialize () {
    if (!initialized) {
        sdlSensorCount = SDL_NumSensors();
        sdlSensors.clear();

        for (int i = 0; i < sdlSensorCount; i++) {
            SDL_SensorType sdlSensorType = SDL_SensorGetDeviceType(i);

            if (sdlSensorType == SDL_SENSOR_ACCEL) {
                sdlSensors.emplace(Android::SENSOR_TYPE_ACCELEROMETER, i);
            } else if (sdlSensorType == SDL_SENSOR_GYRO) {
                sdlSensors.emplace(Android::SENSOR_TYPE_GYROSCOPE, i);
            } else {
                sdlSensors.emplace(SDL_SensorGetDeviceNonPortableType(i), i);
            }
        }

        vector<string> value_labels;

        value_labels.clear();
        value_labels.push_back("Acceleration on the x-axis");
        value_labels.push_back("Acceleration on the y-axis");
        value_labels.push_back("Acceleration on the z-axis");
        sensors[SENSOR_TYPE_ACCELEROMETER - 1].setup(3, "m/s" + Symbols::squared(), value_labels);

        value_labels.clear();
        value_labels.push_back("Acceleration on the x-axis without bias compensation");
        value_labels.push_back("Acceleration on the y-axis without bias compensation");
        value_labels.push_back("Acceleration on the z-axis without bias compensation");
        value_labels.push_back("Estimated x-axis bias");
        value_labels.push_back("Estimated y-axis bias");
        value_labels.push_back("Estimated z-axis bias");
        sensors[SENSOR_TYPE_ACCELEROMETER_UNCALIBRATED - 1].setup(6, "m/s" + Symbols::squared(), value_labels);

        value_labels.clear();
        value_labels.push_back("Ambient temperature");
        sensors[SENSOR_TYPE_AMBIENT_TEMPERATURE - 1].setup(1, Symbols::degrees() + "C", value_labels);

        value_labels.clear();
        value_labels.push_back("x*sin(theta/2)");
        value_labels.push_back("y*sin(theta/2)");
        value_labels.push_back("z*sin(theta/2)");
        value_labels.push_back("cos(theta/2)");
        sensors[SENSOR_TYPE_GAME_ROTATION_VECTOR - 1].setup(4, "", value_labels);

        value_labels.clear();
        value_labels.push_back("x*sin(theta/2)");
        value_labels.push_back("y*sin(theta/2)");
        value_labels.push_back("z*sin(theta/2)");
        value_labels.push_back("cos(theta/2)");
        value_labels.push_back("Estimated heading accuracy");
        sensors[SENSOR_TYPE_GEOMAGNETIC_ROTATION_VECTOR - 1].setup(5, "", value_labels);

        value_labels.clear();
        value_labels.push_back("Acceleration due to gravity on the x-axis");
        value_labels.push_back("Acceleration due to gravity on the y-axis");
        value_labels.push_back("Acceleration due to gravity on the z-axis");
        sensors[SENSOR_TYPE_GRAVITY - 1].setup(3, "m/s" + Symbols::squared(), value_labels);

        value_labels.clear();
        value_labels.push_back("Angular speed around the x-axis");
        value_labels.push_back("Angular speed around the y-axis");
        value_labels.push_back("Angular speed around the z-axis");
        sensors[SENSOR_TYPE_GYROSCOPE - 1].setup(3, "rad/s", value_labels);

        value_labels.clear();
        value_labels.push_back("Uncalibrated angular speed around the x-axis");
        value_labels.push_back("Uncalibrated angular speed around the y-axis");
        value_labels.push_back("Uncalibrated angular speed around the z-axis");
        value_labels.push_back("Estimated drift around the x-axis");
        value_labels.push_back("Estimated drift around the y-axis");
        value_labels.push_back("Estimated drift around the z-axis");
        sensors[SENSOR_TYPE_GYROSCOPE_UNCALIBRATED - 1].setup(6, "rad/s", value_labels);

        value_labels.clear();
        value_labels.push_back("Confidence");
        sensors[SENSOR_TYPE_HEART_BEAT - 1].setup(1, "", value_labels);

        value_labels.clear();
        value_labels.push_back("Heart rate");
        sensors[SENSOR_TYPE_HEART_RATE - 1].setup(1, "bpm", value_labels);

        value_labels.clear();
        value_labels.push_back("Ambient light level");
        sensors[SENSOR_TYPE_LIGHT - 1].setup(1, "lx", value_labels);

        value_labels.clear();
        value_labels.push_back("Acceleration without gravity on the x-axis");
        value_labels.push_back("Acceleration without gravity on the y-axis");
        value_labels.push_back("Acceleration without gravity on the z-axis");
        sensors[SENSOR_TYPE_LINEAR_ACCELERATION - 1].setup(3, "m/s" + Symbols::squared(), value_labels);

        value_labels.clear();
        value_labels.push_back("Off-body state");
        sensors[SENSOR_TYPE_LOW_LATENCY_OFFBODY_DETECT - 1].setup(1, "", value_labels);

        value_labels.clear();
        value_labels.push_back("Ambient magnetic field in the x-axis");
        value_labels.push_back("Ambient magnetic field in the y-axis");
        value_labels.push_back("Ambient magnetic field in the z-axis");
        sensors[SENSOR_TYPE_MAGNETIC_FIELD - 1].setup(3, "uT", value_labels);

        value_labels.clear();
        value_labels.push_back("Uncalibrated ambient magnetic field in the x-axis");
        value_labels.push_back("Uncalibrated ambient magnetic field in the y-axis");
        value_labels.push_back("Uncalibrated ambient magnetic field in the z-axis");
        value_labels.push_back("Estimated iron bias in the x-axis");
        value_labels.push_back("Estimated iron bias in the y-axis");
        value_labels.push_back("Estimated iron bias in the z-axis");
        sensors[SENSOR_TYPE_MAGNETIC_FIELD_UNCALIBRATED - 1].setup(6, "uT", value_labels);

        value_labels.clear();
        value_labels.push_back("Motion detected");
        sensors[SENSOR_TYPE_MOTION_DETECT - 1].setup(1, "", value_labels);

        value_labels.clear();
        value_labels.push_back("x*sin(θ/2)");
        value_labels.push_back("y*sin(θ/2)");
        value_labels.push_back("z*sin(θ/2)");
        value_labels.push_back("cos(θ/2)");
        value_labels.push_back("Translation along x-axis from an arbitrary origin");
        value_labels.push_back("Translation along y-axis from an arbitrary origin");
        value_labels.push_back("Translation along z-axis from an arbitrary origin");
        value_labels.push_back("Delta quaternion rotation x*sin(θ/2)");
        value_labels.push_back("Delta quaternion rotation y*sin(θ/2)");
        value_labels.push_back("Delta quaternion rotation z*sin(θ/2)");
        value_labels.push_back("Delta quaternion rotation cos(θ/2)");
        value_labels.push_back("Delta translation along x-axis");
        value_labels.push_back("Delta translation along y-axis");
        value_labels.push_back("Delta translation along z-axis");
        value_labels.push_back("Sequence number");
        sensors[SENSOR_TYPE_POSE_6DOF - 1].setup(15, "", value_labels);

        value_labels.clear();
        value_labels.push_back("Atmospheric pressure");
        sensors[SENSOR_TYPE_PRESSURE - 1].setup(1, "hPa", value_labels);

        value_labels.clear();
        value_labels.push_back("Proximity distance");
        sensors[SENSOR_TYPE_PROXIMITY - 1].setup(1, "cm", value_labels);

        value_labels.clear();
        value_labels.push_back("Relative ambient humidity");
        sensors[SENSOR_TYPE_RELATIVE_HUMIDITY - 1].setup(1, "%", value_labels);

        value_labels.clear();
        value_labels.push_back("x*sin(theta/2)");
        value_labels.push_back("y*sin(theta/2)");
        value_labels.push_back("z*sin(theta/2)");
        value_labels.push_back("cos(theta/2)");
        value_labels.push_back("Estimated heading accuracy");
        sensors[SENSOR_TYPE_ROTATION_VECTOR - 1].setup(5, "", value_labels);

        value_labels.clear();
        sensors[SENSOR_TYPE_SIGNIFICANT_MOTION - 1].setup(0, "", value_labels);

        value_labels.clear();
        value_labels.push_back("Device determined to be stationary");
        sensors[SENSOR_TYPE_STATIONARY_DETECT - 1].setup(1, "", value_labels);

        value_labels.clear();
        value_labels.push_back("Steps");
        sensors[SENSOR_TYPE_STEP_COUNTER - 1].setup(1, "", value_labels);

        value_labels.clear();
        value_labels.push_back("");
        sensors[SENSOR_TYPE_STEP_DETECTOR - 1].setup(1, "", value_labels);

        initialized = true;
    }
}

void Android::deinitialize () {
    if (initialized) {
        initialized = false;

        #ifdef GAME_OS_ANDROID
            jni_reset();
        #endif

        sdlSensorCount = 0;
        sdlSensors.clear();

        for (int i = 0; i < SENSOR_TYPE_COUNT; i++) {
            sensors[i].reset();
        }
    }
}

string Android::getSensorsString () {
    string sensorsString = "";

    for (int i = 0; i < sdlSensorCount; i++) {
        const char* sdlSensorName = SDL_SensorGetDeviceName(i);

        sensorsString += sdlSensorName;
        sensorsString += "\n";
    }

    return sensorsString;
}

bool Android::get_sensor_availability (string sensorType) {
    return isSensorAvailable(sensorType);
}

bool Android::get_sensor_state (string sensorType) {
    int sensorNumber = getSensorNumber(sensorType);

    if (sensorNumber > 0 && sensorNumber <= SENSOR_TYPE_COUNT) {
        if (isSensorAvailable(sensorType)) {
            return sensors[sensorNumber - 1].enabled;
        }
    }

    return false;
}

int Android::get_sensor_value_count (string sensorType) {
    int sensorNumber = getSensorNumber(sensorType);

    if (sensorNumber > 0 && sensorNumber <= SENSOR_TYPE_COUNT) {
        return sensors[sensorNumber - 1].value_count;
    }

    return 0;
}

string Android::get_sensor_units (string sensorType) {
    int sensorNumber = getSensorNumber(sensorType);

    if (sensorNumber > 0 && sensorNumber <= SENSOR_TYPE_COUNT) {
        return sensors[sensorNumber - 1].units;
    }

    return "";
}

void Android::get_sensor_value_labels (string sensorType, string valueLabels[SENSOR_VALUES_MAX]) {
    int sensorNumber = getSensorNumber(sensorType);

    if (sensorNumber > 0 && sensorNumber <= SENSOR_TYPE_COUNT) {
        for (int i = 0; i < SENSOR_VALUES_MAX; i++) {
            valueLabels[i] = sensors[sensorNumber - 1].value_labels[i];
        }
    }
}

bool Android::get_sensor_values (string sensorType, float values[SENSOR_VALUES_MAX]) {
    if (isSensorAvailable(sensorType) && get_sensor_state(sensorType)) {
        int sensorNumber = getSensorNumber(sensorType);

        if (SDL_SensorGetData(sensors[sensorNumber - 1].sdlSensor, values,
                              sensors[sensorNumber - 1].value_count) == 0) {
            return true;
        }
    }

    return false;
}

void Android::enable_sensor (string sensorType) {
    setSensorEnabled(sensorType, true);
}

void Android::disable_sensor (string sensorType) {
    setSensorEnabled(sensorType, false);
}

void Android::vibrate (uint32_t length) {
    call_android_method_static("vibrate", "(I)V", (int) length);
}

void Android::vibrate_stop () {
    call_android_method_static("vibrateStop", "()V");
}

void Android::open_url (string url) {
    #ifdef GAME_OS_ANDROID
        JNIEnv* env = (JNIEnv*) SDL_AndroidGetJNIEnv();

        if (env != 0) {
            jstring jstr = env->NewStringUTF(url.c_str());

            call_android_method_static("openUrl", "(Ljava/lang/String;)V", jstr);
        } else {
            Log::add_error("Error opening URL '" + url + "': SDL_AndroidGetJNIEnv returned 0");
        }

    #endif
}

bool Android::get_gps_availability () {
    #ifdef GAME_OS_ANDROID

        return get_gps_available();
    #endif

    return false;
}

bool Android::get_gps_accessibility () {
    #ifdef GAME_OS_ANDROID

        return get_gps_accessible();
    #endif

    return false;
}

bool Android::get_gps_state () {
    #ifdef GAME_OS_ANDROID

        return get_gps_enabled();
    #endif

    return false;
}

Android_GPS Android::get_gps_readout () {
    Android_GPS gps;

    #ifdef GAME_OS_ANDROID
        double values[GPS_VALUES_MAX];

        get_gps_values(values);

        gps.accuracy = values[0];
        gps.altitude = values[1];
        gps.bearing = values[2];
        gps.latitude = values[3];
        gps.longitude = values[4];
        gps.speed = values[5];
    #endif

    return gps;
}

void Android::enable_gps (uint32_t minimum_update_time, float minimum_update_distance) {
    set_gps_enabled(true, minimum_update_time, minimum_update_distance);
}

void Android::disable_gps () {
    set_gps_enabled(false, 0, 0.0f);
}

bool Android::gpg_is_silent_sign_in_attempt_complete () {
    #ifdef GAME_OS_ANDROID

        return getGooglePlayServicesSilentSignInAttemptComplete();
    #endif

    return false;
}

bool Android::gpg_is_signed_in () {
    #ifdef GAME_OS_ANDROID

        return getGooglePlayServicesSignedIn();
    #endif

    return false;
}

void Android::gpg_sign_in () {
    Log::add_log("Google Play Games: Signing in");
    call_android_method_static("signInToGooglePlayServices", "()V");
}

void Android::gpg_sign_out () {
    Log::add_log("Google Play Games: Signing out");
    call_android_method_static("signOutOfGooglePlayServices", "()V");
}

void Android::gpg_unlock_achievement (string achievement_id) {
    #ifdef GAME_OS_ANDROID
        Log::add_log("Google Play Games: Unlocked achievement: '" + achievement_id + "'");

        JNIEnv* env = (JNIEnv*) SDL_AndroidGetJNIEnv();

        if (env != 0) {
            jstring jstr = env->NewStringUTF(achievement_id.c_str());

            call_android_method_static("unlockAchievement", "(Ljava/lang/String;)V", jstr);
        } else {
            Log::add_error("Error unlocking achievement '" + achievement_id + "': SDL_AndroidGetJNIEnv returned 0");
        }

    #endif
}

void Android::gpg_submit_highscore (string leaderboard_id, uint64_t score) {
    #ifdef GAME_OS_ANDROID
        Log::add_log("Google Play Games: Submitting score: '" + Strings::num_to_string(score) + "' for leaderboard '" +
                     leaderboard_id + "'");

        JNIEnv* env = (JNIEnv*) SDL_AndroidGetJNIEnv();

        if (env != 0) {
            jstring jstr = env->NewStringUTF(leaderboard_id.c_str());

            call_android_method_static("submitScore", "(Ljava/lang/String;J)V", jstr);
        } else {
            Log::add_error("Error submitting score '" + Strings::num_to_string(score) + "' for leaderboard '" +
                           leaderboard_id + "': SDL_AndroidGetJNIEnv returned 0");
        }

    #endif
}

void Android::gpg_show_achievements () {
    Log::add_log("Google Play Games: Displaying achievements UI");
    call_android_method_static("showAchievements", "()V");
}

void Android::gpg_show_leaderboard (string leaderboard_id) {
    #ifdef GAME_OS_ANDROID
        Log::add_log("Google Play Games: Displaying leaderboard: '" + leaderboard_id + "'");

        JNIEnv* env = (JNIEnv*) SDL_AndroidGetJNIEnv();

        if (env != 0) {
            jstring jstr = env->NewStringUTF(leaderboard_id.c_str());

            call_android_method_static("showLeaderboard", "(Ljava/lang/String;J)V", jstr);
        } else {
            Log::add_error("Error displaying leaderboard '" + leaderboard_id + "': SDL_AndroidGetJNIEnv returned 0");
        }

    #endif
}

void Android::gpg_show_all_leaderboards () {
    Log::add_log("Google Play Games: Displaying all leaderboards");
    call_android_method_static("showAllLeaderboards", "()V");
}
