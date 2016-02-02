/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "data_reader.h"

#include <boost/algorithm/string.hpp>

using namespace std;

bool Data_Reader::check_for_comment_ender(string& line){
    if(boost::algorithm::contains(line,"*/")){
        boost::iterator_range<string::iterator> range_comment_end=boost::algorithm::find_first(line,"*/");
        boost::iterator_range<string::iterator> range_start_to_comment_end=boost::make_iterator_range(line.begin(),range_comment_end.end());
        boost::algorithm::erase_range(line,range_start_to_comment_end);
        boost::algorithm::trim(line);

        return true;
    }
    else{
        return false;
    }
}

vector<string> Data_Reader::read_data(File_IO_Load* load,string end_tag){
    vector<string> lines;

    bool multi_line_comment=false;
    bool early_break=false;

    while(!early_break && !load->eof()){
        string line="";

        load->getline(&line);
        boost::algorithm::trim(line);

        while(true){
            if(multi_line_comment && check_for_comment_ender(line)){
                multi_line_comment=false;
            }

            if(!multi_line_comment){
                if(boost::algorithm::starts_with(line,"/*")){
                    multi_line_comment=true;
                }
                else if(boost::algorithm::starts_with(line,"//")){
                    break;
                }
                else if(boost::algorithm::starts_with(line,end_tag)){
                    lines.push_back(line);

                    early_break=true;

                    break;
                }
                else{
                    lines.push_back(line);

                    break;
                }
            }
            else{
                break;
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
