#include "MIDIParserBase.h"

#include <fstream>
#include <assert.h>
#include <algorithm>
#include <utility>
#include <bit>
#include <string>
#include <iostream>
#include <cstring> // memcpy
#include "MIDIParserException.h"
#include "EndianHelpers.h"
#include "Buffers/BufferReader.h"
#include "IMIDIEventReceiver.h"

void MIDIParserBase::Reduce(size_t& size, size_t reduced)
{
    if (size >= reduced)
    {
        //size -= reduced;
    }
    else 
    {
        throw std::runtime_error("Can't reduce anymore");
    }
}

const char* MIDIParserBase::GetDataStrFromByte(void* byte) const
{
    return byteToDataStr.find(byte)->second.c_str();
}

const FileHeader MIDIParserBase::LoadFileHeaderFromBuffer(BufferReader& buffer)
{
    FileHeader header = * (FileHeader*) buffer.Consume(sizeof(char) * 4 + sizeof(uint32_t));
    //header.name = *(const char*)buffer.Consume(sizeof(char) * 4);
    //header.size = *(uint32_t*)buffer.Consume(sizeof(uint32_t));
    BigToNativeEndian(header.size);

    if (memcmp(header.name, "MThd", 4))
    {
        byteToDataStr[buffer.Data()] = "Header name is not MThd";
        throw std::runtime_error(std::string("Header name is not MThd : ") + header.name);
    }
    // assert(!memcmp(header.name, "MThd", 4)); 

    if (header.size != 6)
    {
        throw std::runtime_error("Header name size is not 6"); 
    }
    // assert(header.size == 6);

    return header;
}

FileHeaderData MIDIParserBase::LoadFileHeaderDataFromBuffer(BufferReader& buffer)
{
    FileHeaderData fileHeaderData;

    fileHeaderData.format = *(uint16_t*)buffer.Consume(sizeof(uint16_t));
    BigToNativeEndian(fileHeaderData.format);

    fileHeaderData.nbTracks = *(uint16_t*)buffer.Consume(sizeof(uint16_t));
    BigToNativeEndian(fileHeaderData.nbTracks);

    int8_t* division = (int8_t*) buffer.Consume(sizeof(uint8_t) * 2);

    fileHeaderData.isTicksPerFrame = division[0] < 0; 
    if(fileHeaderData.isTicksPerFrame)
    {
        fileHeaderData.delta.perFrame.SMPTEFormat = -(signed char)division[0];
        fileHeaderData.delta.perFrame.ticksPerFrame = (unsigned) division[1];
    }
    else
    {
        // TODO : fix?
        fileHeaderData.delta.ticksPerQuarterNote = ((unsigned)division[0] << 8) | ((unsigned)division[1] & 0xff);

        // BigToNativeEndian(division);
        // fileHeaderData.delta.ticksPerQuarterNote = division;
    }


    // fileHeaderData.divi = * (uint16_t*) bytes;
    // bytes += sizeof(uint16_t);

    // BigToNativeEndian(data->division);

    // Files with format 0 MUST have only 1 track
    if (fileHeaderData.format == 0 && fileHeaderData.nbTracks != 1)
    {
        throw std::runtime_error("MIDI file with format 0 should have only 1 track (instead of " + std::to_string(fileHeaderData.nbTracks) + ")");
    }
    // assert(fileHeaderData.format != 0 || fileHeaderData.nbTracks == 1);

    return fileHeaderData;
}

uint32_t ReadVarLen(BufferReader& buffer)
{
    uint32_t value;
    unsigned char c;

    if ( (value = *buffer.Consume(sizeof(uint8_t))) & 0x80 )
    {
        value &= 0x7F;
        do
        {
            value = (value << 7) + ((c = *buffer.Consume(sizeof(uint8_t))) & 0x7F);
        } while (c & 0x80);
    }

    return value;
}

