#pragma once

#include "Macros.h"

// Combines every track into only 1 track
class MIDIPARSEREXPORT MIDIMusic_MonoTrackConverter
{
public:
    void Convert(class MIDIMusic& newMusic);

    // If music is already in absolute time, then performance gain
    void ConvertUnsafe(class MIDIMusic& newMusic);
};

extern "C"
{
    MIDIPARSEREXPORT void MIDIMusic_ConvertToMonoTrack(class MIDIMusic* music);
}