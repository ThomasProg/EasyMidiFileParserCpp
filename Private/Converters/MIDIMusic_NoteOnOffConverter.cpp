#include "Converters/MIDIMusic_NoteOnOffConverter.h"
#include "MIDIMusic.h"
#include <stdexcept>

void MIDIMusic_NoteOnOffConverter::Convert(MIDIMusic& music)
{
    for (MIDIMusic::TrackData& trackData : music.tracks)
    {
        trackData.ForEachEvent(*this);
        openNotes.clear();
    }
}

bool MIDIMusic_NoteOnOffConverter::operator()(std::shared_ptr<PMIDIEvent>& event)
{
    for (std::shared_ptr<NoteOnOff>& noteOnOff : openNotes)
    {
        noteOnOff->duration += event->deltaTime;
    }

    if (NoteOn* noteOnPtr = dynamic_cast<NoteOn*>(event.get()))
    {
        NoteOn& noteOn = *noteOnPtr;

        std::shared_ptr<NoteOnOff> noteOnOff = std::make_shared<NoteOnOff>();
        
        noteOnOff->deltaTime = noteOn.deltaTime;
        noteOnOff->channel = noteOn.channel;
        noteOnOff->key = noteOn.key;
        noteOnOff->velocity = noteOn.velocity;

        openNotes.push_back(noteOnOff);

        event = std::move(noteOnOff);

        return false;
    }
    else if (NoteOff* noteOffPtr = dynamic_cast<NoteOff*>(event.get()))
    {
        NoteOff& noteOff = *noteOffPtr;

        auto isSame = [&noteOff](const std::shared_ptr<NoteOnOff>& noteOnOff) 
        {
            return noteOnOff->channel == noteOff.channel && noteOnOff->key == noteOff.key;
        };
        auto it = std::find_if(openNotes.begin(), openNotes.end(), isSame);
        if (it != openNotes.end()) // shouldn't be the case
        {
            NoteOnOff& noteOnOff = **it;
            openNotes.erase(it);
        }
        else 
        {
            throw std::runtime_error("NoteOff doesn't have corresponding NoteOn");
        }

        return true;
    }
    return false;
}

void MIDIMusic_ConvertToNoteOnOff(class MIDIMusic* music)
{
    MIDIMusic_NoteOnOffConverter().Convert(*music);
}