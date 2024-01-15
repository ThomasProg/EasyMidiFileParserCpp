#pragma once

#include <atomic>
#include "Macros.h"

// Async utilities
// Prevents std library from being included in AMIDIPlayer
// <atomic>, <mutex>, <chrono>Q
class MIDIPARSEREXPORT MIDIPlayerAsync
{
private:
    std::atomic<bool> isPlaying;

public:
    class AMIDIPlayer* player = nullptr;

public:
    virtual void Play();
    virtual void Stop();
    bool IsPlaying() const;
    void PlayAsync();
};

extern "C"
{
    MIDIPARSEREXPORT MIDIPlayerAsync* MIDIPlayerAsync_Create();
    MIDIPARSEREXPORT void MIDIPlayerAsync_Destroy(MIDIPlayerAsync* player);

    MIDIPARSEREXPORT void MIDIPlayerAsync_SetPlayer(AMIDIPlayer* playerAsync, class AMIDIPlayer* internalPlayer);
    MIDIPARSEREXPORT void MIDIPlayerAsync_Play(AMIDIPlayer* player);
    MIDIPARSEREXPORT void MIDIPlayerAsync_Stop(AMIDIPlayer* player);
    MIDIPARSEREXPORT bool MIDIPlayerAsync_IsPlaying(AMIDIPlayer* player);
}