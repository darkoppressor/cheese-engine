/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef vfs_h
#define vfs_h

#include <string>
#include <set>
#include <vector>
#include <cstdint>

#include <SDL.h>

class VFS_Search_Path {
    public:
        bool pak_file;

        // The path to the directory or pak file to be searched
        // If pak_file is true, path is the location of a pak file
        // If pak_file is false, path is the location of a directory
        std::string path;

        VFS_Search_Path (std::string new_path, bool new_pak_file = false);

        bool operator<= (const VFS_Search_Path& other) const;
};

class VFS_Full_Path {
    public:
        // The path to the pak file, if any
        std::string pak_path;

        // If pak_path is a non-empty string, path is relative to the pak_path-specified pak file's directory structure
        // If pak_path is an empty string, path is the path to the file within the regular file system
        std::string path;

        VFS_Full_Path (std::string new_pak_path, std::string new_path);
};

class Pak_File_Info {
    public:
        size_t file_count;

        Pak_File_Info ();
        Pak_File_Info (size_t new_file_count);
};

class VFS_RWops {
    public:
        std::uint8_t* buffer;
        SDL_RWops* rwops;

        VFS_RWops (std::uint8_t* new_buffer, SDL_RWops* new_rwops);

        void close_buffer();
};

class VFS {
    private:
        static void clean_path(std::string& path);
        static void add_files(const std::string& search_directory, const std::string& directory,
                              std::set<std::string>& file_list, bool recursive);
        static void add_files_to_list(const VFS_Search_Path& prefix, const std::string& directory,
                                      std::set<std::string>& file_list, bool recursive);
        static void add_pak_files_to_search_paths(std::string directory, std::vector<VFS_Search_Path>& search_paths);
        static VFS_Full_Path get_full_path(const std::string& path);

    public:
        static const std::string PAK_FILE_EXTENSION;
        static const size_t ZIP_MAX_FILENAME_LENGTH;
        static const char ZIP_DIRECTORY_DELIMITER;

        static Pak_File_Info get_pak_file_info(const std::string& path);

        // Returns a list of search paths, with the highest priority path being first
        static std::vector<VFS_Search_Path> get_search_paths();
        static std::vector<std::string> get_file_list(const std::string& directory);

        // Returns a VFS_RWops containing a SDL_RWops* for the file specified by path if it exists
        // The caller is responsible for freeing the buffer and SDL_RWops* contained in the VFS_RWops
        static VFS_RWops get_rwops(std::string path, bool binary = false);
};

#endif
