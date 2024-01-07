#include "Converters/MIDIMusic_CompressorConverter.h"

void MIDIMusic_CompressorConverter::Convert(class MIDIMusic& newMusic)
{
    // Works better in absolute dt because of imprecision
    MIDIMusic_AbsoluteConverter::Convert(newMusic);

    ConvertUnsafe(newMusic);

    // Converts back
    MIDIMusic_RelativeConverter::Convert(newMusic);
}

void MIDIMusic_CompressorConverter::ConvertUnsafe(class MIDIMusic& newMusic)
{
    this->music = &newMusic;
    for (MIDIMusic::TrackData& trackData : newMusic.tracks)
    {
        trackData.ForEachEvent(*this);
    }
    newMusic.ticksPerQuarterNote = newTicksPerQuarterNote;

}

bool MIDIMusic_CompressorConverter::operator()(std::shared_ptr<PMIDIEvent>& event)
{
    event->deltaTime = OldToNewTime(event->deltaTime);

    if (NoteOnOff* noteOnOffPtr = dynamic_cast<NoteOnOff*>(event.get()))
    {
        NoteOnOff& noteOnOff = *noteOnOffPtr;
        noteOnOff.duration = OldToNewTime(noteOnOff.duration);
    }

    return false;
}