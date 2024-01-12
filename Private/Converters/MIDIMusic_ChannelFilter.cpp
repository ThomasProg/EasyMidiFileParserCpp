#include "Converters/MIDIMusic_ChannelFilter.h"

#include "MIDIMusic.h"
#include "PMIDIEvent.h"

void MIDIMusic_ChannelFilter::Convert(class MIDIMusic& music)
{
    for (auto& track : music.tracks)
    {
        track.ForEachEvent([this](std::shared_ptr<PMIDIEvent>& elem)
        {
            if (const PMIDIChannelEvent* channelEvent = dynamic_cast<const PMIDIChannelEvent*>(elem.get()))
            {
                return (channelEvent->channel == filteredChannel) == filterOut;
            }
            return false;
        });
    }
}

void MIDIMusic_FilterChannel(class MIDIMusic* music, uint32_t filteredChannel, bool filterOut)
{
    MIDIMusic_ChannelFilter(filteredChannel, filterOut).Convert(*music);
}
