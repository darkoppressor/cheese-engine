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
        static bool gps_available;
        static bool gps_accessible;
        static bool gps_enabled;
        static double gps_values[GPS_VALUES_MAX];

        /*void Java_org_libsdl_app_SDLActivity_nativeUpdateGPSAvailable (JNIEnv* env, jclass jcls, jboolean available)
              {
            gps_available = available;
           }

           void Java_org_libsdl_app_SDLActivity_nativeUpdateGPSAccessible (JNIEnv* env, jclass jcls, jboolean
              accessible) {
            gps_accessible = accessible;
           }

           void Java_org_libsdl_app_SDLActivity_nativeUpdateGPSEnabled (JNIEnv* env, jclass jcls, jboolean enabled) {
            gps_enabled = enabled;
           }

           void Java_org_libsdl_app_SDLActivity_nativeGPSUpdate (JNIEnv* env, jclass jcls, jfloat accuracy,
                                                              jdouble altitude, jfloat bearing, jdouble latitude,
                                                              jdouble longitude, jfloat speed) {
            gps_values[0] = accuracy;
            gps_values[1] = altitude;
            gps_values[2] = bearing;
            gps_values[3] = latitude;
            gps_values[4] = longitude;
            gps_values[5] = speed;
           }

           void jni_initialize () {
            gps_available = false;
            gps_accessible = false;
            gps_enabled = false;

            for (int i = 0; i < GPS_VALUES_MAX; i++) {
                gps_values[i] = 0.0;
            }
           }

           bool jni_get_gps_available () {
            return gps_available;
           }

           bool jni_get_gps_accessible () {
            return gps_accessible;
           }

           bool jni_get_gps_enabled () {
            return gps_enabled;
           }

           void jni_get_gps_values (double values[GPS_VALUES_MAX]) {
            for (int i = 0; i < GPS_VALUES_MAX; i++) {
                values[i] = gps_values[i];
            }
           }*/
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

