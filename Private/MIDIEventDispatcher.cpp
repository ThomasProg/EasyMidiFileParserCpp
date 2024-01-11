#include "MIDIEventDispatcher.h"
#include "MIDIMusic.h"

void MIDIMusic_Dispatch(void* userData, MIDIEventCallbacks callbacks, class MIDIMusic* music)
{
    MIDIEventDispatcher dispatcher;
    dispatcher.userData = userData;
    dispatcher.callbacks = callbacks;

    for (auto& track : music->tracks)
    {
        for (auto& event : track.midiEvents)
        {
            event->Execute(&dispatcher);
        }
    }
}