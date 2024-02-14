#ifndef _MIDI_HELPERS_H_
#define _MIDI_HELPERS_H_

#include <cstdint>
#include "Macros.h"

enum class MIDIPARSEREXPORT ENoteEvent : uint8_t
{
    // Regular Events
    NOTE_OFF = 0x80,
    NOTE_ON = 0x90,
    NOTE_AFTER_TOUCH = 0xA0,
    CONTROL_CHANGE = 0xB0,
    PGM_CHANGE = 0xC0,
    CHANNEL_AFTER_TOUCH = 0xD0,
    PITCH_BEND = 0xE0,
};

enum class MIDIPARSEREXPORT EMidiMeta : uint8_t
{
    SEQ_NUM = 0x00,
    TEXT = 0x01,
    COPYRIGHT = 0x02,
    TRACK_NAME = 0x03,
    INSTRUMENT_NAME = 0x04,
    LYRICS = 0x05,
    MARKER = 0x06,
    CUE_POINT = 0x07,
    CHANNEL_PREFIX = 0x20,
    MIDI_PORT = 0x21,
    END_OF_TRACK = 0x2F,
    SET_TEMPO = 0x51,
    SMPTE_OFFSET = 0x54,
    TIME_SIGNATURE = 0x58,
    KEY_SIGNATURE = 0x59,
    SEQ_SPECIFIC = 0x7F,
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
    // TODO : Enum ?
    // 0 = 1 track
    // 1 = multiple tracks
    // 2 = ? (unsupported)
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
        // TODO : wtf
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

inline int GetMIDIEventDataLength(ENoteEvent channelEvent)
{
    switch (channelEvent) {
        case ENoteEvent::PGM_CHANGE: 
        case ENoteEvent::CHANNEL_AFTER_TOUCH: 
        case ENoteEvent::CONTROL_CHANGE: // min is 1
            return 1;

        default: 
            return 2;
    }
}

extern "C"
{
    MIDIPARSEREXPORT inline const char* MIDIPitchToNoteName(uint32_t note) 
    {
        constexpr const char* array[] = 
        {
            "0","1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20",
            "A0", "A#0/Bb0", "B0", 
            "C1", "C#1/Db1", "D1", "D#1/Eb1", "E1", "F1", "F#1/Gb1", "G1", "G#1/Ab1", "A1", "A#1/Bb1", "B1",
            "C2", "C#2/Db2", "D2", "D#2/Eb2", "E2", "F2", "F#2/Gb2", "G2", "G#2/Ab2", "A2", "A#2/Bb2", "B2",
            "C3", "C#3/Db3", "D3", "D#3/Eb3", "E3", "F3", "F#3/Gb3", "G3", "G#3/Ab3", "A3", "A#3/Bb3", "B3",
            "C4", "C#4/Db4", "D4", "D#4/Eb4", "E4", "F4", "F#4/Gb4", "G4", "G#4/Ab4", "A4", "A#4/Bb4", "B4",
            "C5", "C#5/Db5", "D5", "D#5/Eb5", "E5", "F5", "F#5/Gb5", "G5", "G#5/Ab5", "A5", "A#5/Bb5", "B5",
            "C6", "C#6/Db6", "D6", "D#6/Eb6", "E6", "F6", "F#6/Gb6", "G6", "G#6/Ab6", "A6", "A#6/Bb6", "B6",
            "C7", "C#7/Db7", "D7", "D#7/Eb7", "E7", "F7", "F#7/Gb7", "G7", "G#7/Ab7", "A7", "A#7/Bb7", "B7",
            "C8", "C#8/Db8", "D8", "D#8/Eb8", "E8", "F8", "F#8/Gb8", "G8", "G#8/Ab8", "A8", "A#8/Bb8", "B8",
            "C9", "C#9/Db9", "D9", "D#9/Eb9", "E9", "F9", "F#9/Gb9", "G9", "G#9/Ab9"
        };

        return array[note];
    }
}

#endif