/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "file_io.h"
#include "log.h"

#ifdef GAME_OS_ANDROID
    #include "engine_strings.h"

    #include <errno.h>

    #include <boost/algorithm/string.hpp>
#endif

using namespace std;

const string File_IO::SAVE_SUFFIX_TEMPORARY="_TEMPORARY";
const string File_IO::SAVE_SUFFIX_BACKUP="_BACKUP";

File_IO_Load::File_IO_Load(){
    binary=false;
    load_success=false;
    loaded_backup=false;
    data="";
    data_stream.str("");
}

File_IO_Load::File_IO_Load(string path,bool path_is_backup,bool get_binary,bool suppress_errors){
    load_success=false;
    loaded_backup=false;
    data="";
    data_stream.str("");

    open(path,path_is_backup,get_binary,suppress_errors);
}

File_IO_Load::File_IO_Load(SDL_RWops* rwops,bool get_binary){
    load_success=false;
    loaded_backup=false;
    data="";
    data_stream.str("");

    open(rwops,get_binary);
}

void File_IO_Load::open(string path,bool path_is_backup,bool get_binary,bool suppress_errors){
    binary=get_binary;

    //Did this function call succeed?
    //This is here because this function is recursive,
    //and at the end, it loads data into data_stream,
    //so a failed call that recursively called itself again
    //should not touch data_stream at its end
    bool succeeded=false;

    string rw_mode="r";
    if(binary){
        rw_mode+="b";
    }

    SDL_RWops* rwops=SDL_RWFromFile(path.c_str(),rw_mode.c_str());

    if(rwops!=0){
        succeeded=true;
        load_success=true;

        if(path_is_backup){
            loaded_backup=true;

            Log::add_log("Successfully opened backup file: '"+path+"'");
        }

        unsigned char* data_chunk=new unsigned char[100];

        for(long length=0;(length=SDL_RWread(rwops,data_chunk,1,100))>0;){
            for(long i=0;i<length;i++){
                data+=data_chunk[i];
            }
        }

        delete[] data_chunk;

        if(SDL_RWclose(rwops)!=0 && !suppress_errors){
            string msg="Error closing file '"+path+"' after loading: ";
            msg+=SDL_GetError();
            Log::add_error(msg,false);
        }
    }
    else{
        if(!suppress_errors){
            string msg="Error opening file '"+path+"' for loading: ";
            msg+=SDL_GetError();
            Log::add_error(msg,false);
        }

        string path_backup=path+File_IO::SAVE_SUFFIX_BACKUP;

        if(!path_is_backup && File_IO::is_regular_file(path_backup)){
            open(path_backup,true,binary,suppress_errors);
        }
    }

    if(!binary && succeeded){
        data_stream.str(data.c_str());
    }
}

void File_IO_Load::open(SDL_RWops* rwops,bool get_binary){
    binary=get_binary;

    if(rwops!=0){
        load_success=true;

        unsigned char* data_chunk=new unsigned char[100];

        for(long length=0;(length=SDL_RWread(rwops,data_chunk,1,100))>0;){
            for(long i=0;i<length;i++){
                data+=data_chunk[i];
            }
        }

        delete[] data_chunk;

        SDL_RWclose(rwops);
    }

    if(!binary){
        data_stream.str(data.c_str());
    }
}

void File_IO_Load::close(){
    binary=false;
    load_success=false;
    loaded_backup=false;
    data="";
    data_stream.str("");
}

bool File_IO_Load::is_opened(){
    return load_success;
}

bool File_IO_Load::is_backup(){
    return loaded_backup;
}

bool File_IO_Load::eof(){
    if(data_stream.eof()){
        return true;
    }
    else{
        return false;
    }
}

void File_IO_Load::getline(string* line){
    if(!binary){
        std::getline(data_stream,*line);
    }
}

string File_IO_Load::get_data(){
    return data;
}

