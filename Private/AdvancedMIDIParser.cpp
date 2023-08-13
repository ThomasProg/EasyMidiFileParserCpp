#include "AdvancedMIDIParser.h"
#include <cassert>
#include <string>
#include <stdexcept>

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

void AdvancedMIDIParser::OnSysEventLoaded(uint32_t deltaTime, SysexEvent& sysEvent) 
{
    timePerTrack[currentTrackIndex] += deltaTime;
} 

void AdvancedMIDIParser::OnMetaEventLoaded(uint32_t deltaTime, MetaEvent& metaEvent) 
{
    timePerTrack[currentTrackIndex] += deltaTime;

    switch (metaEvent.type)
    {
        case EMidiMeta::SET_TEMPO:

            tempo = (metaEvent.bytes[0] << 16) + (metaEvent.bytes[1] << 8) + metaEvent.bytes[2];
            break;

        case EMidiMeta::TIME_SIGNATURE:
        {
            assert(metaEvent.length == 4);
            int nominator = metaEvent.bytes[0];
            int denominator = 2 << metaEvent.bytes[1];
            int clocks = metaEvent.bytes[2];
            int notes = metaEvent.bytes[3];
        }
            break;

        case EMidiMeta::END_OF_TRACK:
            break;

        case EMidiMeta::TRACK_NAME:
        {
            std::string trackName((char*)metaEvent.bytes, metaEvent.length);
            break;
        }

        default:
            throw std::runtime_error( MidiMetaToStr(metaEvent.type) );
            break;    
    }
}

void AdvancedMIDIParser::OnChannelEventLoaded(uint32_t deltaTime, ChannelEvent& channelEvent, bool isOpti) 
{
    timePerTrack[currentTrackIndex] += deltaTime;

    switch (channelEvent.message)
    {
        case ENoteEvent::NOTE_ON:
            OnNoteOn(channelEvent.channel, channelEvent.param1, channelEvent.param2);
            break;

        case ENoteEvent::NOTE_OFF:
            OnNoteOff(channelEvent.channel, channelEvent.param1);
            break;

        case ENoteEvent::PGM_CHANGE:
            OnProgramChange(channelEvent.channel, channelEvent.param1);
            break;

        case ENoteEvent::CONTROL_CHANGE:
            OnControlChange(channelEvent.channel, channelEvent.param1, channelEvent.param2);
            break;

        case ENoteEvent::PITCH_BEND:
            OnPitchBend(channelEvent.channel, channelEvent.param1);
            break;

        default:
            throw std::runtime_error( ENoteEventToStr(channelEvent.message) );
            break;
    }
}