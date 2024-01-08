#include "Converters/MIDIMusic_InstrumentFilter.h"
#include <unordered_map>

#include "MIDIMusic.h"
#include "PMIDIEvent.h"

void MIDIMusic_InstrumentFilter::Convert(class MIDIMusic& music)
{
    if (music.tracks.size() != 1)
        return;

    std::unordered_map<uint32_t, uint32_t> channelToProgram;
    music.tracks[0].ForEachEvent([&](std::shared_ptr<PMIDIEvent>& elem)
    {
        if (const ProgramChange* programChange = dynamic_cast<const ProgramChange*>(elem.get()))
        {
            channelToProgram[programChange->channel] = programChange->newProgram; 
            // program = programChange->newProgram;// TODO : Depends on channel
            return false;
        }
        else if (const NoteOnOff* noteOnOff = dynamic_cast<NoteOnOff*>(elem.get()))
        {
            const uint32_t program = channelToProgram[noteOnOff->channel];
            return (program >= start && program <= end) == filterOut;
        }
        return false;
    });
}