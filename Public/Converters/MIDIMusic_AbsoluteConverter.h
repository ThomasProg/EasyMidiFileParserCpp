#pragma once

#include "Macros.h"

// Converts deltatime from relative (delay since the last note) to absolute (delay since the beginning of the music)
// The unit is still ticks 
class MIDIPARSEREXPORT MIDIMusic_AbsoluteConverter
{
public:
    static void Convert(class MIDIMusic& newMusic);
};

extern "C"
{
    MIDIPARSEREXPORT void MIDIMusic_ConvertAbsolute(class MIDIMusic* music);
}