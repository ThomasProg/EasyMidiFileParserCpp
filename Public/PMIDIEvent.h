#pragma once

#include <cstdint>
#include "MIDIHelpers.h"

// Polymorphic struct for midi representation
struct MIDIPARSEREXPORT PMIDIEvent
{
    uint32_t start = 0; // in ms

    //virtual void Play() = 0;
    virtual ~PMIDIEvent() noexcept {}
};

struct MIDIPARSEREXPORT PMIDIChannelEvent : PMIDIEvent
{
    uint32_t channel = 0;
};

struct MIDIPARSEREXPORT NoteOn : PMIDIChannelEvent
{
    uint32_t key = 0;
    uint32_t velocity = 0;

    //virtual void Play() override;
};

struct MIDIPARSEREXPORT NoteOff : PMIDIChannelEvent
{
    uint32_t key = 0;

    ////virtual void Play() override;
};

// A mix of NoteOn and NoteOff ; ensures there is always a noteoff for a noteon note
struct MIDIPARSEREXPORT NoteOnOff : PMIDIChannelEvent
{
    uint32_t key = 0;
    uint32_t velocity = 0;
    uint32_t duration = 0;

    // virtual void Play() override;
};

// A note used in a music partition
struct MIDIPARSEREXPORT PartitionNote : PMIDIChannelEvent
{
    uint32_t key = 0;
    uint32_t velocity = 0;
    // virtual void Play() override;

    virtual uint8_t GetDiv() const = 0;
};

// A note used in a music partition
struct MIDIPARSEREXPORT WholeNote final : PartitionNote
{
    virtual uint8_t GetDiv() const override
    {
        return 1;
    }
};

// A note used in a music partition
struct MIDIPARSEREXPORT HalfNote final : PartitionNote
{
    virtual uint8_t GetDiv() const override
    {
        return 2;
    }
};

// A note used in a music partition
struct MIDIPARSEREXPORT QuarterNote final : PartitionNote
{
    virtual uint8_t GetDiv() const override
    {
        return 4;
    }
};

// A note used in a music partition
struct MIDIPARSEREXPORT EigthNote final : PartitionNote
{
    virtual uint8_t GetDiv() const override
    {
        return 8;
    }
};

// A note used in a music partition
struct MIDIPARSEREXPORT SixteenthNote final : PartitionNote
{
    virtual uint8_t GetDiv() const override
    {
        return 16;
    }
};



struct MIDIPARSEREXPORT ControlChange : PMIDIChannelEvent
{
    EControlChange ctrl;
    uint32_t value = 0;

    // virtual void Play() override;
};

struct MIDIPARSEREXPORT PitchBend : PMIDIChannelEvent
{
    uint32_t value = 0;

    // virtual void Play() override;
};


struct MIDIPARSEREXPORT ProgramChange : PMIDIChannelEvent
{
    uint32_t newProgram = 0;

    // virtual void Play() override;
};

struct MIDIPARSEREXPORT KeySignature : PMIDIChannelEvent
{
    uint8_t sf = 0;
    uint8_t mi = 0;

    // virtual void Play() override {}
};