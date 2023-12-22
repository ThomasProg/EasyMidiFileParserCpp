#pragma once

// #include ""
#include <vector>
#include <memory>
#include "PMIDIEvent.h"

// Convert NoteOn events and NoteOff events into NoteOnOff events
class MIDIMusic_NoteOnOffConverter
{
public:
    // using Super = IMIDIEventReceiver;

    std::vector<std::shared_ptr<NoteOnOff>> openNotes;

    void Convert(class MIDIMusic& music)
    {
        for (MIDIMusic::TrackData& trackData : music.tracks)
        {
            trackData.ForEachEvent(*this);
            openNotes.clear();
        }

        openNotes.clear();
    }

    bool operator()(std::shared_ptr<PMIDIEvent>& event)
    {
        for (std::shared_ptr<NoteOnOff>& noteOnOff : openNotes)
        {
            noteOnOff->duration += event->deltaTime;
        }

        if (typeid(*event) == typeid(NoteOn))
        {
            NoteOn& noteOn = *(NoteOn*)event.get();

            std::shared_ptr<NoteOnOff> noteOnOff = std::make_shared<NoteOnOff>();;
            
            noteOnOff->deltaTime = noteOn.deltaTime;
            noteOnOff->channel = noteOn.channel;
            noteOnOff->key = noteOn.key;
            noteOnOff->velocity = noteOn.velocity;

            openNotes.push_back(noteOnOff);

            event = std::move(noteOnOff);

            return false;
        }
        else if (typeid(*event) == typeid(NoteOff))
        {
            NoteOff& noteOff = *(NoteOff*)event.get();

            auto isSame = [&noteOff](const std::shared_ptr<NoteOnOff>& noteOnOff) 
            {
                return noteOnOff->channel == noteOff.channel && noteOnOff->key == noteOff.key;
            };
            auto it = std::find_if(openNotes.begin(), openNotes.end(), isSame);
            NoteOnOff& noteOnOff = **it;
            openNotes.erase(it);

            return true;
        }
        return false;
    }
};

