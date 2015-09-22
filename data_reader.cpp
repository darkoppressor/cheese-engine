/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "data_reader.h"

#include <boost/algorithm/string.hpp>

using namespace std;

vector<string> Data_Reader::read_data(File_IO_Load* load,string end_tag){
    vector<string> lines;

    bool multi_line_comment=false;

    while(!load->eof()){
        string line="";

        load->getline(&line);
        boost::algorithm::trim(line);

        if(boost::algorithm::contains(line,"*/")){
            multi_line_comment=false;
        }

        if(!multi_line_comment){
            if(boost::algorithm::starts_with(line,"/*")){
                multi_line_comment=true;

                continue;
            }
            else if(boost::algorithm::starts_with(line,"//")){
            }
            else if(boost::algorithm::starts_with(line,end_tag)){
                break;
            }
            else{
                lines.push_back(line);
            }
        }
    }

    return lines;
}

bool Data_Reader::check_prefix(string& line,string prefix){
    if(boost::algorithm::starts_with(line,prefix)){
        line.erase(0,prefix.length());

        return true;
    }

    return false;
}
