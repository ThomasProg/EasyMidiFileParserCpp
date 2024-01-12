#include "Converters/MIDIMusic_AbsoluteConverter.h"

#include <vector>
#include <memory>
#include "PMIDIEvent.h"
#include "MIDIMusic.h"

void MIDIMusic_AbsoluteConverter::Convert(class MIDIMusic& newMusic)
{
    for (MIDIMusic::TrackData& trackData : newMusic.tracks)
    {
        //for (const std::shared_ptr<PMIDIEvent>& ptr : trackData.midiEvents
        if (trackData.midiEvents.size() < 2)
            continue;

        for (std::vector<std::shared_ptr<PMIDIEvent>>::iterator it = trackData.midiEvents.begin() + 1; it != trackData.midiEvents.end(); ++it)
        {
            const std::shared_ptr<PMIDIEvent>& current = *it;
            const std::shared_ptr<PMIDIEvent>& previous = *(it - 1);

            current->deltaTime += previous->deltaTime;
        }
    }
}

void MIDIMusic_ConvertAbsolute(class MIDIMusic* music)
{
    MIDIMusic_AbsoluteConverter::Convert(*music);
}