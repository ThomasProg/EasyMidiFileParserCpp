#pragma once

// #include ""
#include <vector>
#include <memory>
#include "PMIDIEvent.h"

// Convert NoteOn events and NoteOff events into NoteOnOff events
class MIDIPARSEREXPORT MIDIMusic_NoteOnOffConverter
{
public:
    // using Super = IMIDIEventReceiver;

    std::pmr::vector<std::shared_ptr<NoteOnOff>> openNotes;

    void Convert(class MIDIMusic& music);

    bool operator()(std::shared_ptr<PMIDIEvent>& event);
};

