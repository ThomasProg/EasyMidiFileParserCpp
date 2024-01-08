#include "MIDIMusic.h"
#include <map>

uint32_t MIDIMusic::GetDurationInTicks() const
{
    uint32_t maxTicks = 0;
    for (auto& track : tracks)
    {
        uint32_t trackNbTicks = 0;
        for (auto& e : track.midiEvents)
        {
            trackNbTicks += e->deltaTime;
        }

        maxTicks = std::max(maxTicks, trackNbTicks);
    }
    return maxTicks;
}

double MIDIMusic::GetDurationInMicroseconds() const
{
    std::map<uint32_t, uint32_t> tickTimeToNewTempo;
    
    uint32_t maxTicks = 0;
    for (auto& track : tracks)
    {
        uint32_t trackNbTicks = 0;
        for (auto& e : track.midiEvents)
        {
            trackNbTicks += e->deltaTime;

            if (Tempo* tempo = dynamic_cast<Tempo*>(e.get()))
            {
                tickTimeToNewTempo.emplace(trackNbTicks, tempo->newTempo);
            }
        }

        maxTicks = std::max(maxTicks, trackNbTicks);
    }

    // Compute time from start to last Tempo Event
    double totalTime = 0.0; // microseconds / quarter note
    double currentTempo = 0.5 * 1000.0 * 1000.0; // microseconds / quarter note
    uint32_t lastTickTime = 0;
    for (auto& [ticks, tempo] : tickTimeToNewTempo)
    {
        const double tickDuration = currentTempo / ticksPerQuarterNote; // in microseconds
        const uint32_t deltaTickTime = ticks - lastTickTime;
        const double deltaTimeSinceLastTempoEvent = deltaTickTime * tickDuration; // in microseconds

        totalTime += deltaTimeSinceLastTempoEvent;

        currentTempo = tempo;
        lastTickTime = ticks;
    }

    // Compute time from last Tempo Event to end
    {
        const double tickDuration = currentTempo / ticksPerQuarterNote; // in microseconds
        const uint32_t deltaTickTime = maxTicks - lastTickTime;
        const double deltaTimeSinceLastTempoEvent = deltaTickTime * tickDuration; // in microseconds

        totalTime += deltaTimeSinceLastTempoEvent;
    }

    return totalTime;
}

#include <set>
uint32_t MIDIMusic::GetNbChannels() const
{
    std::set<uint32_t> channels;
    for (auto& track : tracks)
    {
        for (auto& e : track.midiEvents)
        {
            if (PMIDIChannelEvent* channelEvent = dynamic_cast<PMIDIChannelEvent*>(e.get()))
            {
                channels.emplace(channelEvent->channel);
            }
        }
    }
    return channels.size();
}