File_IO_Save::File_IO_Save(string get_path,bool append,bool binary){
    rwops=0;
    path="";

    open(get_path,append,binary);
}

void File_IO_Save::open(string get_path,bool append,bool binary){
    path=get_path;

    string rw_mode="w";
    if(append){
        rw_mode="a";
    }
    if(binary){
        rw_mode+="b";
    }

    rwops=SDL_RWFromFile(path.c_str(),rw_mode.c_str());

    if(!is_opened()){
        string msg="Error opening file '"+path+"' for saving: ";
        msg+=SDL_GetError();
        Log::add_error(msg,false);
    }
}

bool File_IO_Save::close(){
    int close_result=SDL_RWclose(rwops);

    rwops=0;

    if(close_result!=0){
        string msg="Error closing file '"+path+"' after saving: ";
        msg+=SDL_GetError();
        Log::add_error(msg,false);

        path="";

        return false;
    }
    else{
        path="";

        return true;
    }
}

bool File_IO_Save::is_opened(){
    if(rwops!=0){
        return true;
    }
    else{
        return false;
    }
}

bool File_IO_Save::write(const void* ptr,size_t data_size,size_t data_count){
    size_t write_count=SDL_RWwrite(rwops,ptr,data_size,data_count);

    if(write_count!=data_count){
        string msg="Error saving file '"+path+"': ";
        msg+=SDL_GetError();
        Log::add_error(msg,false);

        return false;
    }
    else{
        return true;
    }
}

bool File_IO::save_file(string path,string data,bool append,bool binary){
    File_IO_Save save(path,append,binary);

    if(save.is_opened()){
        const char* data_chars=data.c_str();

        size_t length=SDL_strlen(data_chars);

        bool write_result=save.write(data_chars,1,length);

        bool close_result=save.close();

        if(!write_result || !close_result){
            return false;
        }
    }
    else{
        return false;
    }

    return true;
}

bool File_IO::save_atomic(string path,string data,bool backup,bool append,bool binary){
    string path_temp=path+SAVE_SUFFIX_TEMPORARY;

    if(append){
        if(exists(path_temp)){
            if(!remove_file(path_temp)){
                return false;
            }
        }

        if(exists(path)){
            if(!copy_file(path,path_temp)){
                return false;
            }
        }
    }

    if(save_file(path_temp,data,append,binary)){
        if(backup){
            string path_backup=path+SAVE_SUFFIX_BACKUP;

            if(exists(path_backup)){
                if(!remove_file(path_backup)){
                    return false;
                }
            }

            if(exists(path)){
                if(!rename_file(path,path_backup)){
                    return false;
                }
            }
            else{
                if(!copy_file(path_temp,path_backup)){
                    return false;
                }
            }
        }

        return rename_file(path_temp,path);
    }
    else{
        return false;
    }
}

