#pragma once

#include "IMIDIEventReceiver.h"
#include <vector>

class MIDIPARSEREXPORT AMIDIPlayer : public IMIDIEventReceiver
{
private:
    std::vector<uint32_t> trackIndices;
    std::vector<double> trackLastEventTime;

    // only 3 bytes
    // microsecondsPerQuarterNote;
    uint32_t tempo = 500000; // 120 bpm by default (0.5s delay, so 0.5 * 1000 * 1000 microseconds)
    double time = 0.0;

public:
    using Super = IMIDIEventReceiver;

    class MIDIMusic* music = nullptr;
    uint16_t GetNbTracks() const;

    virtual void OnPrePlay()
    {
        trackIndices.resize(GetNbTracks());
        trackLastEventTime.resize(GetNbTracks());   
        time = 0.0;

        for (double& d : trackLastEventTime)
        {
            d = 0.0;
        }
    }

    void ExecuteEventsUntil(double currentTime /* in microseconds */);
    virtual void OnTempo(const Tempo& newTempo) override
    {
        Super::OnTempo(newTempo);
        tempo = newTempo.newTempo;
    }

    void PlayStep(double addedTime /* in microseconds */)
    {
        ExecuteEventsUntil(time + addedTime);
    }

    virtual ~AMIDIPlayer() = default;
};

MIDIPARSEREXPORT void AMIDIPlayer_SetMusic(AMIDIPlayer* player, class MIDIMusic* music);
MIDIPARSEREXPORT void AMIDIPlayer_PlayStep(AMIDIPlayer* player, double addedTime /* in microseconds */);