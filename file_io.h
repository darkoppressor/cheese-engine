/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef file_io_h
#define file_io_h

#include "vfs.h"

#include <string>
#include <sstream>

#include <SDL.h>

#ifdef GAME_OS_ANDROID
    #include <vector>
    #include <cstdint>
    #include <dirent.h>
    #include <sys/stat.h>
#else
    #include <boost/filesystem.hpp>
#endif

class File_IO_Load {
    private:
        bool binary;
        bool load_success;
        bool loaded_backup;
        std::string data;
        std::istringstream data_stream;

    public:
        File_IO_Load ();
        File_IO_Load (std::string path, bool path_is_backup = false, bool get_binary = false,
                      bool suppress_errors = false, bool no_backup = false);
        File_IO_Load (VFS_RWops rwops, bool get_binary = false);

        void open(std::string path, bool path_is_backup, bool get_binary, bool suppress_errors, bool no_backup);
        void open(VFS_RWops rwops, bool get_binary);
        void close();

        bool is_opened();
        bool is_backup();
        bool eof();

        void getline(std::string* line);
        std::string get_data();
};

class File_IO_Save {
    private:
        SDL_RWops* rwops;
        std::string path;

    public:
        File_IO_Save (std::string get_path, bool append = false, bool binary = false);

        void open(std::string get_path, bool append, bool binary);
        bool close();

        bool is_opened();

        bool write(const void* ptr, size_t data_size, size_t data_count);
};

class File_IO {
    private:
        static bool save_file(std::string path, std::string data, bool append, bool binary);

    public:
        static const std::string SAVE_SUFFIX_TEMPORARY;
        static const std::string SAVE_SUFFIX_BACKUP;

        // First saves to a temporary file, then renames that file to the final filename
        static bool save_atomic(std::string path, std::string data, bool backup = false, bool append = false,
                                bool binary = false);

        #ifdef GAME_OS_ANDROID
            static bool is_in_directory_list(std::string path);
        #endif

        static bool exists(std::string path);
        // suppress_errors is only used by the Android version of this function
        static bool is_directory(std::string path, bool suppress_errors = false);
        static bool is_regular_file(std::string path);
        static bool create_directory(std::string path);
        static bool rename_file(std::string old_path, std::string new_path);
        // This does NOT overwrite new_path if it already exists
        static bool copy_file(std::string old_path, std::string new_path);
        static bool remove_file(std::string path);
        static void remove_directory(std::string path);
        static std::string get_file_name(std::string path);

        #ifdef GAME_OS_ANDROID
            static bool external_storage_available();

        #endif
};

class File_IO_Directory_Iterator {
    private:
        #ifdef GAME_OS_ANDROID
            std::string directory;
            std::vector<std::string> asset_list;
            std::uint32_t entry;

        #else
            boost::filesystem::directory_iterator it;
        #endif

    public:
        File_IO_Directory_Iterator (std::string get_directory);

        bool evaluate();
        void iterate();
        bool is_directory();
        bool is_regular_file();
        std::string get_full_path();
        std::string get_file_name();
};

#ifdef GAME_OS_ANDROID
    class File_IO_Directory_Iterator_User_Data {
        private:
            std::string directory;
            DIR* dir;
            struct dirent* dir_entry;
            std::uint32_t entry;
            std::uint32_t entries;

        public:
            File_IO_Directory_Iterator_User_Data (std::string get_directory);
            ~File_IO_Directory_Iterator_User_Data ();

            bool evaluate();
            void iterate();
            bool exists();
            bool is_regular_file();
            bool is_directory();
            std::string get_full_path();
            std::string get_file_name();
    };
#endif

#endif
