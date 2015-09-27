/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "custom_sound.h"
#include "rng.h"
#include "strings.h"
#include "engine_math.h"

#include <boost/algorithm/string.hpp>

using namespace std;

Custom_Sound::Custom_Sound(){
    sample_rate=0;
    tempo=0.0;
    channels=0;

    sharps="";
    flats="";

    name="";

    length="quarter";
}

void Custom_Sound::set_length(string get_length){
    length=get_length;
}

void Custom_Sound::set_volumes(string get_values){
    double default_value=0.5;

    volumes.clear();

    vector<string> values;
    boost::algorithm::split(values,get_values,boost::algorithm::is_any_of(","));

    for(int i=0;i<channels;i++){
        if(i<values.size()){
            volumes.push_back(Strings::string_to_double(values[i]));

            default_value=Strings::string_to_double(values[i]);
        }
        else{
            volumes.push_back(default_value);
        }
    }
}

void Custom_Sound::set_waveforms(string get_values){
    string default_value="sine";

    waveforms.clear();

    vector<string> values;
    boost::algorithm::split(values,get_values,boost::algorithm::is_any_of(","));

    for(int i=0;i<channels;i++){
        if(i<values.size()){
            waveforms.push_back(values[i]);

            default_value=values[i];
        }
        else{
            waveforms.push_back(default_value);
        }
    }
}

void Custom_Sound::set_frequencies(string get_values){
    string default_value="C";

    frequencies.clear();

    vector<string> values;
    boost::algorithm::split(values,get_values,boost::algorithm::is_any_of(","));

    for(int i=0;i<channels;i++){
        if(i<values.size()){
            frequencies.push_back(values[i]);

            default_value=values[i];
        }
        else{
            frequencies.push_back(default_value);
        }
    }
}

