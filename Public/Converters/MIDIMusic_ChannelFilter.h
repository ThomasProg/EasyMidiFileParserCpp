#pragma once

#include "Macros.h"
#include <cstdint>

// Only keeps the target Channel
class MIDIPARSEREXPORT MIDIMusic_ChannelFilter
{
public:
    uint8_t filteredChannel = 0;
    bool filterOut = true;

    MIDIMusic_ChannelFilter(uint8_t filteredChannel, bool filterOut = true)
    {
        this->filteredChannel = filteredChannel;
        this->filterOut = filterOut;
    }

    void Convert(class MIDIMusic& music);
};