const char* MIDIParserBase::MidiMetaToStr(const EMidiMeta& midiMeta)
{
    switch (midiMeta) 
    {
        case EMidiMeta::SEQ_NUM: return "Sequence Number";
        case EMidiMeta::TEXT: return "Text";
        case EMidiMeta::COPYRIGHT: return "Copyright";
        case EMidiMeta::TRACK_NAME: return "Track Name";
        case EMidiMeta::INSTRUMENT_NAME: return "Instrument Name";
        case EMidiMeta::LYRICS: return "Lyrics";
        case EMidiMeta::MARKER: return "Marker";
        case EMidiMeta::CUE_POINT: return "Cue Point";
        case EMidiMeta::CHANNEL_PREFIX: return "Channel Prefix";
        case EMidiMeta::MIDI_PORT: return "MIDI Port";
        case EMidiMeta::END_OF_TRACK: return "End of Track";
        case EMidiMeta::SET_TEMPO: return "Set Tempo";
        case EMidiMeta::SMPTE_OFFSET: return "SMPTE Offset";
        case EMidiMeta::TIME_SIGNATURE: return "Time Signature";
        case EMidiMeta::KEY_SIGNATURE: return "Key Signature";
        case EMidiMeta::SEQ_SPECIFIC: return "Sequencer Specific";

        default: 
            //throw std::runtime_error("MIDI invalid Meta Event: " + std::to_string((int)midiMeta)); 
            return "(unknown)";
    }
}

const char* MIDIParserBase::SysEventToStr(const ESysEvent& sysEvent)
{
    switch (sysEvent) 
    {
        case ESysEvent::SYSTEM_EXCLUSIVE: return "System Exclusive";

        case ESysEvent::MIDI_TIMING_CODE: return "MIDI Timing Code";
        case ESysEvent::SONG_POSITION_POINTER: return "Song Position Pointer";
        case ESysEvent::SONG_SELECT: return "Song Select";
        case ESysEvent::TUNE_REQUEST: return "Tune Request";
        case ESysEvent::ESCAPE_EVENT: return "Escape Event (or continuation)";

        case ESysEvent::TIMING_CLOCK: return "Timing Clock";
        case ESysEvent::START_SEQUENCE: return "Start Sequence";
        case ESysEvent::CONTINUE_SEQUENCE: return "Continue Sequence";
        case ESysEvent::STOP_SEQUENCE: return "Stop Sequence";
        case ESysEvent::ACTIVE_SENSING: return "Active Sensing";
        case ESysEvent::SYSTEM_RESET: return "System reset";

        default: throw std::runtime_error("MIDI invalid Sys Event: " + std::to_string((int)sysEvent)); return "(unknown)";
    }
}

const char* MIDIParserBase::ENoteEventToStr(const ENoteEvent& noteEvent)
{
    switch (noteEvent) {
        case ENoteEvent::NOTE_OFF: return "Note Off";
        case ENoteEvent::NOTE_ON: return "Note On";
        case ENoteEvent::NOTE_AFTER_TOUCH: return "Note Aftertouch";
        case ENoteEvent::CONTROL_CHANGE: return "Control Change";
        case ENoteEvent::PGM_CHANGE: return "Program Change";
        case ENoteEvent::CHANNEL_AFTER_TOUCH: return "Channel Aftertouch";
        case ENoteEvent::PITCH_BEND: return "Pitch Bend";

        default: throw std::runtime_error("MIDI invalid Note Event: " + std::to_string((int)noteEvent)); return "(unknown)";
    }
}

MetaEvent MIDIParserBase::LoadMetaEventFromBuffer(BufferReader& buffer)
{
    MetaEvent metaEvent;
    metaEvent.type = *(EMidiMeta*)buffer.Consume(sizeof(EMidiMeta));
    byteToDataStr[buffer.Data() - sizeof(EMidiMeta)] = MidiMetaToStr(metaEvent.type);

    byteToDataStr[buffer.Data()] = "Meta Event Length";
    metaEvent.length = ReadVarLen(buffer);

    // length is unsigned, so it will always be greater than 0
    // assert(metaEvent.length >= 0);

    for (uint32_t i = 0; i < metaEvent.length; i++)
    {
        byteToDataStr[buffer.Data() + i] = "Meta Event Data";
    }
    metaEvent.bytes = (uint8_t*) buffer.Consume(sizeof(uint8_t) * metaEvent.length);

    return metaEvent;
}