#ifdef GAME_OS_ANDROID
    bool File_IO::exists(string path){
        DIR* dir=0;

        if((dir=opendir(path.c_str()))!=0){
            closedir(dir);

            return true;
        }

        File_IO_Load load(path);

        if(load.is_opened()){
            return true;
        }
        else{
            return false;
        }
    }

    bool File_IO::is_directory(string path){
        struct stat file_info;
        string file_name_compare=get_file_name(path);

        if(stat(path.c_str(),&file_info)==0){
            if(file_name_compare!="." && file_name_compare!=".." && S_ISDIR(file_info.st_mode)){
                return true;
            }
        }
        else{
            Log::add_error("Error getting file information for file '"+path+"': "+Strings::num_to_string(errno),false);
        }

        return false;
    }

    bool File_IO::is_regular_file(string path){
        struct stat file_info;

        if(stat(path.c_str(),&file_info)==0){
            if(S_ISREG(file_info.st_mode)){
                return true;
            }
        }
        else{
            Log::add_error("Error getting file information for file '"+path+"': "+Strings::num_to_string(errno),false);
        }

        return false;
    }

    bool File_IO::create_directory(string path){
        if(!exists(path) && mkdir(path.c_str(),S_IRWXU|S_IRWXG|S_IRWXO)!=0){
            Log::add_error("Error creating directory '"+path+"': "+Strings::num_to_string(errno),false);

            return false;
        }
        else{
            return true;
        }
    }

    bool File_IO::rename_file(string old_path,string new_path){
        if(is_regular_file(old_path)){
            if(rename(old_path.c_str(),new_path.c_str())!=0){
                Log::add_error("Error renaming file '"+old_path+"' to '"+new_path+"': "+Strings::num_to_string(errno),false);

                return false;
            }
            else{
                return true;
            }
        }
        else{
            Log::add_error("Error renaming file: '"+old_path+"' is not a regular file",false);

            return false;
        }
    }

    bool File_IO::copy_file(string old_path,string new_path){
        if(is_regular_file(old_path)){
            if(!exists(new_path)){
                File_IO_Load load(old_path,false,true);

                if(load.is_opened()){
                    File_IO_Save save(new_path,false,true);

                    if(save.is_opened()){
                        const char* data_chars=load.get_data().c_str();

                        size_t length=SDL_strlen(data_chars);

                        bool write_result=save.write(data_chars,1,length);

                        bool close_result=save.close();

                        if(!write_result || !close_result){
                            return false;
                        }
                    }
                    else{
                        Log::add_error("Error opening file '"+new_path+"' for copying to",false);

                        return false;
                    }
                }
                else{
                    Log::add_error("Failed to load file for copying from: '"+old_path+"'",false);

                    return false;
                }
            }
            else{
                Log::add_error("Error copying file to '"+new_path+"': file exists",false);

                return false;
            }
        }
        else{
            Log::add_error("Error copying file: '"+old_path+"' is not a regular file",false);

            return false;
        }

        return true;
    }

    bool File_IO::remove_file(string path){
        if(exists(path)){
            if(is_regular_file(path)){
                if(remove(path.c_str())!=0){
                    Log::add_error("Error removing file '"+path+"': "+Strings::num_to_string(errno),false);

                    return false;
                }
            }
            else{
                Log::add_error("Error removing file: '"+path+"' is not a regular file",false);

                return false;
            }
        }
        else{
            Log::add_error("Error removing file: '"+path+"' does not exist",false);

            return false;
        }

        return true;
    }

    void File_IO::remove_directory(string path){
        //Look through all of the files in the directory.
        for(File_IO_Directory_Iterator_User_Data it(path);it.evaluate();it.iterate()){
            //If the file is a directory.
            if(it.is_directory()){
                remove_directory(it.get_full_path());
            }
            else if(it.is_regular_file()){
                remove_file(it.get_full_path());
            }
        }

        if(exists(path) && is_directory(path)){
            rmdir(path.c_str());
        }
    }

    string File_IO::get_file_name(string path){
        for(int i=0;i<path.size();i++){
            if(path[i]=='/' || path[i]=='\\'){
                path.erase(0,i+1);
                i=0;
            }
        }

        return path;
    }

    bool File_IO::external_storage_available(){
        int external_storage_state=SDL_AndroidGetExternalStorageState();

        if(external_storage_state!=0){
            if((external_storage_state&SDL_ANDROID_EXTERNAL_STORAGE_READ)!=0 &&
               (external_storage_state&SDL_ANDROID_EXTERNAL_STORAGE_WRITE)!=0){
                return true;
            }
            else{
                Log::add_error("External storage is not RW enabled.",false);
            }
        }
        else{
            string msg="External storage is unavailable: ";
            msg+=SDL_GetError();
            Log::add_error(msg,false);
        }

        return false;
    }

    File_IO_Directory_Iterator::File_IO_Directory_Iterator(string get_directory){
        directory=get_directory;
        entry=0;

        File_IO_Load load(directory+"/asset_list");
        if(load.is_opened()){
            while(!load.eof()){
                string line="";

                load.getline(&line);

                boost::algorithm::trim(line);

                if(line.length()>0){
                    asset_list.push_back(line);
                }
            }
        }
    }

    bool File_IO_Directory_Iterator::evaluate(){
        if(entry<asset_list.size()){
            return true;
        }
        else{
            return false;
        }
    }

    void File_IO_Directory_Iterator::iterate(){
        entry++;
    }

    bool File_IO_Directory_Iterator::is_directory(){
        return false;
    }

    bool File_IO_Directory_Iterator::is_regular_file(){
        return true;
    }

    string File_IO_Directory_Iterator::get_full_path(){
        return directory+"/"+get_file_name();
    }

    string File_IO_Directory_Iterator::get_file_name(){
        return asset_list[entry];
    }

    File_IO_Directory_Iterator_User_Data::File_IO_Directory_Iterator_User_Data(string get_directory){
        directory=get_directory;

        dir_entry=0;
        entry=0;
        entries=0;

        if((dir=opendir(directory.c_str()))!=0){
            while(dir_entry=readdir(dir)){
                entries++;
            }

            closedir(dir);

            dir=opendir(directory.c_str());

            dir_entry=readdir(dir);
        }
    }

    File_IO_Directory_Iterator_User_Data::~File_IO_Directory_Iterator_User_Data(){
        if(dir!=0){
            closedir(dir);
        }
    }

    bool File_IO_Directory_Iterator_User_Data::evaluate(){
        if(entry<entries && exists()){
            return true;
        }
        else{
            return false;
        }
    }

    void File_IO_Directory_Iterator_User_Data::iterate(){
        if(++entry<entries){
            dir_entry=readdir(dir);
        }
    }

    bool File_IO_Directory_Iterator_User_Data::exists(){
        if(dir_entry!=0){
            return true;
        }
        else{
            return false;
        }
    }

    bool File_IO_Directory_Iterator_User_Data::is_regular_file(){
        struct stat file_info;

        if(stat(get_full_path().c_str(),&file_info)==0){
            if(S_ISREG(file_info.st_mode)){
                return true;
            }
        }
        else{
            Log::add_error("Error getting file information for file '"+get_full_path()+"': "+Strings::num_to_string(errno),false);
        }

        return false;
    }

    bool File_IO_Directory_Iterator_User_Data::is_directory(){
        struct stat file_info;
        string file_name_compare=dir_entry->d_name;

        if(stat(get_full_path().c_str(),&file_info)==0){
            if(file_name_compare!="." && file_name_compare!=".." && S_ISDIR(file_info.st_mode)){
                return true;
            }
        }
        else{
            Log::add_error("Error getting file information for file '"+get_full_path()+"': "+Strings::num_to_string(errno),false);
        }

        return false;
    }

    string File_IO_Directory_Iterator_User_Data::get_full_path(){
        return directory+"/"+get_file_name();
    }

    string File_IO_Directory_Iterator_User_Data::get_file_name(){
        return dir_entry->d_name;
    }
