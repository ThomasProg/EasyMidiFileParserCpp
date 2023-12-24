#pragma once

#include <vector>
#include <memory>
#include "PMIDIEvent.h"
#include "MIDIMusic.h"

// Converts deltatime from absolute (delay since the beginning of the music) to relative (delay since the last note)
// The unit is still ticks 
class MIDIMusic_RelativeConverter
{
public:
    static void Convert(class MIDIMusic& newMusic)
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
};

