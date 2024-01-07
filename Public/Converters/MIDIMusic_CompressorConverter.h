#pragma once

#include <vector>
#include <memory>
#include "PMIDIEvent.h"
#include "MIDIMusic.h"
#include "MIDIMusic_AbsoluteConverter.h"
#include "MIDIMusic_RelativeConverter.h"
 
// Reduces tick precision
class MIDIPARSEREXPORT MIDIMusic_CompressorConverter
{
public:
    int16_t newTicksPerQuarterNote = 4; // is 4 the default ?

    MIDIMusic* music = nullptr;

    MIDIMusic_CompressorConverter(int16_t ticksPerQuarterNote) : newTicksPerQuarterNote(ticksPerQuarterNote)
    {

    }

    void Convert(class MIDIMusic& newMusic);

    // If music is already in absolute time, then performance gain
    void ConvertUnsafe(class MIDIMusic& newMusic);

    uint32_t OldToNewTime(uint32_t oldTime)
    {
        return oldTime * newTicksPerQuarterNote / music->ticksPerQuarterNote;
    }

    bool operator()(std::shared_ptr<PMIDIEvent>& event);
};