#else
    bool File_IO::exists(string path){
        try{
            return boost::filesystem::exists(path);
        }
        catch(boost::filesystem::filesystem_error e){
            string error_message=e.what();
            Log::add_error("Error checking existence of '"+path+"': "+error_message,false);

            return false;
        }
        catch(...){
            Log::add_error("Error checking existence of '"+path+"'",false);

            return false;
        }
    }

    bool File_IO::is_directory(string path){
        try{
            return boost::filesystem::is_directory(path);
        }
        catch(boost::filesystem::filesystem_error e){
            string error_message=e.what();
            Log::add_error("Error checking directory status of '"+path+"': "+error_message,false);

            return false;
        }
        catch(...){
            Log::add_error("Error checking directory status of '"+path+"'",false);

            return false;
        }
    }

    bool File_IO::is_regular_file(string path){
        try{
            return boost::filesystem::is_regular_file(path);
        }
        catch(boost::filesystem::filesystem_error e){
            string error_message=e.what();
            Log::add_error("Error checking regular file status of '"+path+"': "+error_message,false);

            return false;
        }
        catch(...){
            Log::add_error("Error checking regular file status of '"+path+"'",false);

            return false;
        }
    }

    bool File_IO::create_directory(string path){
        try{
            if(!boost::filesystem::create_directory(path)){
                Log::add_error("Error creating directory '"+path+"': boost::filesystem::create_directory returned false",false);

                return false;
            }
            else{
                return true;
            }
        }
        catch(boost::filesystem::filesystem_error e){
            string error_message=e.what();
            Log::add_error("Error creating directory '"+path+"': "+error_message,false);

            return false;
        }
        catch(...){
            Log::add_error("Error creating directory '"+path+"'",false);

            return false;
        }
    }

    bool File_IO::rename_file(string old_path,string new_path){
        try{
            boost::filesystem::rename(old_path,new_path);

            return true;
        }
        catch(boost::filesystem::filesystem_error e){
            string error_message=e.what();
            Log::add_error("Error renaming file '"+old_path+"': "+error_message,false);

            return false;
        }
        catch(...){
            Log::add_error("Error renaming file '"+old_path+"'",false);

            return false;
        }
    }

    bool File_IO::copy_file(string old_path,string new_path){
        if(is_regular_file(old_path)){
            if(!exists(new_path)){
                try{
                    boost::filesystem::copy(old_path,new_path);
                }
                catch(boost::filesystem::filesystem_error e){
                    string error_message=e.what();
                    Log::add_error("Error copying file '"+old_path+"': "+error_message,false);

                    return false;
                }
                catch(...){
                    Log::add_error("Error copying file '"+old_path+"'",false);

                    return false;
                }
            }
            else{
                Log::add_error("Error copying file to '"+new_path+"': file exists",false);

                return false;
            }
        }
        else{
            Log::add_error("Error copying file: '"+old_path+"' is not a regular file",false);

            return false;
        }

        return true;
    }

    bool File_IO::remove_file(string path){
        try{
            if(!boost::filesystem::remove(path)){
                Log::add_error("Error removing file: '"+path+"' does not exist",false);

                return false;
            }
            else{
                return true;
            }
        }
        catch(boost::filesystem::filesystem_error e){
            string error_message=e.what();
            Log::add_error("Error removing file '"+path+"': "+error_message,false);

            return false;
        }
        catch(...){
            Log::add_error("Error removing file '"+path+"'",false);

            return false;
        }
    }

    void File_IO::remove_directory(string path){
        try{
            boost::filesystem::remove_all(path);
        }
        catch(boost::filesystem::filesystem_error e){
            string error_message=e.what();
            Log::add_error("Error removing directory '"+path+"': "+error_message,false);
        }
        catch(...){
            Log::add_error("Error removing directory '"+path+"'",false);
        }
    }

    string File_IO::get_file_name(string path){
        boost::filesystem::path boost_path(path);
        return boost_path.filename().string();
    }

    File_IO_Directory_Iterator::File_IO_Directory_Iterator(string get_directory){
        it=boost::filesystem::directory_iterator(get_directory);
    }

    bool File_IO_Directory_Iterator::evaluate(){
        if(it!=boost::filesystem::directory_iterator()){
            return true;
        }
        else{
            return false;
        }
    }

    void File_IO_Directory_Iterator::iterate(){
        it++;
    }

    bool File_IO_Directory_Iterator::is_directory(){
        if(boost::filesystem::is_directory(it->path())){
            return true;
        }
        else{
            return false;
        }
    }

    bool File_IO_Directory_Iterator::is_regular_file(){
        if(boost::filesystem::is_regular_file(it->path())){
            return true;
        }
        else{
            return false;
        }
    }

    string File_IO_Directory_Iterator::get_full_path(){
        return it->path().string();
    }

    string File_IO_Directory_Iterator::get_file_name(){
        return it->path().filename().string();
    }
#endif