#ifdef GAME_OS_ANDROID
    /*Android_Google_Play_Games::Android_Google_Play_Games () {
        initialized = false;

        reset();
       }

       void Android_Google_Play_Games::reset () {
        auth_in_progress = false;

        silent_sign_in_attempt_complete = false;
        signed_in = false;
       }

       void Android_Google_Play_Games::initialize (gpg::PlatformConfiguration const& pc,
                                                gpg::GameServices::Builder::OnAuthActionStartedCallback
                                                   started_callback,
                                                gpg::GameServices::Builder::OnAuthActionFinishedCallback
                                                   finished_callback)
       {
        if (!initialized) {
            Log::add_log("Google Play Games: Initializing services");

            if (!game_services) {
                Log::add_log("Google Play Games: Services uninitialized, so creating");

                game_services =
                    gpg::GameServices::Builder().SetOnLog(gpg::DEFAULT_ON_LOG,
                                                          gpg::LogLevel::VERBOSE).SetOnAuthActionStarted([this,
                                                                                                          started_callback]
                                                                                                             (
                                                                                                             gpg::
                                                                                                             AuthOperation
                                                                                                             op) {
                auth_in_progress = true;

                if (started_callback != nullptr) {
                    started_callback(op);
                }
            }).SetOnAuthActionFinished([this, finished_callback] (gpg::AuthOperation op, gpg::AuthStatus status) {
                string sign_state = (op == gpg::AuthOperation::SIGN_IN ? "in" : "out");
                Log::add_log("Google Play Games: Sign " + sign_state + " finished with a result of " +
                             Strings::num_to_string(status));

                auth_in_progress = false;

                silent_sign_in_attempt_complete = true;

                if (gpg::IsSuccess(status)) {
                    signed_in = true;
                } else {
                    signed_in = false;
                }

                if (finished_callback != nullptr) {
                    finished_callback(op, status);
                }

                if (op != gpg::AuthOperation::SIGN_IN) {
                    return;
                }

                Log::add_log("Google Play Games: Fetching all blocking");

                gpg::AchievementManager::FetchAllResponse fetch_response =
                    game_services->Achievements().FetchAllBlocking(std::chrono::milliseconds(
                                                                       1000));

                Log::add_log("Google Play Games: --------------------------------------------------------------");

                Log::add_log("Google Play Games: Fetching all nonblocking");

                game_services->Achievements().FetchAll(gpg::DataSource::CACHE_OR_NETWORK,
                                                       [] (gpg::AchievementManager::FetchAllResponse response) {});

                Log::add_log("Google Play Games: --------------------------------------------------------------");
            }).Create(pc);

                Log::add_log("Google Play Games: Created");
            }

            if (game_services) {
                Log::add_log("Google Play Games: Services initialized successfully");

                initialized = true;
            } else {
                Log::add_log("Google Play Games: Services failed to initialize");
            }
        }
       }

       void Android_Google_Play_Games::deinitialize () {
        if (initialized) {
            initialized = false;

            reset();

            if (game_services) {
                game_services.reset();
            }
        }
       }

       bool Android_Google_Play_Games::is_auth_in_progress () {
        return initialized && auth_in_progress;
       }

       bool Android_Google_Play_Games::is_silent_sign_in_attempt_complete () const {
        return initialized && silent_sign_in_attempt_complete;
       }

       bool Android_Google_Play_Games::is_signed_in () const {
        return initialized && signed_in;
       }

       gpg::GameServices* Android_Google_Play_Games::get_game_services () {
        if (initialized) {
            return game_services.get();
        } else {
            return 0;
        }
       }

       void Android_Google_Play_Games::sign_in () {
        if (initialized && !is_auth_in_progress() && !game_services->IsAuthorized()) {
            Log::add_log("Google Play Games: StartAuthorizationUI");

            game_services->StartAuthorizationUI();
        }
       }

       void Android_Google_Play_Games::sign_out () {
        if (initialized && !is_auth_in_progress() && game_services->IsAuthorized()) {
            Log::add_log("Google Play Games: SignOut");

            game_services->SignOut();
        }
       }

       void Android_Google_Play_Games::unlock_achievement (const char* achievement_id) {
        if (initialized && !is_auth_in_progress() && game_services->IsAuthorized()) {
            Log::add_log("Google Play Games: Achievement unlocked");

            game_services->Achievements().Unlock(achievement_id);
        }
       }

       void Android_Google_Play_Games::submit_highscore (const char* leaderboard_id, uint64_t score) {
        if (initialized && !is_auth_in_progress() && game_services->IsAuthorized()) {
            Log::add_log("Google Play Games: High score submitted");

            game_services->Leaderboards().SubmitScore(leaderboard_id, score);
        }
       }

       void Android_Google_Play_Games::show_achievements () {
        if (initialized && !is_auth_in_progress() && game_services->IsAuthorized()) {
            Log::add_log("Google Play Games: Showing achievements");

            game_services->Achievements().ShowAllUI();
        }
       }

       void Android_Google_Play_Games::show_leaderboard (const char* leaderboard_id) {
        if (initialized && !is_auth_in_progress() && game_services->IsAuthorized()) {
            Log::add_log("Google Play Games: Showing leaderboard");

            game_services->Leaderboards().ShowUI(leaderboard_id);
        }
       }

       void Android_Google_Play_Games::show_all_leaderboards () {
        if (initialized && !is_auth_in_progress() && game_services->IsAuthorized()) {
            Log::add_log("Google Play Games: Showing all leaderboards");

            game_services->Leaderboards().ShowAllUI();
        }
       }*/
#endif

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
Android_Sensor Android::sensors[SENSOR_TYPE_COUNT];

#ifdef GAME_OS_ANDROID
    // Android_Google_Play_Games Android::google_play_games;
#endif

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
    #ifdef GAME_OS_ANDROID
        // call_android_method_static("enableGPS", "(ZIF)V", enabled, minimum_update_time, minimum_update_distance);
    #endif
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

        #ifdef GAME_OS_ANDROID
            // jni_initialize();

            if (Engine_Data::android_gpg) {
                /*JNIEnv* env = (JNIEnv*) SDL_AndroidGetJNIEnv();

                   if (env != 0) {
                    JavaVM* jvm = 0;
                    int jvm_status = env->GetJavaVM(&jvm);

                    if (jvm != 0 && jvm_status == 0) {
                        gpg::AndroidInitialization::JNI_OnLoad(jvm);

                        jobject activity = (jobject) SDL_AndroidGetActivity();

                        if (activity != NULL) {
                            gpg::AndroidPlatformConfiguration platform_configuration;
                            platform_configuration.SetActivity(activity);

                            auto OnAuthActionStarted = [&] (gpg::AuthOperation op) {
                                                           Log::add_log("Google Play Games: OnAuthActionStarted");

                                                           switch (op) {
                                                               case gpg::AuthOperation::SIGN_IN:
                                                                   Log::add_log("Google Play Games: Signing in");
                                                                   break;
                                                               case gpg::AuthOperation::SIGN_OUT:
                                                                   Log::add_log("Google Play Games: Signing out");
                                                                   break;
                                                           }
                                                       };
                            auto OnAuthActionFinished = [&] (gpg::AuthOperation op, gpg::AuthStatus status) {
                                                            Log::add_log("Google Play Games: OnAuthActionFinished");

                                                            if (gpg::IsSuccess(status)) {
                                                                Log::add_log("Google Play Games: You are logged in");
                                                            } else {
                                                                Log::add_log("Google Play Games: You are not logged
                                                                   in");
                                                            }
                                                        };

                            google_play_games.initialize(platform_configuration, OnAuthActionStarted,
                                                         OnAuthActionFinished);

                            env->DeleteLocalRef(activity);
                        } else {
                            Log::add_error(
                                "Error getting Android platform configuration: SDL_AndroidGetActivity returned NULL");
                        }
                    } else {
                        Log::add_error("Error getting Android platform configuration: JavaVM status is " +
                           Strings::num_to_string(
                                           jvm_status));
                    }
                   } else {
                    Log::add_error("Error getting Android platform configuration: SDL_AndroidGetJNIEnv returned 0");
                   }*/
            }

        #endif

        initialized = true;
    }
}

