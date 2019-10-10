/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef log_h
#define log_h

#include <vector>
#include <string>

class Log {
    private:
        static bool is_error_log_ready;

        // These holds log messages that are posted before certain parts of startup are completed
        static std::vector<std::string> world_load_errors;
        static std::vector<std::string> error_log_unready_errors;
        static std::vector<std::string> world_load_logs;
        static void add_world_load_error(std::string message);
        static void add_error_log_unready_error(std::string message);
        static void add_world_load_log(std::string message);

    public:
        static void post_world_load_errors();
        static void post_error_log_unready_errors();
        static void post_world_load_logs();
        static void clear_error_log();
        static void add_error(std::string message, bool allow_save = true);
        static void add_log(std::string message);

        // Returns a string with a time and/or date stamp
        // If filename is true, the returned timestamp is in a format appropriate for a filename
        static std::string get_timestamp(bool include_date = true, bool include_time = true, bool filename = false);
};

#endif
