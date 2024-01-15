#include "MIDIPlayerAsync.h"
#include <chrono>
#include "AMIDIPlayer.h"

void MIDIPlayerAsync::Play()
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

void MIDIPlayerAsync::Stop()
{
    isPlaying = false;
}

bool MIDIPlayerAsync::IsPlaying() const
{
    return isPlaying.load();
}

MIDIPlayerAsync* MIDIPlayerAsync_Create()
{
    return new MIDIPlayerAsync();
}
void MIDIPlayerAsync_Destroy(MIDIPlayerAsync* player)
{
    delete player;
}

void MIDIPlayerAsync_SetPlayer(MIDIPlayerAsync* playerAsync, class AMIDIPlayer* internalPlayer)
{
    playerAsync->player = internalPlayer;
}
void MIDIPlayerAsync_Play(MIDIPlayerAsync* player)
{
    player->Play();
}
void MIDIPlayerAsync_Stop(MIDIPlayerAsync* player)
{
    player->Stop();
}
bool MIDIPlayerAsync_IsPlaying(MIDIPlayerAsync* player)
{
    return player->IsPlaying();
}