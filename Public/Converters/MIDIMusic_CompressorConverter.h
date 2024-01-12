#pragma once

#include "Macros.h"
#include <cstdint>

// Reduces tick precision
class MIDIPARSEREXPORT MIDIMusic_CompressorConverter
{
public:
    int16_t newTicksPerQuarterNote = 4; // is 4 the default ?

    class MIDIMusic* music = nullptr;

    MIDIMusic_CompressorConverter(int16_t ticksPerQuarterNote) : newTicksPerQuarterNote(ticksPerQuarterNote)
    {

    }

    void Convert(class MIDIMusic& newMusic);

    // If music is already in absolute time, then performance gain
    void ConvertUnsafe(class MIDIMusic& newMusic);

    uint32_t OldToNewTime(uint32_t oldTime) const;
};

extern "C"
{
    MIDIPARSEREXPORT void MIDIMusic_Compress(class MIDIMusic* music, int16_t newTicksPerQuarterNote);
}