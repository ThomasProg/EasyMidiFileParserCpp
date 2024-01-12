#pragma once

// #include ""
#include <vector>
#include <memory>
#include "PMIDIEvent.h"

// Convert NoteOn events and NoteOff events into NoteOnOff events
class MIDIPARSEREXPORT MIDIMusic_NoteOnOffConverter
{
public:
    bool operator()(std::shared_ptr<PMIDIEvent>& event);

    std::pmr::vector<std::shared_ptr<NoteOnOff>> openNotes;

    void Convert(class MIDIMusic& music);
};

extern "C"
{
    MIDIPARSEREXPORT void MIDIMusic_ConvertToNoteOnOff(class MIDIMusic* music);
}