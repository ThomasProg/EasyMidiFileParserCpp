#pragma once

#include <vector>
#include <memory>
// #include "json/single_include/nlohmann/json.hpp"
#include "PMIDIEvent.h"

class MIDIPARSEREXPORT MIDIMusic
{
public:
    struct MIDIPARSEREXPORT TrackData
    {
        std::vector<std::shared_ptr<PMIDIEvent>> midiEvents;

        template<typename CALLBACK>
        void ForEachEvent(CALLBACK&& callback)
        {
            std::vector<std::shared_ptr<PMIDIEvent>>::iterator it = midiEvents.begin();
            while (it != midiEvents.end())
            {
                bool shouldBeRemoved = callback(*it);

                if (shouldBeRemoved)
                {
                    uint32_t removedEventDeltatime = (*it)->deltaTime;
                    it = midiEvents.erase(it);
                    if (it == midiEvents.end())
                        break;
                    (*it)->deltaTime += removedEventDeltatime;
                }
                else 
                {
                    it++;
                }
            }
        }
    };

    FileHeaderData fileHeaderData;
    std::vector<TrackData> tracks;

    int16_t ticksPerQuarterNote = 4; // is 4 the default ?

    int8_t sf = 0;
    uint8_t mi = 0;
    bool isTonalitySet = false;

    uint32_t GetDurationInTicks() const;
    double GetDurationInMicroseconds() const;
    uint32_t GetNbChannels() const;

    template<typename CALLBACK>
    void ForEachTrack(CALLBACK&& callback)
    {
        std::vector<TrackData>::iterator it = tracks.begin();
        while (it != tracks.end())
        {
            bool shouldBeRemoved = callback(*it);

            if (shouldBeRemoved)
            {
                it = tracks.erase(it);
            }
            else 
            {
                it++;
            }
        }
    }
};
