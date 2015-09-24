/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef data_reader_h
#define data_reader_h

#include "file_io.h"

#include <vector>
#include <string>

class Data_Reader{
public:

    //Reads data from the passed file one line at a time until reaching either the end of the file or a line beginning with the passed end tag
    //Returns all of the lines read from the file, including the line with the end tag (if one was reached)
    static std::vector<std::string> read_data(File_IO_Load* load,std::string end_tag);

    static bool check_prefix(std::string& line,std::string prefix);
};

#endif
