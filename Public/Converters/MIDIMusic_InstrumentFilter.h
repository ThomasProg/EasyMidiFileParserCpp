#pragma once

#include "Macros.h"
#include <cstdint>

// Only keeps the target instrument
// Only works for mono tracks
class MIDIPARSEREXPORT MIDIMusic_InstrumentFilter
{
public:
    uint8_t start = 0;
    uint8_t end = 0;
    bool filterOut = true;

    MIDIMusic_InstrumentFilter(uint8_t instrument, bool filterOut = true)
    {
        this->start = instrument;
        this->end = instrument;
        this->filterOut = filterOut;
    }

    MIDIMusic_InstrumentFilter(uint8_t start, uint8_t end, bool filterOut = true)
    {
        this->start = start;
        this->end = end;
        this->filterOut = filterOut;
    }

    void Convert(class MIDIMusic& music);

    static void Piano(class MIDIMusic& music)
    {
        MIDIMusic_InstrumentFilter(0, 7, true).Convert(music);
    }

    static void Guitar(class MIDIMusic& music)
    {
        MIDIMusic_InstrumentFilter(24, 31, true).Convert(music);
    }
    
    static void Bass(class MIDIMusic& music)
    {
        MIDIMusic_InstrumentFilter(32, 39, true).Convert(music);
    }

    static void Strings(class MIDIMusic& music)
    {
        MIDIMusic_InstrumentFilter(40, 47, true).Convert(music);
    }
};

