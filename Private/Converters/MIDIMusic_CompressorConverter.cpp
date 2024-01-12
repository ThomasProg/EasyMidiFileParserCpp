#include "Converters/MIDIMusic_CompressorConverter.h"

#include <memory>
#include "PMIDIEvent.h"
#include "MIDIMusic.h"
#include "Converters/MIDIMusic_AbsoluteConverter.h"
#include "Converters/MIDIMusic_RelativeConverter.h"

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
        trackData.ForEachEvent([this](std::shared_ptr<PMIDIEvent>& event)
        {
            event->deltaTime = OldToNewTime(event->deltaTime);

            if (NoteOnOff* noteOnOffPtr = dynamic_cast<NoteOnOff*>(event.get()))
            {
                NoteOnOff& noteOnOff = *noteOnOffPtr;
                noteOnOff.duration = OldToNewTime(noteOnOff.duration);
            }

            return false;
        });
    }
    newMusic.ticksPerQuarterNote = newTicksPerQuarterNote;
}

uint32_t MIDIMusic_CompressorConverter::OldToNewTime(uint32_t oldTime) const
{
    return oldTime * newTicksPerQuarterNote / music->ticksPerQuarterNote;
}

void MIDIMusic_Compress(class MIDIMusic* music, int16_t newTicksPerQuarterNote)
{
    MIDIMusic_CompressorConverter(newTicksPerQuarterNote).Convert(*music);
}