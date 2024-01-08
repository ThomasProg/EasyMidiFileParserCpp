#include "Converters/MIDIMusic_ChannelFilter.h"

#include "MIDIMusic.h"
#include "PMIDIEvent.h"

void MIDIMusic_ChannelFilter::Convert(class MIDIMusic& music)
{
    if (music.tracks.size() != 1)
        return;

    for (auto& track : music.tracks)
    {
        track.ForEachEvent([&](std::shared_ptr<PMIDIEvent>& elem)
        {
            if (const ChannelEvent* channelEvent = dynamic_cast<const ChannelEvent*>(elem.get()))
            {
                return (channelEvent->channel == filteredChannel) == filterOut;
            }
            return false;
        });
    }
}