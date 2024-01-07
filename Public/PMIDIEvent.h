#pragma once

#include <cstdint>
#include "MIDIHelpers.h"

class IMIDIEventReceiver;

// Polymorphic struct for midi representation
struct MIDIPARSEREXPORT PMIDIEvent
{
    uint32_t deltaTime = 0; // in ms (or ticks), from start or from last

    //virtual void Play() = 0;
    virtual void Execute(IMIDIEventReceiver* receiver) {}
    virtual ~PMIDIEvent() noexcept {}
};

struct MIDIPARSEREXPORT PMIDIChannelEvent : PMIDIEvent
{
    uint32_t channel = 0;
};

struct MIDIPARSEREXPORT PMIDIMetaEvent : PMIDIEvent
{

};

struct MIDIPARSEREXPORT PMIDISysEvent : PMIDIEvent
{
    virtual void Execute(IMIDIEventReceiver* receiver) override;
};

struct MIDIPARSEREXPORT NoteOn : PMIDIChannelEvent
{
    uint32_t key = 0;
    uint32_t velocity = 0;

    virtual void Execute(IMIDIEventReceiver* receiver) override;
};

struct MIDIPARSEREXPORT NoteOff : PMIDIChannelEvent
{
    uint32_t key = 0;

    virtual void Execute(IMIDIEventReceiver* receiver) override;
};

// A mix of NoteOn and NoteOff ; ensures there is always a noteoff for a noteon note
struct MIDIPARSEREXPORT NoteOnOff : PMIDIChannelEvent
{
    uint32_t key = 0;
    uint32_t velocity = 0;
    uint32_t duration = 0;

    virtual void Execute(IMIDIEventReceiver* receiver) override;
};

// A note used in a music partition
struct MIDIPARSEREXPORT PartitionNote : PMIDIChannelEvent
{
    uint32_t key = 0;
    uint32_t velocity = 0;

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

    virtual void Execute(IMIDIEventReceiver* receiver) override;
};

struct MIDIPARSEREXPORT PitchBend : PMIDIChannelEvent
{
    uint32_t value = 0;

    virtual void Execute(IMIDIEventReceiver* receiver) override;
};

// @TODO
struct MIDIPARSEREXPORT NoteAfterTouch : PMIDIChannelEvent
{
    virtual void Execute(IMIDIEventReceiver* receiver) override;
};

// @TODO
struct MIDIPARSEREXPORT ChannelAfterTouch : PMIDIChannelEvent
{
    virtual void Execute(IMIDIEventReceiver* receiver) override;
};


struct MIDIPARSEREXPORT ProgramChange : PMIDIChannelEvent
{
    uint32_t newProgram = 0;

    virtual void Execute(IMIDIEventReceiver* receiver) override;
};

struct MIDIPARSEREXPORT KeySignature : PMIDIMetaEvent
{
    uint8_t sf = 0;
    uint8_t mi = 0;

    virtual void Execute(IMIDIEventReceiver* receiver) override;
};

// @TODO
struct MIDIPARSEREXPORT TimeSignature : PMIDIMetaEvent
{
    virtual void Execute(IMIDIEventReceiver* receiver) override;
};

// @TODO
struct MIDIPARSEREXPORT MIDIPort : PMIDIMetaEvent
{
    virtual void Execute(IMIDIEventReceiver* receiver) override;
};

// @TODO
struct MIDIPARSEREXPORT EndOfTrack : PMIDIMetaEvent
{
    virtual void Execute(IMIDIEventReceiver* receiver) override;
};

struct MIDIPARSEREXPORT Tempo : PMIDIMetaEvent
{
    uint32_t newTempo = 0;

    virtual void Execute(IMIDIEventReceiver* receiver) override;
};


struct MIDIPARSEREXPORT PMIDIMetaEvent_WithText : PMIDIMetaEvent
{
    const char* text = nullptr;
    uint32_t length = 0;
};

struct MIDIPARSEREXPORT Text : PMIDIMetaEvent_WithText 
{
    virtual void Execute(IMIDIEventReceiver* receiver) override;
};

struct MIDIPARSEREXPORT Copyright : PMIDIMetaEvent_WithText 
{
    virtual void Execute(IMIDIEventReceiver* receiver) override;
};

struct MIDIPARSEREXPORT TrackName : PMIDIMetaEvent_WithText 
{
    virtual void Execute(IMIDIEventReceiver* receiver) override;
};

struct MIDIPARSEREXPORT InstrumentName : PMIDIMetaEvent_WithText 
{
    virtual void Execute(IMIDIEventReceiver* receiver) override;
};

struct MIDIPARSEREXPORT Lyrics : PMIDIMetaEvent_WithText 
{
    virtual void Execute(IMIDIEventReceiver* receiver) override;
};

struct MIDIPARSEREXPORT Marker : PMIDIMetaEvent_WithText 
{
    virtual void Execute(IMIDIEventReceiver* receiver) override;
};

struct MIDIPARSEREXPORT CuePoint : PMIDIMetaEvent_WithText 
{
    virtual void Execute(IMIDIEventReceiver* receiver) override;
};

struct MIDIPARSEREXPORT UnsupportedEvent : PMIDIEvent
{
    virtual void Execute(IMIDIEventReceiver* receiver) override;
};

