#include "Converters/MIDIMusic_RelativeConverter.h"

#include <vector>
#include <memory>
#include "PMIDIEvent.h"
#include "MIDIMusic.h"

void MIDIMusic_RelativeConverter::Convert(class MIDIMusic& newMusic)
{
    for (MIDIMusic::TrackData& trackData : newMusic.tracks)
    {
        if (trackData.midiEvents.size() < 2)
            continue;

        for (std::vector<std::shared_ptr<PMIDIEvent>>::reverse_iterator it = trackData.midiEvents.rbegin(); it != trackData.midiEvents.rend() - 1; ++it)
        {
            const std::shared_ptr<PMIDIEvent>& current = *it;
            const std::shared_ptr<PMIDIEvent>& previous = *(it + 1);

            current->deltaTime -= previous->deltaTime;
        }
    }
}

void MIDIMusic_ConvertRelative(class MIDIMusic* music)
{
    MIDIMusic_RelativeConverter::Convert(*music);
}