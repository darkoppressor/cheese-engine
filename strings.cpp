/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "strings.h"
#include "log.h"

#include <vector>

#include <boost/algorithm/string.hpp>

using namespace std;

bool Strings::is_newline_character(char input){
    string newline="\xA";

    if(input==newline[0]){
        return true;
    }
    else{
        return false;
    }
}

string Strings::first_letter_capital(string str_input){
    if(str_input.length()>0){
        string first_letter="";
        first_letter+=str_input[0];

        boost::algorithm::to_upper(first_letter);

        str_input.erase(str_input.begin());
        str_input.insert(0,first_letter);
    }

    return str_input;
}

string Strings::capitalize_all_words(const string& str_input){
    string message="";

    vector<string> words;
    boost::algorithm::split(words,str_input,boost::algorithm::is_any_of(" "));

    for(int i=0;i<words.size();i++){
        words[i]=first_letter_capital(words[i]);

        message+=words[i];

        if(i<words.size()-1){
            message+=" ";
        }
    }

    return message;
}

string Strings::upper_case(const string& str_input){
    return boost::algorithm::to_upper_copy(str_input);
}

string Strings::lower_case(const string& str_input){
    return boost::algorithm::to_lower_copy(str_input);
}

string Strings::underscore_to_space(const string& str_input){
    return boost::algorithm::replace_all_copy(str_input,"_"," ");
}

string Strings::add_newlines(const string& str_input){
    string newline="\\";
    newline+="n";

    return boost::algorithm::replace_all_copy(str_input,"\xA",newline);
}

string Strings::process_newlines(const string& str_input){
    string newline="\\";
    newline+="n";

    return boost::algorithm::replace_all_copy(str_input,newline,"\xA");
}

int Strings::newline_count(const string& str_input){
    int newlines=0;

    for(int i=0;i<str_input.length();i++){
        if(is_newline_character(str_input[i])){
            newlines++;
        }
    }

    return newlines;
}

int Strings::length_of_last_line(const string& str_input){
    vector<string> lines;
    boost::algorithm::split(lines,str_input,boost::algorithm::is_any_of("\xA"));

    return lines.back().length();
}

int Strings::length_of_line(const string& str_input,int line){
    vector<string> lines;
    boost::algorithm::split(lines,str_input,boost::algorithm::is_any_of("\xA"));

    if(line>=0 && line<lines.size()){
        return lines[line].length();
    }
    else{
        return 0;
    }
}

int Strings::longest_line(const string& str_input){
    vector<string> lines;
    boost::algorithm::split(lines,str_input,boost::algorithm::is_any_of("\xA"));

    int longest_line_length=0;
    for(int i=0;i<lines.size();i++){
        if(lines[i].length()>longest_line_length){
            longest_line_length=lines[i].length();
        }
    }

    return longest_line_length;
}

int Strings::which_line(const string& str_input,int character){
    int line=0;

    if(character<0 || character>str_input.length()-1){
        return 0;
    }

    for(int i=0;i<character;i++){
        if(is_newline_character(str_input[i])){
            line++;
        }
    }

    return line;
}

int Strings::where_on_line(const string& str_input,int character){
    int location=0;

    if(character<0 || character>str_input.length()-1){
        return 0;
    }

    for(int i=0;i<character;i++){
        if(is_newline_character(str_input[i])){
            location=0;
        }
        else{
            location++;
        }
    }

    return location;
}

int Strings::which_character(const string& str_input,int line,int index_on_line){
    int character=0;

    int lines=newline_count(str_input)+1;

    if(line<0 || line>lines-1){
        return 0;
    }

    //The character index of the first character on the passed line
    int line_start=-1;

    for(int i=0,checking_line=0;i<str_input.length();i++){
        if(is_newline_character(str_input[i])){
            checking_line++;
        }

        if(checking_line==line){
            if(line==0){
                line_start=i;
            }
            else{
                line_start=i+1;
            }

            break;
        }
    }

    if(line_start>-1){
        for(int i=line_start,checking_character=0;i<str_input.length();i++,checking_character++){
            if(is_newline_character(str_input[i])){
                break;
            }

            if(checking_character==index_on_line){
                character=i;

                break;
            }
        }
    }

    return character;
}

string Strings::erase_first_line(string str_input){
    for(int i=0;i<str_input.length();i++){
        if(is_newline_character(str_input[i])){
            str_input.erase(0,1);

            break;
        }
        else{
            str_input.erase(0,1);

            i--;
        }
    }

    return str_input;
}

