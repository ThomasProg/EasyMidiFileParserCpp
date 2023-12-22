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
    double time = 0.0;

public:
    virtual void Play()
    {
        player->OnPrePlay();
        isPlaying = true;

        std::chrono::time_point programBeginTime = std::chrono::high_resolution_clock::now();
        while (isPlaying.load())
        {
            auto frameStartTime = std::chrono::high_resolution_clock::now();

            time = std::chrono::duration<double, std::micro>(frameStartTime - programBeginTime).count();

            player->ExecuteEventsUntil(time);
        }
    }

    virtual void Stop()
    {
        isPlaying = false;
    }

    void PlayStep(double addedTime /* in microseconds */)
    {
        player->ExecuteEventsUntil(time + addedTime);
    }
};

