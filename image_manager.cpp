/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "image_manager.h"
#include "file_io.h"
#include "log.h"

#include <boost/algorithm/string.hpp>

using namespace std;

vector<Image_Data> Image_Manager::images;
vector<string> Image_Manager::image_names;

Image_Data* Image_Manager::error_image=0;

void Image_Manager::set_error_image(){
    error_image=get_image("error");
}

void Image_Manager::add_image(string name,SDL_Surface* surface){
    images.push_back(Image_Data());

    images.back().load_image(surface);

    image_names.push_back(name);
}

void Image_Manager::remove_image(string name){
    for(int i=0;i<image_names.size();i++){
        if(image_names[i]==name){
            image_names.erase(image_names.begin()+i);

            images[i].unload_image();
            images.erase(images.begin()+i);

            break;
        }
    }
}

void Image_Manager::load_images(){
    error_image=0;

    //Look through all of the files in the directory.
    for(File_IO_Directory_Iterator it("data/images");it.evaluate();it.iterate()){
        //If the file is not a directory.
        if(it.is_regular_file()){
            string file_name=it.get_file_name();

            boost::algorithm::trim(file_name);

            image_names.push_back(file_name);
        }
    }

    for(int i=0;i<image_names.size();i++){
        images.push_back(Image_Data());

        images.back().load_image("data/images/"+image_names[i]);

        image_names[i].erase(image_names[i].end()-4,image_names[i].end());
    }
}

void Image_Manager::unload_images(){
    for(int i=0;i<images.size();i++){
        images[i].unload_image();
    }

    images.clear();
    image_names.clear();

    error_image=0;
}

Image_Data* Image_Manager::get_image(string name){
    Image_Data* ptr_object=0;

    for(size_t i=0;i<image_names.size();i++){
        if(image_names[i]==name){
            ptr_object=&images[i];

            break;
        }
    }

    if(ptr_object==0){
        Log::add_error("Error accessing image '"+name+"'");

        ptr_object=error_image;

        if(ptr_object==0){
            Log::add_error("Error accessing error image! Really?!");
        }
    }

    return ptr_object;
}
