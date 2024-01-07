#pragma once

// #include ""
#include <vector>
#include <memory>
#include "PMIDIEvent.h"

#include <cassert>
#include "MIDIMusic.h"

// Only keeps the target instrument
class MIDIPARSEREXPORT MIDIMusic_InstrumentFilter
{
public:
    uint8_t start = 0;
    uint8_t end = 0;

    uint32_t program = 0;

    MIDIMusic_InstrumentFilter(uint8_t start, uint8_t end, bool keep = true)
    {
        this->start = start;
        this->end = end;
    }

    void Convert(class MIDIMusic& music);

    static void Piano(class MIDIMusic& music)
    {
        MIDIMusic_InstrumentFilter(0, 7).Convert(music);
    }

    static void Guitar(class MIDIMusic& music)
    {
        MIDIMusic_InstrumentFilter(24, 31).Convert(music);
    }
    
    static void Bass(class MIDIMusic& music)
    {
        MIDIMusic_InstrumentFilter(32, 39).Convert(music);
    }

    static void Strings(class MIDIMusic& music)
    {
        MIDIMusic_InstrumentFilter(40, 47).Convert(music);
    }
};

