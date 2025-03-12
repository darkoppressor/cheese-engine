/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "symbols.h"

using namespace std;

string Symbols::degrees () {
    return string(1, (unsigned char) 167);
}

string Symbols::dot () {
    return string(1, (unsigned char) 250);
}

string Symbols::squared () {
    return string(1, (unsigned char) 253);
}

string Symbols::cubed () {
    return string(1, (unsigned char) 252);
}

string Symbols::cursor () {
    return string(1, (unsigned char) 128);
}
