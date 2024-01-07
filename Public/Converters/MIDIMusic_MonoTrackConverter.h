#pragma once

#include <vector>
#include <memory>
#include <map>
#include "PMIDIEvent.h"
#include "MIDIMusic.h"
#include "MIDIMusic_AbsoluteConverter.h"
#include "MIDIMusic_RelativeConverter.h"

// Combines every track into only 1 track
class MIDIPARSEREXPORT MIDIMusic_MonoTrackConverter
{
public:
    void Convert(class MIDIMusic& newMusic);

    // If music is already in absolute time, then performance gain
    void ConvertUnsafe(class MIDIMusic& newMusic);
};