bool Strings::is_number(const string& str_input){
    if(str_input.length()==0 || (str_input.length()==1 && str_input[0]=='-')){
        return false;
    }

    for(int i=0;i<str_input.length();i++){
        if(!isdigit(str_input[i]) && (i!=0 || (i==0 && str_input[i]!='-'))){
            return false;
        }
    }

    return true;
}

string Strings::time_string(int64_t seconds,bool highest){
    if(seconds<0){
        return "Negative number of seconds";
    }

    string text="";

    int64_t minutes=0;
    int64_t hours=0;
    int64_t days=0;
    int64_t weeks=0;
    int64_t months=0;
    int64_t years=0;

    for(int64_t i=seconds;i>=60;){
        minutes++;
        i-=60;
        if(i<60){
            seconds=i;
        }
    }
    for(int64_t i=minutes;i>=60;){
        hours++;
        i-=60;
        if(i<60){
            minutes=i;
        }
    }
    for(int64_t i=hours;i>=24;){
        days++;
        i-=24;
        if(i<24){
            hours=i;
        }
    }
    for(int64_t i=days;i>=7;){
        weeks++;
        i-=7;
        if(i<7){
            days=i;
        }
    }
    for(int64_t i=weeks;i>=4;){
        months++;
        i-=4;
        if(i<4){
            weeks=i;
        }
    }
    for(int64_t i=months;i>=12;){
        years++;
        i-=12;
        if(i<12){
            months=i;
        }
    }

    if(years>0){
        text+=num_to_string(years)+" year";
        if(years!=1){
            text+="s";
        }
        if(!highest){
            text+=", ";
        }
    }
    if((!highest || years<=0) && months>0){
        text+=num_to_string(months)+" month";
        if(months!=1){
            text+="s";
        }
        if(!highest){
            text+=", ";
        }
    }
    if((!highest || (years<=0 && months<=0)) && weeks>0){
        text+=num_to_string(weeks)+" week";
        if(weeks!=1){
            text+="s";
        }
        if(!highest){
            text+=", ";
        }
    }
    if((!highest || (years<=0 && months<=0 && weeks<=0)) && days>0){
        text+=num_to_string(days)+" day";
        if(days!=1){
            text+="s";
        }
        if(!highest){
            text+=", ";
        }
    }
    if((!highest || (years<=0 && months<=0 && weeks<=0 && days<=0)) && hours>0){
        text+=num_to_string(hours)+" hour";
        if(hours!=1){
            text+="s";
        }
        if(!highest){
            text+=", ";
        }
    }
    if((!highest || (years<=0 && months<=0 && weeks<=0 && days<=0 && hours<=0)) && minutes>0){
        text+=num_to_string(minutes)+" minute";
        if(minutes!=1){
            text+="s";
        }
        if(!highest){
            text+=", ";
        }
    }
    if(!highest || (years<=0 && months<=0 && weeks<=0 && days<=0 && hours<=0 && minutes<=0)){
        text+=num_to_string(seconds)+" second";
        if(seconds!=1){
            text+="s";
        }
    }

    return text;
}

bool Strings::string_to_bool(const string& get_string){
    string str_lower_case=lower_case(get_string);

    if(str_lower_case=="true" || str_lower_case=="on" || get_string=="1"){
        return true;
    }
    else if(str_lower_case=="false" || str_lower_case=="off" || get_string=="0"){
        return false;
    }
    else{
        Log::add_error("Invalid value for string_to_bool: '"+get_string+"'");

        return false;
    }
}

string Strings::bool_to_string(bool get_bool){
    if(get_bool){
        return "true";
    }
    else{
        return "false";
    }
}

long Strings::string_to_long(const string& get_string){
    return strtol(get_string.c_str(),NULL,0);
}

unsigned long Strings::string_to_unsigned_long(const string& get_string){
    return strtoul(get_string.c_str(),NULL,0);
}

double Strings::string_to_double(const string& get_string){
    return strtod(get_string.c_str(),NULL);
}

void Strings::slashes_to_slash(string* str_input){
    boost::algorithm::replace_all(*str_input,"\\","/");
}

void Strings::slashes_to_backslash(string* str_input){
    boost::algorithm::replace_all(*str_input,"/","\\");
}