double Custom_Sound::get_note_frequency(string frequency_string){
    //Note letters all on their own are shortcuts for the middle octave frequencies
    if(frequency_string=="A"){
        frequency_string="A5";
    }
    else if(frequency_string=="B"){
        frequency_string="B5";
    }
    else if(frequency_string=="C"){
        frequency_string="C5";
    }
    else if(frequency_string=="D"){
        frequency_string="D5";
    }
    else if(frequency_string=="E"){
        frequency_string="E5";
    }
    else if(frequency_string=="F"){
        frequency_string="F5";
    }
    else if(frequency_string=="G"){
        frequency_string="G5";
    }

    if(!boost::algorithm::contains(frequency_string,"n")){
        if(frequency_string.length()>0 && !boost::algorithm::contains(frequency_string,"#") && boost::algorithm::contains(sharps,string(1,frequency_string[0]))){
            frequency_string+="#";
        }

        if(frequency_string.length()>0 && !boost::algorithm::contains(frequency_string,"b") && boost::algorithm::contains(flats,string(1,frequency_string[0]))){
            frequency_string+="b";
        }
    }
    else{
        boost::algorithm::erase_first(frequency_string,"n");
    }

    if(frequency_string=="C0"){
        return 16.35;
    }
    else if(frequency_string=="D0"){
        return 18.35;
    }
    else if(frequency_string=="E0"){
        return 20.60;
    }
    else if(frequency_string=="F0"){
        return 21.83;
    }
    else if(frequency_string=="G0"){
        return 24.50;
    }
    else if(frequency_string=="A0"){
        return 27.50;
    }
    else if(frequency_string=="B0"){
        return 30.87;
    }
    else if(frequency_string=="C1"){
        return 32.70;
    }
    else if(frequency_string=="D1"){
        return 36.71;
    }
    else if(frequency_string=="E1"){
        return 41.20;
    }
    else if(frequency_string=="F1"){
        return 43.65;
    }
    else if(frequency_string=="G1"){
        return 49.00;
    }
    else if(frequency_string=="A1"){
        return 55.00;
    }
    else if(frequency_string=="B1"){
        return 61.74;
    }
    else if(frequency_string=="C2"){
        return 65.41;
    }
    else if(frequency_string=="D2"){
        return 73.42;
    }
    else if(frequency_string=="E2"){
        return 82.41;
    }
    else if(frequency_string=="F2"){
        return 87.31;
    }
    else if(frequency_string=="G2"){
        return 98.00;
    }
    else if(frequency_string=="A2"){
        return 110.00;
    }
    else if(frequency_string=="B2"){
        return 123.47;
    }
    else if(frequency_string=="C3"){
        return 130.81;
    }
    else if(frequency_string=="D3"){
        return 146.83;
    }
    else if(frequency_string=="E3"){
        return 164.81;
    }
    else if(frequency_string=="F3"){
        return 174.61;
    }
    else if(frequency_string=="G3"){
        return 196.00;
    }
    else if(frequency_string=="A3"){
        return 220.00;
    }
    else if(frequency_string=="B3"){
        return 246.94;
    }
    else if(frequency_string=="C4"){
        return 261.63;
    }
    else if(frequency_string=="D4"){
        return 293.66;
    }
    else if(frequency_string=="E4"){
        return 329.63;
    }
    else if(frequency_string=="F4"){
        return 349.23;
    }
    else if(frequency_string=="G4"){
        return 392.00;
    }
    else if(frequency_string=="A4"){
        return 440.00;
    }
    else if(frequency_string=="B4"){
        return 493.88;
    }
    else if(frequency_string=="C5"){
        return 523.25;
    }
    else if(frequency_string=="D5"){
        return 587.33;
    }
    else if(frequency_string=="E5"){
        return 659.25;
    }
    else if(frequency_string=="F5"){
        return 698.46;
    }
    else if(frequency_string=="G5"){
        return 783.99;
    }
    else if(frequency_string=="A5"){
        return 880.00;
    }
    else if(frequency_string=="B5"){
        return 987.77;
    }
    else if(frequency_string=="C6"){
        return 1046.50;
    }
    else if(frequency_string=="D6"){
        return 1174.66;
    }
    else if(frequency_string=="E6"){
        return 1318.51;
    }
    else if(frequency_string=="F6"){
        return 1396.91;
    }
    else if(frequency_string=="G6"){
        return 1567.98;
    }
    else if(frequency_string=="A6"){
        return 1760.00;
    }
    else if(frequency_string=="B6"){
        return 1975.53;
    }
    else if(frequency_string=="C7"){
        return 2093.00;
    }
    else if(frequency_string=="D7"){
        return 2349.32;
    }
    else if(frequency_string=="E7"){
        return 2637.02;
    }
    else if(frequency_string=="F7"){
        return 2793.83;
    }
    else if(frequency_string=="G7"){
        return 3135.96;
    }
    else if(frequency_string=="A7"){
        return 3520.00;
    }
    else if(frequency_string=="B7"){
        return 3951.07;
    }
    else if(frequency_string=="C8"){
        return 4186.01;
    }
    else if(frequency_string=="D8"){
        return 4698.63;
    }
    else if(frequency_string=="E8"){
        return 5274.04;
    }
    else if(frequency_string=="F8"){
        return 5587.65;
    }
    else if(frequency_string=="G8"){
        return 6271.93;
    }
    else if(frequency_string=="A8"){
        return 7040.00;
    }
    else if(frequency_string=="B8"){
        return 7902.13;
    }
    else if(frequency_string=="C0#" || frequency_string=="D0b"){
        return 17.32;
    }
    else if(frequency_string=="D0#" || frequency_string=="E0b"){
        return 19.45;
    }
    else if(frequency_string=="F0#" || frequency_string=="G0b"){
        return 23.12;
    }
    else if(frequency_string=="G0#" || frequency_string=="A0b"){
        return 25.96;
    }
    else if(frequency_string=="A0#" || frequency_string=="B0b"){
        return 29.14;
    }
    else if(frequency_string=="C1#" || frequency_string=="D1b"){
        return 34.65;
    }
    else if(frequency_string=="D1#" || frequency_string=="E1b"){
        return 38.89;
    }
    else if(frequency_string=="F1#" || frequency_string=="G1b"){
        return 46.25;
    }
    else if(frequency_string=="G1#" || frequency_string=="A1b"){
        return 51.91;
    }
    else if(frequency_string=="A1#" || frequency_string=="B1b"){
        return 58.27;
    }
    else if(frequency_string=="C2#" || frequency_string=="D2b"){
        return 69.30;
    }
    else if(frequency_string=="D2#" || frequency_string=="E2b"){
        return 77.78;
    }
    else if(frequency_string=="F2#" || frequency_string=="G2b"){
        return 92.50;
    }
    else if(frequency_string=="G2#" || frequency_string=="A2b"){
        return 103.83;
    }
    else if(frequency_string=="A2#" || frequency_string=="B2b"){
        return 116.54;
    }
    else if(frequency_string=="C3#" || frequency_string=="D3b"){
        return 138.59;
    }
    else if(frequency_string=="D3#" || frequency_string=="E3b"){
        return 155.56;
    }
    else if(frequency_string=="F3#" || frequency_string=="G3b"){
        return 185.00;
    }
    else if(frequency_string=="G3#" || frequency_string=="A3b"){
        return 207.65;
    }
    else if(frequency_string=="A3#" || frequency_string=="B3b"){
        return 233.08;
    }
    else if(frequency_string=="C4#" || frequency_string=="D4b"){
        return 277.18;
    }
    else if(frequency_string=="D4#" || frequency_string=="E4b"){
        return 311.13;
    }
    else if(frequency_string=="F4#" || frequency_string=="G4b"){
        return 369.99;
    }
    else if(frequency_string=="G4#" || frequency_string=="A4b"){
        return 415.30;
    }
    else if(frequency_string=="A4#" || frequency_string=="B4b"){
        return 466.16;
    }
    else if(frequency_string=="C5#" || frequency_string=="D5b"){
        return 554.37;
    }
    else if(frequency_string=="D5#" || frequency_string=="E5b"){
        return 622.25;
    }
    else if(frequency_string=="F5#" || frequency_string=="G5b"){
        return 739.99;
    }
    else if(frequency_string=="G5#" || frequency_string=="A5b"){
        return 830.61;
    }
    else if(frequency_string=="A5#" || frequency_string=="B5b"){
        return 932.33;
    }
    else if(frequency_string=="C6#" || frequency_string=="D6b"){
        return 1108.73;
    }
    else if(frequency_string=="D6#" || frequency_string=="E6b"){
        return 1244.51;
    }
    else if(frequency_string=="F6#" || frequency_string=="G6b"){
        return 1479.98;
    }
    else if(frequency_string=="G6#" || frequency_string=="A6b"){
        return 1661.22;
    }
    else if(frequency_string=="A6#" || frequency_string=="B6b"){
        return 1864.66;
    }
    else if(frequency_string=="C7#" || frequency_string=="D7b"){
        return 2217.46;
    }
    else if(frequency_string=="D7#" || frequency_string=="E7b"){
        return 2489.02;
    }
    else if(frequency_string=="F7#" || frequency_string=="G7b"){
        return 2959.96;
    }
    else if(frequency_string=="G7#" || frequency_string=="A7b"){
        return 3322.44;
    }
    else if(frequency_string=="A7#" || frequency_string=="B7b"){
        return 3729.31;
    }
    else if(frequency_string=="C8#" || frequency_string=="D8b"){
        return 4434.92;
    }
    else if(frequency_string=="D8#" || frequency_string=="E8b"){
        return 4978.03;
    }
    else if(frequency_string=="F8#" || frequency_string=="G8b"){
        return 5919.91;
    }
    else if(frequency_string=="G8#" || frequency_string=="A8b"){
        return 6644.88;
    }
    else if(frequency_string=="A8#" || frequency_string=="B8b"){
        return 7458.62;
    }
    else if(frequency_string=="rest"){
        return 0.0;
    }
    else{
        return Strings::string_to_double(frequency_string);
    }
}

