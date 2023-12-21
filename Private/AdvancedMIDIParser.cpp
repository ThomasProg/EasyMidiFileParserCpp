#include "AdvancedMIDIParser.h"
#include <cassert>
#include <string>
#include <stdexcept>
#include <iostream>
#include "Buffers/BufferReader.h"

void AdvancedMIDIParser::OnTrackLoaded()
{
    currentTrackIndex++;
}

void AdvancedMIDIParser::OnFileHeaderDataLoaded(FileHeaderData& fileHeaderData) 
{
    if (nbTracks != fileHeaderData.nbTracks)
    {
        nbTracks = fileHeaderData.nbTracks;

        if (timePerTrack)
            delete[] timePerTrack;

        timePerTrack = new uint32_t[nbTracks];
        for (uint32_t i = 0u; i < nbTracks; i++)
        {
            timePerTrack[i] = 0;
        }
    }

    ticksPerQuarterNote = fileHeaderData.delta.ticksPerQuarterNote;
    currentTrackIndex = 0;
}

AdvancedMIDIParser::~AdvancedMIDIParser()
{
    if (timePerTrack)
        delete[] timePerTrack;
}

void AdvancedMIDIParser::AddTimeToTrack(int16_t trackIndex, uint32_t deltaTime)
{
    //timePerTrack[trackIndex] += deltaTitimePerTrack[currentTrackIndex]me;
    timePerTrack[currentTrackIndex] += deltaTime * (tempo / ticksPerQuarterNote);
}

void AdvancedMIDIParser::OnEvent(const PMIDIEvent& event)
{
    AddTimeToTrack(currentTrackIndex, event.deltaTime);
}