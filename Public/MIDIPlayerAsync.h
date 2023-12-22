#pragma once

#include <chrono>
#include <atomic>
#include "AMIDIPlayer.h"

// Async utilities
// Prevents std library from being included in AMIDIPlayer
// <atomic>, <mutex>, <chrono>
class MIDIPARSEREXPORT MIDIPlayerAsync
{
public:
    AMIDIPlayer* player = nullptr;
    std::atomic<bool> isPlaying;

public:
    virtual void Play()
    {
        player->OnPrePlay();
        isPlaying = true;

        std::chrono::time_point programBeginTime = std::chrono::high_resolution_clock::now();
        while (isPlaying.load())
        {
            auto frameStartTime = std::chrono::high_resolution_clock::now();

            double time = std::chrono::duration<double, std::micro>(frameStartTime - programBeginTime).count();

            player->ExecuteEventsUntil(time);
        }
    }

    virtual void Stop()
    {
        isPlaying = false;
    }
};

