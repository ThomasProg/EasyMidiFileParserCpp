#pragma once

#include "Macros.h"

// Converts deltatime from absolute (delay since the beginning of the music) to relative (delay since the last note)
// The unit is still ticks 
class MIDIPARSEREXPORT MIDIMusic_RelativeConverter
{
public:
    static void Convert(class MIDIMusic& newMusic);
};

extern "C"
{
    MIDIPARSEREXPORT void MIDIMusic_ConvertRelative(class MIDIMusic* music);
}