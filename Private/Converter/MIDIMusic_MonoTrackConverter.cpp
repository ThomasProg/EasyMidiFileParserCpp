#include "Converters/MIDIMusic_MonoTrackConverter.h"

void MIDIMusic_MonoTrackConverter::Convert(class MIDIMusic& newMusic)
{
    if (newMusic.tracks.size() == 1)
        return;

    // Works better in absolute dt because of imprecision
    MIDIMusic_AbsoluteConverter::Convert(newMusic);

    ConvertUnsafe(newMusic);

    // Converts back
    MIDIMusic_RelativeConverter::Convert(newMusic);
}

void MIDIMusic_MonoTrackConverter::ConvertUnsafe(class MIDIMusic& newMusic)
{
    std::map<uint32_t /*time*/, std::shared_ptr<PMIDIEvent>> map;

    for (MIDIMusic::TrackData& trackData : newMusic.tracks)
    {
        trackData.ForEachEvent([&map](std::shared_ptr<PMIDIEvent>& elem)
        {
            map.emplace(elem->deltaTime, elem);
            return false;
        });
    }

    newMusic.tracks.clear();
    
    MIDIMusic::TrackData& trackData = newMusic.tracks.emplace_back();
    for (auto& [time, event] : map)
    {
        trackData.midiEvents.push_back(event);
    }
    // newMusic.tracks.shrink_to_fit(); // space opti, probably not gonna create more tracks in the future
}