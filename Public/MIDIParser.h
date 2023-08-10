#ifndef _MIDI_PARSER_H_
#define _MIDI_PARSER_H_

#include <cstdint>

#ifdef MAKE_DLL // CPP
#define MIDIPARSEREXPORT __declspec(dllexport)
#else
#define MIDIPARSEREXPORT __declspec(dllimport)
#endif

class MIDIPARSEREXPORT MIDIParser
{
public:
    struct FileHeader
    {
        // ['M', 'T', 'h', 'd']
        // no '\0' character at the end
        char name[4]; 

        // Should be 6
        uint32_t size;
    };

    struct FileHeaderData
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
    struct TrackHeader
    {
        uint32_t type;
        uint32_t length;
    };

    // <MTrk event>
    struct TrackEvent
    {
        // <delta-time>
        // Variable-length, int32_t is the maximum size this variable can have
        uint32_t deltaTime;
    };

    struct Event
    {
        uint64_t midiEvent;
        uint64_t sysexEvent;
        uint64_t metaEvent;
    };

    enum class ENoteEvent : uint8_t
    {
        // Regular Events
        NOTE_OFF         = 0x80,
        NOTE_ON          = 0x90,
        NOTE_AFTER_TOUCH = 0xA0, 
        CONTROL_CHANGE   = 0xB0, 
        PGM_CHANGE       = 0xC0,
        CHANNEL_AFTER_TOUCH = 0xD0,
        PITCH_BEND = 0xE0,
    };

    // 0 | XXX XXXX : then it's a value byte, not a status
    // 1 | Message (3 bytes) | Channel (4 bytes)
    struct MessageStatus
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
            return value & 0b10000000;
        }

        inline bool IsSysExEvent() const
        {
            return value == 0xf0;
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

    enum class EMidiMeta : uint8_t
    {
        SEQ_NUM         = 0x00,
        TEXT            = 0x01,
        COPYRIGHT       = 0x02,
        TRACK_NAME      = 0x03,
        INSTRUMENT_NAME = 0x04,
        LYRICS          = 0x05,
        MAKER           = 0x06,
        CUE_POINT       = 0x07,
        CHANNEL_PREFIX  = 0x20,
        END_OF_TRACK    = 0x2F,
        SET_TEMPO       = 0x51,
        SMPTE_OFFSET    = 0x54,
        TIME_SIGNATURE  = 0x58,
        KEY_SIGNATURE   = 0x59,
        SEQ_SPECIFIC    = 0x7F,
    };

    struct MetaEvent 
    {
        EMidiMeta  type;
        uint8_t* bytes;  // reference to the input buffer
        int32_t  length;
    };

    struct ChannelEvent 
    {
        ENoteEvent message; 
        uint8_t channel:4;
        uint8_t  param1;
        uint8_t  param2;
    };

    // https://users.cs.cf.ac.uk/dave/Multimedia/node158.html
    enum class ESysEvent : uint8_t
    {
        // System exclusive messages (i.e. non standard)
        SYSTEM_EXCLUSIVE = 0xF0,

        // System common messages
        MIDI_TIMING_CODE = 0xF1,    // has 1 param
        SONG_POSITION_POINTER = 0xF2, // has 2 params
        SONG_SELECT = 0xF3, // has no params
        TUNE_REQUEST = 0xF6, 

        ESCAPE_EVENT = 0xf7,

        // System real-time messages
        TIMING_CLOCK = 0xF8,
        START_SEQUENCE = 0xFA,
        CONTINUE_SEQUENCE = 0xFB,
        STOP_SEQUENCE = 0xFC,
        ACTIVE_SENSING = 0xFE,
        SYSTEM_RESET = 0xFF,
    };

    struct SysexEvent
    {
        ESysEvent type;
        uint32_t messageLength;
        const char* message;
    };

public:
    virtual void OnLoadedFromFile(const char* filename) {}
    virtual void OnLoadedFromBytes() {}

    virtual void OnFileHeaderLoaded(FileHeader* fileHeader) {}
    virtual void OnFileHeaderDataLoaded(FileHeaderData& fileHeaderData) {}

    virtual void OnTrackHeaderLoaded(TrackHeader& fileHeader) {}
    virtual void OnTrackLoaded() {}

    virtual void OnSysEventLoaded(uint32_t deltaTime, SysexEvent& sysEvent) {}
    virtual void OnMetaEventLoaded(uint32_t deltaTime, MetaEvent& metaEvent) {}
    virtual void OnChannelEventLoaded(uint32_t deltaTime, ChannelEvent& channelEvent, bool isOpti) {}



    static FileHeader* LoadFileHeaderFromBytes(const char*& bytes, size_t size);
    static FileHeaderData LoadFileHeaderDataFromBytes(const char*& bytes, size_t size);

    void LoadTrackFromBytes(MessageStatus& runtimeStatus, const char*& bytes, size_t size);
    static TrackHeader LoadTrackHeaderFromBytes(const char*& bytes, size_t size);

    static MIDIParser::MetaEvent LoadMetaEventFromBytes(const char*& bytes, size_t size);
    static MIDIParser::ChannelEvent LoadChannelEventDataFromBytes(MessageStatus& midiEvent, const char*& bytes, size_t size);
    static MIDIParser::SysexEvent LoadSysexEventFromBytes(const MessageStatus& status, const char*& bytes, size_t size);

    void LoadFromBytes(const char* bytes, size_t size);
    void LoadFromFile(const char* filename);

    static const char* ENoteEventToStr(const ENoteEvent& noteEvent);
    static const char* MidiMetaToStr(const EMidiMeta& midiMeta);
    static const char* SysEventToStr(const ESysEvent& sysEvent);
};

#endif // _MIDI_PARSER_H_