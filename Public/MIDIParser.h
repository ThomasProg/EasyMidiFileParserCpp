#ifndef _MIDI_PARSER_H_
#define _MIDI_PARSER_H_

#include <cstdint>
#include <unordered_map>
#include <string>

#ifdef MAKE_DLL // CPP
#define MIDIPARSEREXPORT __declspec(dllexport)
#else
#define MIDIPARSEREXPORT __declspec(dllimport)
#endif

class MIDIParser
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

    enum class MIDIPARSEREXPORT ENoteEvent : uint8_t
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

    enum class MIDIPARSEREXPORT EMidiMeta : uint8_t
    {
        SEQ_NUM         = 0x00,
        TEXT            = 0x01,
        COPYRIGHT       = 0x02,
        TRACK_NAME      = 0x03,
        INSTRUMENT_NAME = 0x04,
        LYRICS          = 0x05,
        MARKER           = 0x06,
        CUE_POINT       = 0x07,
        CHANNEL_PREFIX  = 0x20,
        MIDI_PORT       = 0x21,
        END_OF_TRACK    = 0x2F,
        SET_TEMPO       = 0x51,
        SMPTE_OFFSET    = 0x54,
        TIME_SIGNATURE  = 0x58,
        KEY_SIGNATURE   = 0x59,
        SEQ_SPECIFIC    = 0x7F,
    };

    enum class MIDIPARSEREXPORT EControlChange : uint8_t
    {
        BANK_SELECT = 0,
        MODULATION_WHEEL = 1,
        BREATH_CONTROLLER = 2,

        FOOT_PEDAL = 4,
        PORTAMENTO_TIME = 5,
        DATA_ENTRY = 6,
        VOLUME = 7,
        BALANCE = 8,

        PAN = 10,
        EXPRESSION = 11,      
        EFFECT_CONTROLLER_1 = 12,
        EFFECT_CONTROLLER_2 = 13,
        GENERAL_PURPOSE_1 = 16,
        GENERAL_PURPOSE_2 = 17,
        GENERAL_PURPOSE_3 = 18,
        GENERAL_PURPOSE_4 = 19,  

        DAMPER_PEDAL = 64,
        PORTAMENTO_ON_OFF = 65,
        SOSTENUTO_PEDAL_ON_OFF = 66,  
        SOFT_PEDAL_ON_OFF = 67,
        LEGATO_FOOT_SWITCH = 68,
        HOLD_2 = 69,
        SOUND_CONTROLLER_1 = 70,
        SOUND_CONTROLLER_2 = 71,
        SOUND_CONTROLLER_3 = 72,
        SOUND_CONTROLLER_4 = 73,
        SOUND_CONTROLLER_5 = 74,
        SOUND_CONTROLLER_6 = 75,
        SOUND_CONTROLLER_7 = 76,
        SOUND_CONTROLLER_8 = 77,
        SOUND_CONTROLLER_9 = 78,
        SOUND_CONTROLLER_10 = 79,
        GENERAL_PURPOSE_MIDI_CC_CONTROLLER_1 = 80,
        GENERAL_PURPOSE_MIDI_CC_CONTROLLER_2 = 81,
        GENERAL_PURPOSE_MIDI_CC_CONTROLLER_3 = 82,
        GENERAL_PURPOSE_MIDI_CC_CONTROLLER_4 = 83,
        PORTAMENTO_CC_CONTROL = 84,

        HIGH_RESOLUTION_VELOCITY_FIX = 88,

        EFFECT_1_DEPTH = 91,
        EFFECT_2_DEPTH = 92,
        EFFECT_3_DEPTH = 93,
        EFFECT_4_DEPTH = 94,
        EFFECT_5_DEPTH = 95,

        DATA_INCREMENT = 96,
        DATA_DECREMENT = 97,
        NON_REGISTERED_PARAMETER_NUMBER_LSB = 98,   
        NON_REGISTERED_PARAMETER_NUMBER_MSB = 99,
        REGISTERED_PARAMETER_NUMBER_LSB = 100,   
        REGISTERED_PARAMETER_NUMBER_MSB = 101,

        ALL_SOUND_OFF = 120,
        RESET_ALL_CONTROLLERS = 121,
        LOCAL_ON_OFF_SWITCH = 122,
        ALL_NOTES_OFF = 123,
        OMNI_MODE_OFF = 124,
        OMNI_MODE_ON = 125,
        MONO_MODE = 126,
        POLY_MODE = 127,
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

    // https://users.cs.cf.ac.uk/dave/Multimedia/node158.html
    enum class MIDIPARSEREXPORT ESysEvent : uint8_t
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
    MIDIPARSEREXPORT const char* GetDataStrFromByte(void* byte) const;

    MIDIPARSEREXPORT virtual void OnLoadedFromFile(const char* filename) {}
    MIDIPARSEREXPORT virtual void OnLoadedFromBytes() {}

    MIDIPARSEREXPORT virtual void OnFileHeaderLoaded(const FileHeader& fileHeader) {}
    MIDIPARSEREXPORT virtual void OnFileHeaderDataLoaded(FileHeaderData& fileHeaderData) {}

    MIDIPARSEREXPORT virtual void OnTrackHeaderLoaded(TrackHeader& fileHeader) {}
    MIDIPARSEREXPORT virtual void OnTrackLoaded() {}

    MIDIPARSEREXPORT virtual void OnSysEventLoaded(uint32_t deltaTime, SysexEvent& sysEvent) {}
    MIDIPARSEREXPORT virtual void OnMetaEventLoaded(uint32_t deltaTime, MetaEvent& metaEvent) {}
    MIDIPARSEREXPORT virtual void OnChannelEventLoaded(uint32_t deltaTime, ChannelEvent& channelEvent, bool isOpti) {}


    MIDIPARSEREXPORT static void Reduce(size_t& size, size_t reduced);

    MIDIPARSEREXPORT static const FileHeader LoadFileHeaderFromBuffer(BufferReader& buffer);
    MIDIPARSEREXPORT static FileHeaderData LoadFileHeaderDataFromBuffer(BufferReader& buffer);

    MIDIPARSEREXPORT void LoadTrackFromBuffer(BufferReader& buffer, MessageStatus& runtimeStatus);
    MIDIPARSEREXPORT static TrackHeader LoadTrackHeaderFromBuffer(BufferReader& buffer);

    MIDIPARSEREXPORT static MIDIParser::MetaEvent LoadMetaEventFromBuffer(BufferReader& buffer);
    MIDIPARSEREXPORT static MIDIParser::ChannelEvent LoadChannelEventDataFromBuffer(MessageStatus& midiEvent, BufferReader& buffer);
    MIDIPARSEREXPORT static MIDIParser::SysexEvent LoadSysexEventFromBuffer(const MessageStatus& status, BufferReader& buffer);

    MIDIPARSEREXPORT void LoadFromBuffer(BufferReader& buffer);
    MIDIPARSEREXPORT void LoadFromBytes(const char* bytes, size_t size);
    MIDIPARSEREXPORT void LoadFromFile(const char* filename);

    MIDIPARSEREXPORT static const char* ENoteEventToStr(const ENoteEvent& noteEvent);
    MIDIPARSEREXPORT static const char* MidiMetaToStr(const EMidiMeta& midiMeta);
    MIDIPARSEREXPORT static const char* SysEventToStr(const ESysEvent& sysEvent);
};

#endif // _MIDI_PARSER_H_