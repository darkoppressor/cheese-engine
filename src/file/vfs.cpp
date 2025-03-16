/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "vfs.h"
#include "file_io.h"
#include "../options/options.h"
#include "directories.h"
#include "../strings/engine_strings.h"
#include "../log.h"
#include "../sorting.h"
#include "../engine.h"

#include <boost/algorithm/string.hpp>

#ifndef GAME_OS_ANDROID
    #include <contrib/minizip/unzip.h>
#endif

using namespace std;

const string VFS::PAK_FILE_EXTENSION = ".pak";
const size_t VFS::ZIP_MAX_FILENAME_LENGTH = 512;
const char VFS::ZIP_DIRECTORY_DELIMITER = '/';

VFS_Search_Path::VFS_Search_Path (string new_path, bool new_pak_file) {
    path = new_path;
    pak_file = new_pak_file;
}

bool VFS_Search_Path::operator<= (const VFS_Search_Path& other) const {
    return path <= other.path;
}

VFS_Full_Path::VFS_Full_Path (string new_pak_path, string new_path) {
    pak_path = new_pak_path;
    path = new_path;
}

Pak_File_Info::Pak_File_Info () {
    file_count = 0;
}

Pak_File_Info::Pak_File_Info (size_t new_file_count) {
    file_count = new_file_count;
}

VFS_RWops::VFS_RWops (uint8_t* new_buffer, SDL_RWops* new_rwops) {
    buffer = new_buffer;
    rwops = new_rwops;
}

void VFS_RWops::close_buffer () {
    if (buffer != 0) {
        delete[] buffer;

        buffer = 0;
    }
}

void VFS::clean_path (string& path) {
    // Convert backslashes to slashes
    Strings::slashes_to_slash(&path);

    // Remove any double slashes
    boost::algorithm::erase_all(path, "//");

    // Remove any leading slash
    if (boost::algorithm::starts_with(path, "/")) {
        boost::algorithm::erase_first(path, "/");
    }

    // Remove any ending slash
    if (boost::algorithm::ends_with(path, "/")) {
        boost::algorithm::erase_last(path, "/");
    }
}

void VFS::add_files_to_list (const VFS_Search_Path& prefix, const string& directory, set<string>& file_list) {
    if (File_IO::exists(prefix.path)) {
        if (!prefix.pak_file && File_IO::is_directory(prefix.path)) {
            string search_directory = prefix.path + directory;

            // Remove any ending slash
            if (boost::algorithm::ends_with(search_directory, "/")) {
                boost::algorithm::erase_last(search_directory, "/");
            }

            if (File_IO::exists(search_directory) && File_IO::is_directory(search_directory)) {
                for (File_IO_Directory_Iterator it(search_directory); it.evaluate(); it.iterate()) {
                    if (it.is_regular_file()) {
                        string file_name = it.get_file_name();

                        boost::algorithm::trim(file_name);

                        if (!boost::algorithm::ends_with(file_name, PAK_FILE_EXTENSION)) {
                            file_name = directory + "/" + file_name;

                            clean_path(file_name);

                            file_list.insert(file_name);
                        }
                    }
                }
            }
        } else if (prefix.pak_file && File_IO::is_regular_file(prefix.path)) {
            #ifndef GAME_OS_ANDROID
                unzFile file = unzOpen(prefix.path.c_str());

                if (file != 0) {
                    unz_global_info global_info;

                    if (unzGetGlobalInfo(file, &global_info) == UNZ_OK) {
                        for (size_t i = 0; i < global_info.number_entry; i++) {
                            unz_file_info file_info;
                            char filename[ZIP_MAX_FILENAME_LENGTH];

                            if (unzGetCurrentFileInfo(file, &file_info, filename, ZIP_MAX_FILENAME_LENGTH, 0, 0, 0,
                                                      0) == UNZ_OK) {
                                const size_t filename_length = strlen(filename);

                                // If the file is a regular file
                                if (filename[filename_length - 1] != ZIP_DIRECTORY_DELIMITER) {
                                    string file_name = filename;

                                    if (!boost::algorithm::ends_with(file_name, PAK_FILE_EXTENSION)) {
                                        clean_path(file_name);

                                        if ((directory.length() == 0 && !boost::algorithm::contains(file_name,
                                                                                                    "/")) ||
                                            (directory.length() > 0 && boost::algorithm::starts_with(file_name,
                                                                                                     directory +
                                                                                                     "/"))) {
                                            file_list.insert(file_name);
                                        }
                                    }
                                }
                            } else {
                                Log::add_error("Error reading file info in pak file: '" + prefix.path +
                                               "' for adding files to list");

                                break;
                            }

                            if (i + 1 < global_info.number_entry) {
                                if (unzGoToNextFile(file) != UNZ_OK) {
                                    Log::add_error("Error reading next file in pak file: '" + prefix.path +
                                                   "' for adding files to list");

                                    break;
                                }
                            }
                        }
                    } else {
                        Log::add_error("Error reading global info in pak file: '" + prefix.path +
                                       "' for adding files to list");
                    }

                    unzClose(file);
                } else {
                    Log::add_error("Error opening pak file: '" + prefix.path + "' for adding files to list");
                }

            #endif
        }
    }
}

