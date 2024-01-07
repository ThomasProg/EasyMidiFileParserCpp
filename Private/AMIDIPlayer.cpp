#include "AMIDIPlayer.h"
#include "MIDIMusic.h"

uint16_t AMIDIPlayer::GetNbTracks() const
{
    return music == nullptr ? 0 : music->tracks.size();
}

void AMIDIPlayer::ExecuteEventsUntil(double currentTime /* in microseconds */)
{
    time = currentTime;

    for (int track = 0; track < GetNbTracks(); track++)
    {
        MIDIMusic::TrackData& tr = music->tracks[track];

        while (trackIndices[track] < tr.midiEvents.size())
        {
            PMIDIEvent& note = *tr.midiEvents[trackIndices[track]];
            double nextTime = trackLastEventTime[track] + (double(note.deltaTime * tempo) / music->ticksPerQuarterNote);
            if (nextTime < currentTime)
            {
                PMIDIEvent& note = *tr.midiEvents[trackIndices[track]];
                note.Execute(this);
                trackIndices[track]++;
                trackLastEventTime[track] = nextTime;
            }
            else
                break;
        }
    }
}
