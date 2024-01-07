#include "Converters/MIDIMusic_InstrumentFilter.h"

void MIDIMusic_InstrumentFilter::Convert(class MIDIMusic& music)
{
    music.tracks[0].ForEachEvent([this](std::shared_ptr<PMIDIEvent>& elem)
    {
        if (ProgramChange* programChange = dynamic_cast<ProgramChange*>(elem.get()))
        {
            program = programChange->newProgram;
            return false;
        }
        else if (NoteOnOff* noteOnOff = dynamic_cast<NoteOnOff*>(elem.get()))
        {
            return (program >= start && program <= end);
        }
        return false;
    });
}