double Custom_Sound::get_note_length(string length_string){
    double quarter_note_length=60.0/tempo;

    if(length_string=="dotted_whole"){
        return quarter_note_length*6.0;
    }
    else if(length_string=="whole"){
        return quarter_note_length*4.0;
    }
    else if(length_string=="dotted_half"){
        return quarter_note_length*3.0;
    }
    else if(length_string=="half"){
        return quarter_note_length*2.0;
    }
    else if(length_string=="dotted_quarter"){
        return quarter_note_length*1.5;
    }
    else if(length_string=="quarter"){
        return quarter_note_length;
    }
    else if(length_string=="dotted_eighth"){
        return quarter_note_length*0.75;
    }
    else if(length_string=="eighth"){
        return quarter_note_length*0.5;
    }
    else if(length_string=="sixteenth"){
        return quarter_note_length*0.25;
    }
    else{
        return Strings::string_to_double(length_string);
    }
}

void Custom_Sound::add_note(string frequency_string,string length_string,string waveform_override,double volume_override){
    RNG rng;

    if(length_string.length()==0){
        length_string=length;
    }

    for(uint32_t x=0;x<(uint32_t)Math::ceil((double)sample_rate*get_note_length(length_string));x++){
        for(uint32_t i=0;i<channels;i++){
            string waveform=waveforms[i];
            if(waveform_override!="off"){
                waveform=waveform_override;
            }

            double volume=volumes[i];
            if(volume_override>=0.0){
                volume=volume_override;
            }

            if(volume<0.0){
                volume=0.0;
            }
            else if(volume>1.0){
                volume=1.0;
            }

            if(frequency_string.length()==0){
                frequency_string=frequencies[i];
            }

            double amplitude=0.0;

            vector<string> chord;
            boost::algorithm::split(chord,frequency_string,boost::algorithm::is_any_of("+"));

            for(int c=0;c<chord.size();c++){
                double frequency=get_note_frequency(chord[c]);

                double basic_function=(frequency*2.0*Math::PI*(double)x)/(double)sample_rate;

                if(frequency>0.0){
                    if(waveform=="sine"){
                        amplitude+=Math::sin(basic_function);
                    }
                    else if(waveform=="triangle"){
                        amplitude+=Math::asin(Math::sin(basic_function))*(2.0/Math::PI);
                    }
                    else if(waveform=="square"){
                        double addend=Math::sin(basic_function);

                        if(addend<0.0){
                            addend=-1.0;
                        }
                        else if(addend>0.0){
                            addend=1.0;
                        }

                        amplitude+=addend;
                    }
                    else if(waveform=="sawtooth"){
                        amplitude+=0.5-Math::atan(Math::tan((Math::PI/2.0)-basic_function));
                    }
                    else if(waveform=="noise"){
                        amplitude+=Math::sin((((double)rng.random_range(20,20000)*2.0*Math::PI)/(double)sample_rate)*(double)x);
                    }
                }
            }

            double volume_checked=volume;
            if(volume_checked<0.0){
                volume_checked=0.0;
            }
            else if(volume_checked>1.0){
                volume_checked=1.0;
            }

            amplitude*=volume_checked;

            amplitude*=16384.0;
            amplitude+=16384.0;

            if(amplitude<-32768.0){
                amplitude=-32768.0;
            }
            if(amplitude>32767.0){
                amplitude=32767.0;
            }

            samples.push_back((int16_t)Math::floor(amplitude));
        }
    }
}

