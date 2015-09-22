/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef custom_sound_h
#define custom_sound_h

#include "file_io.h"

#include <string>
#include <vector>
#include <stdint.h>

class Custom_Sound{
private:

    template<typename T>
    void write_binary(File_IO_Save& save,const T& t){
        save.write((const char*)&t,sizeof(T),1);
    }

    //Convert the passed note frequency to Hz
    double get_note_frequency(std::string frequency_string);

    //Convert the passed note value to seconds
    double get_note_length(std::string length_string);

public:

    //A note length string can be either one of the strings listed below OR a double value representing the length in seconds
    //The valid note length strings are:
    //dotted_whole
    //whole
    //dotted_half
    //half
    //dotted_quarter
    //quarter
    //dotted_eighth
    //eighth
    //sixteenth

    //Valid volume values are doubles from 0.0 to 1.0

    //The valid waveform strings are:
    //sine
    //triangle
    //square
    //sawtooth
    //noise

    //Frequency strings can represent either a single frequency, or a set of frequencies added together to form a chord
    //To form a chord, simply combine multiple frequencies into a single frequency string with plus sign separators
    //Each individual frequency is represented by a capital letter A to G, a number 0 to 8, and an optional accidental (symbols explained below)
    //Additionally, for convenience, a frequency consisting of only the capital letter component will act as a shortcut to the corresponding letter in octave 5
    //Also, as a special case, a frequency of "rest" indicates a frequency value of 0.0, which is complete silence
    //Finally, a frequency can instead be a double value, directly representing the frequency in Hz

    //The symbols for accidentals are:
    //# - sharp
    //b - flat
    //n - natural

    //in Hz
    int sample_rate;

    //in beats per minute
    double tempo;

    short channels;

    //Letters contained in these strings are all suffixed with the respective modifier when getting their frequency
    //These strings should just be a list of capital letters, without any separation
    std::string sharps;
    std::string flats;

    std::string name;

    std::string length;

    //Per-channel volumes, waveforms, and frequencies
    std::vector<double> volumes;
    std::vector<std::string> waveforms;
    std::vector<std::string> frequencies;

    std::vector<int16_t> samples;

    Custom_Sound();

    void set_length(std::string get_length);

    //The following three functions setup per-channel values for volume, waveform, and frequency
    //Pass a comma-separated list of values, one for each channel
    //If fewer values are passed than channels exist, the last passed value will be used for all subsequent channels
    //If no values are passed, all channels are set to a default value
    //Passed values beyond the number of channels are discarded

    //Values should be doubles
    void set_volumes(std::string get_values);

    //Values should be valid waveform strings (see explanation near the top)
    void set_waveforms(std::string get_values);

    //Values should be valid frequency strings (see explanation near the top)
    void set_frequencies(std::string get_values);

    //Pass an empty string for frequency_string to use the currently set frequency string for each channel. Pass a frequency string to use it instead (for all channels)
    //Pass an empty string for length_string to use the currently set length string. Pass a length string to use it instead
    //Pass "off" for waveform_override to use the currently set waveforms for each channel. Pass a waveform string to specify a different waveform (for all channels)
    //Pass a negative number for volume_override to use the currently set volumes for each channel. Pass a positive volume value (including 0.0) to specify a different volume (for all channels)
    void add_note(std::string frequency_string,std::string length_string,std::string waveform_override="off",double volume_override=-1.0);

    void save(std::string path);
};

#endif
