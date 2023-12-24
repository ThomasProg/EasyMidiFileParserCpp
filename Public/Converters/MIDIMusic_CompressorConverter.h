#pragma once

#include <vector>
#include <memory>
#include "PMIDIEvent.h"
#include "MIDIMusic.h"
#include "MIDIMusic_AbsoluteConverter.h"
#include "MIDIMusic_RelativeConverter.h"

// Reduces tick precision
class MIDIMusic_CompressorConverter
{
public:
    int16_t newTicksPerQuarterNote = 4; // is 4 the default ?

    MIDIMusic* music = nullptr;

    MIDIMusic_CompressorConverter(int16_t ticksPerQuarterNote) : newTicksPerQuarterNote(ticksPerQuarterNote)
    {

    }

    void Convert(class MIDIMusic& newMusic)
    {
        // Works better in absolute dt because of imprecision
        MIDIMusic_AbsoluteConverter::Convert(newMusic);

        this->music = &newMusic;
        for (MIDIMusic::TrackData& trackData : newMusic.tracks)
        {
            trackData.ForEachEvent(*this);
        }
        newMusic.ticksPerQuarterNote = newTicksPerQuarterNote;

        // Converts back
        MIDIMusic_RelativeConverter::Convert(newMusic);
    }

    uint32_t OldToNewTime(uint32_t oldTime)
    {
        return oldTime * newTicksPerQuarterNote / music->ticksPerQuarterNote;
    }

    bool operator()(std::shared_ptr<PMIDIEvent>& event)
    {
        event->deltaTime = OldToNewTime(event->deltaTime);

        if (typeid(*event) == typeid(NoteOnOff))
        {
            NoteOnOff& noteOnOff = *(NoteOnOff*)event.get();
            noteOnOff.duration = OldToNewTime(noteOnOff.duration);
        }

        return false;
    }
};

