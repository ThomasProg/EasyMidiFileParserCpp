#include "AMIDIPlayer.h"
#include "MIDIMusic.h"

uint16_t AMIDIPlayer::GetNbTracks() const
{
    return music->tracks.size();
}

void AMIDIPlayer::ExecuteEventsUntil(double currentTime /* in microseconds */)
{
    time = currentTime;

    for (int track = 0; track < GetNbTracks(); track++)
    {
        MIDIMusic::TrackData& tr = music->tracks[track]; 

        double timeSinceLastEvent = currentTime - trackLastEventTime[track];
        uint32_t tickSinceLastEvent = timeSinceLastEvent * music->ticksPerQuarterNote / double(tempo);

        if (trackIndices[track] < tr.midiEvents.size() && tr.midiEvents[trackIndices[track]]->deltaTime <= tickSinceLastEvent)
        {
            PMIDIEvent& note = *tr.midiEvents[trackIndices[track]];
            note.Execute(this);
            trackIndices[track]++;
            trackLastEventTime[track] += (double(note.deltaTime * tempo) / music->ticksPerQuarterNote);

            timeSinceLastEvent = currentTime - trackLastEventTime[track];
            tickSinceLastEvent = timeSinceLastEvent * music->ticksPerQuarterNote / double(tempo);
        }
    }
}