void Custom_Sound::save(string path){
    File_IO_Save save(path,false,true);

    if(save.is_opened()){
        vector<char> buffer;

        for(size_t i=0;i<samples.size();i++){
            size_t bytes_count=sizeof samples[i];

            char bytes[bytes_count];

            ///I think endianness might be an issue here
            ///I need to look into that
            copy(static_cast<const char*>(static_cast<const void*>(&samples[i])),static_cast<const char*>(static_cast<const void*>(&samples[i]))+sizeof samples[i],bytes);

            for(size_t j=0;j<bytes_count;j++){
                buffer.push_back(bytes[j]);
            }
        }

        size_t buffer_size=buffer.size();

        save.write("RIFF",1,4);
        write_binary<int>(save,36+buffer_size);
        save.write("WAVE",1,4);
        save.write("fmt ",1,4);
        write_binary<int>(save,16);
        write_binary<short>(save,1);
        write_binary<short>(save,channels);
        write_binary<int>(save,sample_rate);
        write_binary<int>(save,sample_rate*channels*sizeof(int16_t));
        write_binary<short>(save,channels*sizeof(int16_t));
        write_binary<short>(save,8*sizeof(int16_t));
        save.write("data",1,4);
        write_binary<size_t>(save,buffer_size);
        save.write((const char*)buffer.data(),1,buffer_size);

        save.close();
    }
}