void VFS::add_pak_files_to_search_paths (string directory, vector<VFS_Search_Path>& search_paths) {
    // Remove any ending slash
    if (boost::algorithm::ends_with(directory, "/")) {
        boost::algorithm::erase_last(directory, "/");
    }

    vector<VFS_Search_Path> search_paths_unsorted;

    if (File_IO::exists(directory) && File_IO::is_directory(directory)) {
        for (File_IO_Directory_Iterator it(directory); it.evaluate(); it.iterate()) {
            if (it.is_regular_file()) {
                string file_name = it.get_file_name();

                boost::algorithm::trim(file_name);

                if (boost::algorithm::ends_with(file_name, PAK_FILE_EXTENSION)) {
                    file_name = directory + "/" + file_name;

                    search_paths_unsorted.push_back(VFS_Search_Path(file_name, true));
                }
            }
        }
    }

    Sorting::quick_sort(search_paths_unsorted);

    for (const auto& search_path : search_paths_unsorted) {
        search_paths.push_back(search_path);
    }
}

VFS_Full_Path VFS::get_full_path (const string& path) {
    vector<VFS_Search_Path> search_paths = get_search_paths();

    for (const auto& search_path : search_paths) {
        if (!search_path.pak_file) {
            VFS_Full_Path full_path("", search_path.path + path);

            if (File_IO::exists(full_path.path) && File_IO::is_regular_file(full_path.path)) {
                return full_path;
            }
        } else {
            #ifndef GAME_OS_ANDROID
                unzFile file = unzOpen(search_path.path.c_str());

                if (file != 0) {
                    if (unzLocateFile(file, path.c_str(), 1) == UNZ_OK) {
                        unzClose(file);

                        return VFS_Full_Path(search_path.path, path);
                    }

                    unzClose(file);
                } else {
                    Log::add_error("Error opening pak file: '" + search_path.path + "' for retrieving file path");
                }

            #endif
        }
    }

    return VFS_Full_Path("", "");
}

Pak_File_Info VFS::get_pak_file_info (const string& path) {
    Pak_File_Info pak_file_info;

    #ifndef GAME_OS_ANDROID
        unzFile file = unzOpen(path.c_str());

        if (file != 0) {
            unz_global_info global_info;

            if (unzGetGlobalInfo(file, &global_info) == UNZ_OK) {
                pak_file_info.file_count = global_info.number_entry;
            } else {
                Log::add_error("Error reading global info in pak file: '" + path + "' for info");
            }

            unzClose(file);
        } else {
            Log::add_error("Error opening pak file: '" + path + "' for info");
        }

    #endif

    return pak_file_info;
}

