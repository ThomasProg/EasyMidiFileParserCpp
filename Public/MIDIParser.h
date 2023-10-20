#ifndef _MIDI_PARSER_H_
#define _MIDI_PARSER_H_

#include <cstdint>
#include <unordered_map>
#include <string>
#include "MIDIHelpers.h"

#include <stdexcept>

#ifdef MAKE_DLL // CPP
#define MIDIPARSEREXPORT __declspec(dllexport)
#else
#define MIDIPARSEREXPORT __declspec(dllimport)
#endif

class MIDIPARSEREXPORT MIDIParser
{
public:
    struct MIDIPARSEREXPORT Buffer
    {
        const uint8_t* bufferStart;
        size_t size;
    };

    class MIDIPARSEREXPORT BufferReader
    {
        Buffer& buffer;
        size_t offset = 0;

    public:
        BufferReader(Buffer& newBuffer) : buffer(newBuffer) {}

        const uint8_t* Consume(size_t size)
        {
            const uint8_t* bufferReturned = buffer.bufferStart + offset;
            offset += size;
            if (offset > buffer.size)
            {
                throw std::runtime_error("offset > buffer.size");
            }
            return bufferReturned;
        }

        void GoBack(size_t size)
        {
            offset -= size;
        }

        const uint8_t* Data() const
        {
            return buffer.bufferStart + offset;
        }
    };

    struct MIDIPARSEREXPORT FileHeader
    {
        // ['M', 'T', 'h', 'd']
        // no '\0' character at the end
        char name[4]; 

        // Should be 6
        uint32_t size;
    };

    struct MIDIPARSEREXPORT FileHeaderData
    {
        // <format >
        uint16_t format;

        // <ntrks>
        // defines the amount of track chunks
        int16_t nbTracks; 
        
        // If true, then it's ticks per frame
        // If false, it's ticks per quarter-note 
        bool isTicksPerFrame:1;

        // <division>
        // specifies the meaning of the delta-times. 
        // It has two formats, one for metrical time, and one for time-code-based time
        union Delta
        {
            int16_t ticksPerQuarterNote:15;
            struct Division
            {
                int8_t SMPTEFormat:7;
                int8_t ticksPerFrame;
            } perFrame;
        } delta;
    }; 

    // <Track Chunk>
    struct MIDIPARSEREXPORT TrackHeader
    {
        uint32_t type;
        uint32_t length;
    };

    // <MTrk event>
    struct MIDIPARSEREXPORT TrackEvent
    {
        // <delta-time>
        // Variable-length, int32_t is the maximum size this variable can have
        uint32_t deltaTime;
    };

    struct MIDIPARSEREXPORT Event
    {
        uint64_t midiEvent;
        uint64_t sysexEvent;
        uint64_t metaEvent;
    };

    // 0 | XXX XXXX : then it's a value byte, not a status
    // 1 | Message (3 bytes) | Channel (4 bytes)
    struct MIDIPARSEREXPORT MessageStatus
    {
        uint8_t value = 0;

        inline void Reset()
        {
            value = 0;
        }

        inline bool IsValid() const
        {
            return value != 0;
        }

        inline bool IsValidStatusEvent() const
        {
            int8_t v = *(int8_t*) &value;
            return v < 0;
        }

        inline bool IsSysExEvent() const
        {
            return ((value & 0b11110000) == 0b11110000) && !IsMetaEvent();
        }

        inline bool IsMetaEvent() const
        {
            return value == 0xff;
        }

        // Returns the channel ID in Big Endian
        inline int8_t GetChannel() const
        {
            return (value & 0b00001111);
        }

        // Returns the message (in)
        inline ENoteEvent GetMessage() const
        {
            return (ENoteEvent) (value & 0b11110000);
        }
    };

    struct MIDIPARSEREXPORT MetaEvent
    {
        EMidiMeta  type;
        uint8_t* bytes;  // reference to the input buffer
        uint32_t  length;
    };

    struct MIDIPARSEREXPORT ChannelEvent
    {
        ENoteEvent message; 
        uint8_t channel:4;
        uint8_t  param1;
        uint8_t  param2;
    };

    struct MIDIPARSEREXPORT SysexEvent
    {
        ESysEvent type;
        uint32_t messageLength;
        const char* message;
    };

public:
    std::unordered_map<const void*, std::string> byteToDataStr;
    BufferReader* debugBufferPtr = nullptr;

public:
    const char* GetDataStrFromByte(void* byte) const;

    virtual void OnLoadedFromFile(const char* filename) {}
    virtual void OnLoadedFromBytes() {}

    virtual void OnFileHeaderLoaded(const FileHeader& fileHeader) {}
    virtual void OnFileHeaderDataLoaded(FileHeaderData& fileHeaderData) {}

    virtual void OnTrackHeaderLoaded(TrackHeader& fileHeader) {}
    virtual void OnTrackLoaded() {}

    virtual void OnSysEventLoaded(uint32_t deltaTime, SysexEvent& sysEvent) {}
    virtual void OnMetaEventLoaded(uint32_t deltaTime, MetaEvent& metaEvent) {}
    virtual void OnChannelEventLoaded(uint32_t deltaTime, ChannelEvent& channelEvent, bool isOpti) {}


    static void Reduce(size_t& size, size_t reduced);

    static const FileHeader LoadFileHeaderFromBuffer(BufferReader& buffer);
    static FileHeaderData LoadFileHeaderDataFromBuffer(BufferReader& buffer);

    void LoadTrackFromBuffer(BufferReader& buffer, MessageStatus& runtimeStatus);
    static TrackHeader LoadTrackHeaderFromBuffer(BufferReader& buffer);

    static MIDIParser::MetaEvent LoadMetaEventFromBuffer(BufferReader& buffer);
    static MIDIParser::ChannelEvent LoadChannelEventDataFromBuffer(MessageStatus& midiEvent, BufferReader& buffer);
    static MIDIParser::SysexEvent LoadSysexEventFromBuffer(const MessageStatus& status, BufferReader& buffer);

    void LoadFromBuffer(BufferReader& buffer);
    void LoadFromBytes(const char* bytes, size_t size);
    void LoadFromFile(const char* filename);

    static const char* ENoteEventToStr(const ENoteEvent& noteEvent);
    static const char* MidiMetaToStr(const EMidiMeta& midiMeta);
    static const char* SysEventToStr(const ESysEvent& sysEvent);
};

#endif // _MIDI_PARSER_H_