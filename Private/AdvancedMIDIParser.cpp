#include "AdvancedMIDIParser.h"
#include <cassert>
#include <string>
#include <stdexcept>
#include <iostream>

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
    if (sysEvent.type == ESysEvent::SONG_SELECT)
    {
        assert(sysEvent.messageLength == 1);
        uint8_t songNumber = * (uint8_t*) sysEvent.message;
    }
} 

void AdvancedMIDIParser::OnMetaEventLoaded(uint32_t deltaTime, MetaEvent& metaEvent) 
{
    timePerTrack[currentTrackIndex] += deltaTime;

    switch (metaEvent.type)
    {
        case EMidiMeta::SET_TEMPO:
            assert(metaEvent.length == 3);
            tempo = (metaEvent.bytes[0] << 16) + (metaEvent.bytes[1] << 8) + metaEvent.bytes[2];
            break;

        case EMidiMeta::TIME_SIGNATURE:
        {
            assert(metaEvent.length == 4);
            int nominator = metaEvent.bytes[0];
            int denominator = 2 << metaEvent.bytes[1];
            int clocks = metaEvent.bytes[2];
            int notes = metaEvent.bytes[3];
            break;
        }

        case EMidiMeta::KEY_SIGNATURE:
            {
            assert(metaEvent.length == 2);
            int8_t sf = (int8_t) metaEvent.bytes[0]; // 0 = key of C, -1 = 1 flat, 1 = 1 sharp
            uint8_t mi = metaEvent.bytes[1]; // major or minor ?

            sf = std::byteswap(sf); // TODO : only if machine is little endian

            OnKeySignature(sf, mi);
            }
            break;

        case EMidiMeta::MIDI_PORT:
            break;

        case EMidiMeta::END_OF_TRACK:
            break;

        case EMidiMeta::TEXT:
            OnText((const char*)metaEvent.bytes, metaEvent.length);
            break;

        case EMidiMeta::COPYRIGHT:
            OnCopyright((const char*)metaEvent.bytes, metaEvent.length);
            break;

        case EMidiMeta::TRACK_NAME:
            OnTrackName((const char*)metaEvent.bytes, metaEvent.length);
            break;

        case EMidiMeta::INSTRUMENT_NAME:
            OnInstrumentName((const char*)metaEvent.bytes, metaEvent.length);
            break;

        case EMidiMeta::LYRICS:
            OnLyric((const char*)metaEvent.bytes, metaEvent.length);
            break;

        case EMidiMeta::MARKER:
            OnMarker((const char*)metaEvent.bytes, metaEvent.length);
            break;

        case EMidiMeta::CUE_POINT:
            OnCuePoint((const char*)metaEvent.bytes, metaEvent.length);
            break;


        default:
            throw std::runtime_error(std::string("OnMetaEventLoaded/") + std::to_string((int)metaEvent.type) );
            break;    
    }
}

void assertControlChange(MIDIParser::EControlChange cc, int param)
{
    assert(param >= 0);

    uint8_t ccValue = (int) cc;

    if (ccValue > 127)
    {
        throw std::runtime_error("Invalid Control Change Value : " + std::to_string(ccValue) + " (Higher than 127)");
    }

    if (ccValue <= 119)
    {
        assert(param <= 127);
    }
    else if (ccValue == 122)
    {
        assert(param == 0 || param == 127);
    }
    else if (ccValue == 126)
    {
        assert(false); // not currently done
    }
    else
    {
        if (param != 0)
        {
            std::cout << "ERROR : " << (int) ccValue << std::endl;
        }
        assert(param == 0);
    }
}

void AdvancedMIDIParser::OnChannelEventLoaded(uint32_t deltaTime, ChannelEvent& channelEvent, bool isOpti) 
{
    timePerTrack[currentTrackIndex] += deltaTime;

    switch (channelEvent.message)
    {
        case ENoteEvent::NOTE_ON:
            // if (!isOpti)
            //     byteToDataStr.emplace(debugBufferPtr->Data() - 3, "NoteOn");
            byteToDataStr.emplace(debugBufferPtr->Data() - 2, "NoteOn:Key");
            byteToDataStr.emplace(debugBufferPtr->Data() - 1, "NoteOn:Velocity");
            OnNoteOn(channelEvent.channel, channelEvent.param1, channelEvent.param2);
            break;

        case ENoteEvent::NOTE_OFF:
            // if (!isOpti)
            //     byteToDataStr.emplace(debugBufferPtr->Data() - 2, "NoteOff");
            byteToDataStr.emplace(debugBufferPtr->Data() - 1, "NoteOff:Key");
            OnNoteOff(channelEvent.channel, channelEvent.param1);
            break;

        case ENoteEvent::PGM_CHANGE:
            // if (!isOpti)
            //     byteToDataStr.emplace(debugBufferPtr->Data() - 2, "ProgramChange");
            byteToDataStr.emplace(debugBufferPtr->Data() - 1, "ProgramChange:ID");
            OnProgramChange(channelEvent.channel, channelEvent.param1);
            break;

        case ENoteEvent::CONTROL_CHANGE:
            // if (!isOpti)
            //     byteToDataStr.emplace(debugBufferPtr->Data() - 3, "ControlChange");
            byteToDataStr.emplace(debugBufferPtr->Data() - 2, "ControlChange:Setting");
            byteToDataStr.emplace(debugBufferPtr->Data() - 1, "ControlChange:Value");
            assertControlChange((EControlChange) channelEvent.param1, channelEvent.param2);
            OnControlChange(channelEvent.channel, (EControlChange) channelEvent.param1, channelEvent.param2);
            break;

        case ENoteEvent::PITCH_BEND:
            OnPitchBend(channelEvent.channel, channelEvent.param1);
            break;

        case ENoteEvent::NOTE_AFTER_TOUCH:
            break;

        case ENoteEvent::CHANNEL_AFTER_TOUCH:
            break;

        default:
            throw std::runtime_error(std::string("OnChannelEventLoaded/") + std::to_string((int)channelEvent.message) + " / Opti : " + std::to_string(isOpti));
            break;
    }
}