vector<VFS_Search_Path> VFS::get_search_paths () {
    #ifdef GAME_OS_ANDROID
        vector<VFS_Search_Path> android_paths;

        android_paths.push_back(VFS_Search_Path("data/"));

        return android_paths;

    #endif

    vector<VFS_Search_Path> search_paths;

    if (Engine::current_mod.length() > 0) {
        // Mod data directory loose files
        search_paths.push_back(VFS_Search_Path(Directories::get_save_directory() + "mods/" + Engine::current_mod +
                                               "/"));

        // Mod data directory pak files
        add_pak_files_to_search_paths(Directories::get_save_directory() + "mods/" + Engine::current_mod, search_paths);
    }

    if (Options::save_location == "home") {
        // Home data directory loose files
        search_paths.push_back(VFS_Search_Path(Directories::get_save_directory() + "data/"));

        // Home data directory pak files
        add_pak_files_to_search_paths(Directories::get_save_directory() + "data", search_paths);
    }

    // Base data directory loose files
    search_paths.push_back(VFS_Search_Path("data/"));

    // Base data directory pak files
    add_pak_files_to_search_paths("data", search_paths);

    return search_paths;
}

vector<string> VFS::get_file_list (const string& directory) {
    set<string> file_list;
    vector<VFS_Search_Path> search_paths = get_search_paths();

    for (const auto& search_path : search_paths) {
        add_files_to_list(search_path, directory, file_list);
    }

    vector<string> sorted_file_list;

    for (const auto& file : file_list) {
        sorted_file_list.push_back(file);
    }

    Sorting::quick_sort(sorted_file_list);

    return sorted_file_list;
}

VFS_RWops VFS::get_rwops (string path, bool binary) {
    uint8_t* buffer = 0;
    SDL_RWops* rwops = 0;

    clean_path(path);

    // Do not allow '..'
    if (boost::algorithm::contains(path, "../") || boost::algorithm::ends_with(path, "/..") || path == "..") {
        Log::add_error("Error getting file: '" + path + "' contains '..'");
    } else {
        VFS_Full_Path full_path = get_full_path(path);

        if (full_path.pak_path.length() == 0) {
            if (full_path.path.length() > 0) {
                string rw_mode = "r";

                if (binary) {
                    rw_mode += "b";
                }

                rwops = SDL_RWFromFile(full_path.path.c_str(), rw_mode.c_str());
            } else {
                Log::add_error("Error getting file: '" + path + "' does not exist");
            }
        } else {
            #ifndef GAME_OS_ANDROID
                unzFile file = unzOpen(full_path.pak_path.c_str());

                if (file != 0) {
                    if (unzLocateFile(file, full_path.path.c_str(), 1) == UNZ_OK) {
                        unz_file_info file_info;
                        char filename[ZIP_MAX_FILENAME_LENGTH];

                        if (unzGetCurrentFileInfo(file, &file_info, filename, ZIP_MAX_FILENAME_LENGTH, 0, 0, 0,
                                                  0) == UNZ_OK) {
                            if (unzOpenCurrentFile(file) == UNZ_OK) {
                                buffer = new uint8_t[file_info.uncompressed_size];

                                if (unzReadCurrentFile(file, buffer,
                                                       file_info.uncompressed_size) == file_info.uncompressed_size) {
                                    rwops = SDL_RWFromConstMem(buffer, file_info.uncompressed_size);
                                } else {
                                    Log::add_error("Error reading file in pak file: '" + full_path.pak_path + "'");
                                }
                            } else {
                                Log::add_error("Error opening file in pak file: '" + full_path.pak_path + "'");
                            }
                        } else {
                            Log::add_error("Error reading file info in pak file: '" + full_path.pak_path +
                                           "' for loading file");
                        }
                    } else {
                        Log::add_error("Error locating file in pak file: '" + full_path.pak_path + "'");
                    }

                    unzClose(file);
                } else {
                    Log::add_error("Error opening pak file: '" + full_path.pak_path + "' for loading file");
                }

            #endif
        }
    }

    return VFS_RWops(buffer, rwops);
}