SysexEvent MIDIParserBase::LoadSysexEventFromBuffer(const MessageStatus& status, BufferReader& buffer)
{
    SysexEvent sysEvent;
    sysEvent.type = (ESysEvent) status.value;

    sysEvent.messageLength = ReadVarLen(buffer);
    sysEvent.message = (const char*) buffer.Consume(sizeof(char) * sysEvent.messageLength);
    return sysEvent;
}

ChannelEvent MIDIParserBase::LoadChannelEventDataFromBuffer(MessageStatus& midiEvent, BufferReader& buffer)
{
    ChannelEvent channelEvent;

    channelEvent.message = midiEvent.GetMessage();

    int8_t channelID = midiEvent.GetChannel();
    BigToNativeEndian(channelID);
    channelEvent.channel = channelID; 

    int dataLength = GetMIDIEventDataLength(channelEvent.message);

    if (dataLength > 0)
    {
        byteToDataStr[buffer.Data()] = "Param1";
        channelEvent.param1 = *buffer.Consume(sizeof(uint8_t));
    }
    // channelEvent.param1 = (dataLength > 0 ? buffer.Data()[0] : 0);

    if (channelEvent.message == ENoteEvent::CONTROL_CHANGE)
    {
        dataLength = 2;
    }

    if (dataLength > 1)
    {
        byteToDataStr[buffer.Data()] = "Param2";
        channelEvent.param2 = *buffer.Consume(sizeof(uint8_t));
    }
    else channelEvent.param2 = 0;

    // channelEvent.param2 = (dataLength > 1 ? buffer.Data()[1] : 0);

    // buffer.Consume(sizeof(uint8_t) * dataLength);

    return channelEvent;
}

