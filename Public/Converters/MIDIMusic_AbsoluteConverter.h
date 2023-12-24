#pragma once

#include <vector>
#include <memory>
#include "PMIDIEvent.h"
#include "MIDIMusic.h"

// Converts deltatime from relative (delay since the last note) to absolute (delay since the beginning of the music)
// The unit is still ticks 
class MIDIMusic_AbsoluteConverter
{
public:
    static void Convert(class MIDIMusic& newMusic)
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
};

