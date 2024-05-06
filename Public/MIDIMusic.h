#pragma once

#include <vector>
#include <memory>
#include <type_traits>
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

    int8_t sf = 0; // 0 = key of C, -1 = 1 flat, 1 = 1 sharp
    uint8_t mi = 0; // major or minor ?
    bool isTonalitySet = false;

    uint32_t GetDurationInTicks() const;
    double GetDurationInMicroseconds() const;
    uint32_t GetNbChannels() const;
    std::vector<uint32_t> GetProgramsList() const;

    template<typename T>
    requires std::is_base_of<PMIDIEvent, T>::value
    std::vector<std::shared_ptr<T>> GetProgramChangeList() const
    {
        std::vector<std::shared_ptr<T>> events;
        for (auto& track : tracks)
        {
            for (auto& e : track.midiEvents)
            {
                if (std::shared_ptr<T> event = dynamic_pointer_cast<T>(e))
                {
                    events.push_back(event);
                }
            }
        }
        return events;
    }

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

extern "C"
{
    MIDIPARSEREXPORT MIDIMusic* MIDIMusic_Create();
    MIDIPARSEREXPORT void MIDIMusic_Destroy(MIDIMusic* music);
    MIDIPARSEREXPORT MIDIMusic* MIDIMusic_Clone(MIDIMusic*);

    MIDIPARSEREXPORT void MIDIMusic_AddEvent(MIDIMusic* music, class PMIDIEvent* event);

    // MIDIPARSEREXPORT int32_t MIDIMusic_GetTempo(MIDIMusic* music);
    MIDIPARSEREXPORT int32_t MIDIMusic_GetTicksPerQuarterNote(MIDIMusic* music);
    MIDIPARSEREXPORT void MIDIMusic_SetTicksPerQuarterNote(MIDIMusic* music, int32_t ticksPerQuarterNote);
    // MIDIPARSEREXPORT int32_t MIDIMusic_GetBPM(MIDIMusic* music);
}