TrackHeader MIDIParserBase::LoadTrackHeaderFromBuffer(BufferReader& buffer)
{
    TrackHeader trackChunk = * (TrackHeader*)buffer.Consume(sizeof(TrackHeader));
    BigToNativeEndian(trackChunk.length);

    for (size_t i = 0; i < sizeof(uint32_t); i++)
    {
        byteToDataStr[buffer.Data() - sizeof(TrackHeader) + i] = std::string("TrackHeader::Type = ") + std::to_string(trackChunk.type);
    }
    for (size_t i = 0; i < sizeof(uint32_t); i++)
    {
        byteToDataStr[buffer.Data() - sizeof(TrackHeader) + sizeof(uint32_t) + i] = std::string("TrackHeader::Length = ") + std::to_string(trackChunk.length);
    }

    return trackChunk;
}
void MIDIParserBase::LoadTrackFromBuffer(BufferReader& buffer, MessageStatus& runtimeStatus)
{
    TrackHeader trackChunk = LoadTrackHeaderFromBuffer(buffer);
    observer->OnTrackHeaderLoaded(trackChunk);

    if (trackChunk.length == 0)
        return;

    const uint8_t* startBytes = buffer.Data();

    // http://midi.teragonaudio.com/tech/midifile/evts.htm

    for (size_t j = 0; buffer.Data() < startBytes + trackChunk.length && j < trackChunk.length; j++)
    {
        byteToDataStr[buffer.Data()] = "DeltaTime";
        uint32_t deltaTime = ReadVarLen(buffer);
        //if ((startBytes + trackChunk.length) - buffer.Data() <= 0)
        //{
        //    //bytes = startBytes + trackChunk.length;
        //    break;
        //}

        byteToDataStr[buffer.Data()] = "Status";

        MessageStatus* status = (MessageStatus*) buffer.Consume(sizeof(MessageStatus));

        //if ((startBytes + trackChunk.length) - buffer.Data() <= 0)
        //{
        //    bytes = startBytes + trackChunk.length;
        //    break;
        //}

        if (!status->IsValidStatusEvent()) [[likely]] // If not Status Byte
        {
            byteToDataStr[buffer.Data() - sizeof(MessageStatus)] = "ChannelEvent : opti";

            buffer.GoBack(sizeof(MessageStatus));

            // // Use previous status byte
            // if (!runtimeStatus.IsValid())
            // {
            //     throw MIDIParserException(*this, "Invalid runtime status", (const char*) buffer.Data());
            // }
            // assert(runtimeStatus.IsValid());
            ChannelEvent channelEvent = LoadChannelEventDataFromBuffer(runtimeStatus, buffer);
            try 
            {
                observer->OnChannelEventLoaded(deltaTime, channelEvent, true);
            } catch (const std::exception& e)
            {
                throw MIDIParserException(*this, e.what(), (const char*) buffer.Data());
            } 
        }
        else if (status->IsSysExEvent()) [[unlikely]]
        {
            byteToDataStr[buffer.Data() - sizeof(MessageStatus)] = "SysExEvent";

            // runtimeStatus.Reset();
            SysexEvent sysEvent = LoadSysexEventFromBuffer(*status, buffer);
            observer->OnSysEventLoaded(deltaTime, sysEvent);
        }
        else if (status->IsMetaEvent()) [[unlikely]] // Non-MIDI events / RESET byte
        {
            // runtimeStatus.Reset();
            byteToDataStr[buffer.Data() - sizeof(MessageStatus)] = "MetaEvent";

            // parse meta event
            MetaEvent metaEvent = LoadMetaEventFromBuffer(buffer);
            observer->OnMetaEventLoaded(deltaTime, metaEvent);

            if (metaEvent.type == EMidiMeta::END_OF_TRACK)
                break;
        }
        else [[likely]] // If normal status byte
        {
            byteToDataStr[buffer.Data() - sizeof(MessageStatus)] = "ChannelEvent";

            // Update status byte
            runtimeStatus = *status;
            ChannelEvent channelEvent = LoadChannelEventDataFromBuffer(runtimeStatus, buffer);
            assert(int(channelEvent.message) != 240);
            try 
            {
                observer->OnChannelEventLoaded(deltaTime, channelEvent, false);
            } catch (const std::exception& e)
            {
                throw MIDIParserException(*this, e.what(), (const char*) buffer.Data());
            } 
        }
    }

    if (buffer.Data() != startBytes + trackChunk.length)
    {
        byteToDataStr[buffer.Data()] = "buffer.Data() != startBytes + trackChunk.length";
        buffer.Consume(startBytes + trackChunk.length - buffer.Data());
        //throw MIDIParserException(*this, "not equal 0", (const char*) buffer.Data());
    }
    // assert(buffer.Data() - temp == 0);

    // bytes = startBytes + trackChunk->length;

    observer->OnTrackLoaded();
}

void MIDIParserBase::LoadFromBuffer(BufferReader& buffer)
{
    debugBufferPtr = &buffer;
    
    const FileHeader header = LoadFileHeaderFromBuffer(buffer);
    observer->OnFileHeaderLoaded(header);
    FileHeaderData data = LoadFileHeaderDataFromBuffer(buffer);
    observer->OnFileHeaderDataLoaded(data);

    MessageStatus runtimeStatus;
    for (size_t i = 0; i < data.nbTracks; i++)
    {
        LoadTrackFromBuffer(buffer, runtimeStatus);
    }

    observer->OnLoadedFromBytes();
}

void MIDIParserBase::LoadFromBytes(const char* bytes, size_t size)
{
    Buffer buffer;
    buffer.bufferStart = (const uint8_t*) bytes;
    buffer.size = size;

    BufferReader reader(buffer);
    LoadFromBuffer(reader);
}

void MIDIParserBase::LoadFromFile(const char* filename)
{
    std::ifstream file (filename, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
        size_t size = file.tellg();
        char* memblock = new char [size];
        file.seekg (0, std::ios::beg);
        file.read (memblock, size);
        file.close();

        LoadFromBytes(memblock, size);
        observer->OnLoadedFromFile(filename);

        delete[] memblock;
    }
    else
    {
        throw std::runtime_error("Couldn't open file : " + std::string(filename));
    }
}