void Android::deinitialize () {
    if (initialized) {
        initialized = false;

        #ifdef GAME_OS_ANDROID
            /*google_play_games.deinitialize();

               jni_initialize();*/
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
    // call_android_method_static("vibrate", "(I)V", (int) length);
}

void Android::vibrate_stop () {
    // call_android_method_static("vibrateStop", "()V");
}

void Android::open_url (string url) {
    #ifdef GAME_OS_ANDROID
        /*JNIEnv* env = (JNIEnv*) SDL_AndroidGetJNIEnv();

           if (env != 0) {
            jstring jstr = env->NewStringUTF(url.c_str());

            call_android_method_static("openUrl", "(Ljava/lang/String;)V", jstr);
           } else {
            Log::add_error("Error opening URL '" + url + "': SDL_AndroidGetJNIEnv returned 0");
           }*/

    #endif
}

bool Android::get_gps_availability () {
    #ifdef GAME_OS_ANDROID

        return false; // return jni_get_gps_available();
    #endif

    return false;
}

bool Android::get_gps_accessibility () {
    #ifdef GAME_OS_ANDROID

        return false; // return jni_get_gps_accessible();
    #endif

    return false;
}

bool Android::get_gps_state () {
    #ifdef GAME_OS_ANDROID

        return false; // return jni_get_gps_enabled();
    #endif

    return false;
}

Android_GPS Android::get_gps_readout () {
    Android_GPS gps;

    #ifdef GAME_OS_ANDROID
        /*double values[GPS_VALUES_MAX];

           jni_get_gps_values(values);

           gps.accuracy = values[0];
           gps.altitude = values[1];
           gps.bearing = values[2];
           gps.latitude = values[3];
           gps.longitude = values[4];
           gps.speed = values[5];*/
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

        return false; // return google_play_games.is_silent_sign_in_attempt_complete();
    #endif

    return false;
}

bool Android::gpg_is_signed_in () {
    #ifdef GAME_OS_ANDROID

        return false; // return google_play_games.is_signed_in();
    #endif

    return false;
}

void Android::gpg_sign_in () {
    #ifdef GAME_OS_ANDROID
        // google_play_games.sign_in();
    #endif
}

void Android::gpg_sign_out () {
    #ifdef GAME_OS_ANDROID
        // google_play_games.sign_out();
    #endif
}

void Android::gpg_unlock_achievement (const char* achievement_id) {
    #ifdef GAME_OS_ANDROID
        // google_play_games.unlock_achievement(achievement_id);
    #endif
}

void Android::gpg_submit_highscore (const char* leaderboard_id, uint64_t score) {
    #ifdef GAME_OS_ANDROID
        // google_play_games.submit_highscore(leaderboard_id, score);
    #endif
}

void Android::gpg_show_achievements () {
    #ifdef GAME_OS_ANDROID
        // google_play_games.show_achievements();
    #endif
}

void Android::gpg_show_leaderboard (const char* leaderboard_id) {
    #ifdef GAME_OS_ANDROID
        // google_play_games.show_leaderboard(leaderboard_id);
    #endif
}

void Android::gpg_show_all_leaderboards () {
    #ifdef GAME_OS_ANDROID
        // google_play_games.show_all_leaderboards();
    #endif
}
