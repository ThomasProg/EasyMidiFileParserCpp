#pragma once

#include <cstdint>
#include "MIDIHelpers.h"

#define CLONE_OVERRIDE(className) virtual PMIDIEvent* Clone() override;

class IMIDIEventReceiver;

// Polymorphic struct for midi representation
struct MIDIPARSEREXPORT PMIDIEvent
{
    uint32_t deltaTime = 0; // in ms (or ticks), from start or from last

    //virtual void Play() = 0;
    virtual void Execute(IMIDIEventReceiver* receiver) {}
    virtual PMIDIEvent* Clone() = 0;
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
    virtual PMIDIEvent* Clone() override;
};

struct MIDIPARSEREXPORT NoteOn : PMIDIChannelEvent
{
    uint32_t key = 0;
    uint32_t velocity = 0;

    virtual void Execute(IMIDIEventReceiver* receiver) override;
    virtual PMIDIEvent* Clone() override;
};

struct MIDIPARSEREXPORT NoteOff : PMIDIChannelEvent
{
    uint32_t key = 0;

    virtual void Execute(IMIDIEventReceiver* receiver) override;
    virtual PMIDIEvent* Clone() override;
};

// A mix of NoteOn and NoteOff ; ensures there is always a noteoff for a noteon note
struct MIDIPARSEREXPORT NoteOnOff : PMIDIChannelEvent
{
    uint32_t key = 0;
    uint32_t velocity = 0;
    uint32_t duration = 0;

    virtual void Execute(IMIDIEventReceiver* receiver) override;
    virtual PMIDIEvent* Clone() override;
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
    virtual PMIDIEvent* Clone() override;
};

struct MIDIPARSEREXPORT PitchBend : PMIDIChannelEvent
{
    uint32_t value = 0;

    virtual void Execute(IMIDIEventReceiver* receiver) override;
    virtual PMIDIEvent* Clone() override;
};

// @TODO
struct MIDIPARSEREXPORT NoteAfterTouch : PMIDIChannelEvent
{
    virtual void Execute(IMIDIEventReceiver* receiver) override;
    virtual PMIDIEvent* Clone() override;
};

// @TODO
struct MIDIPARSEREXPORT ChannelAfterTouch : PMIDIChannelEvent
{
    virtual void Execute(IMIDIEventReceiver* receiver) override;
    virtual PMIDIEvent* Clone() override;
};


struct MIDIPARSEREXPORT ProgramChange : PMIDIChannelEvent
{
    uint32_t newProgram = 0;

    virtual void Execute(IMIDIEventReceiver* receiver) override;
    virtual PMIDIEvent* Clone() override;
};

struct MIDIPARSEREXPORT KeySignature : PMIDIMetaEvent
{
    uint8_t sf = 0;
    uint8_t mi = 0;

    virtual void Execute(IMIDIEventReceiver* receiver) override;
    virtual PMIDIEvent* Clone() override;
};

// @TODO
struct MIDIPARSEREXPORT TimeSignature : PMIDIMetaEvent
{
    virtual void Execute(IMIDIEventReceiver* receiver) override;
    virtual PMIDIEvent* Clone() override;
};

// @TODO
struct MIDIPARSEREXPORT MIDIPort : PMIDIMetaEvent
{
    virtual void Execute(IMIDIEventReceiver* receiver) override;
    virtual PMIDIEvent* Clone() override;
};

// @TODO
struct MIDIPARSEREXPORT EndOfTrack : PMIDIMetaEvent
{
    virtual void Execute(IMIDIEventReceiver* receiver) override;
    virtual PMIDIEvent* Clone() override;
};

struct MIDIPARSEREXPORT Tempo : PMIDIMetaEvent
{
    uint32_t newTempo = 0;

    virtual void Execute(IMIDIEventReceiver* receiver) override;
    virtual PMIDIEvent* Clone() override;
};


struct MIDIPARSEREXPORT PMIDIMetaEvent_WithText : PMIDIMetaEvent
{
    const char* text = nullptr;
    uint32_t length = 0;
};

struct MIDIPARSEREXPORT Text : PMIDIMetaEvent_WithText 
{
    virtual void Execute(IMIDIEventReceiver* receiver) override;
    virtual PMIDIEvent* Clone() override;
};

struct MIDIPARSEREXPORT Copyright : PMIDIMetaEvent_WithText 
{
    virtual void Execute(IMIDIEventReceiver* receiver) override;
    virtual PMIDIEvent* Clone() override;
};

struct MIDIPARSEREXPORT TrackName : PMIDIMetaEvent_WithText 
{
    virtual void Execute(IMIDIEventReceiver* receiver) override;
    virtual PMIDIEvent* Clone() override;
};

struct MIDIPARSEREXPORT InstrumentName : PMIDIMetaEvent_WithText 
{
    virtual void Execute(IMIDIEventReceiver* receiver) override;
    virtual PMIDIEvent* Clone() override;
};

struct MIDIPARSEREXPORT Lyrics : PMIDIMetaEvent_WithText 
{
    virtual void Execute(IMIDIEventReceiver* receiver) override;
    virtual PMIDIEvent* Clone() override;
};

struct MIDIPARSEREXPORT Marker : PMIDIMetaEvent_WithText 
{
    virtual void Execute(IMIDIEventReceiver* receiver) override;
    virtual PMIDIEvent* Clone() override;
};

struct MIDIPARSEREXPORT CuePoint : PMIDIMetaEvent_WithText 
{
    virtual void Execute(IMIDIEventReceiver* receiver) override;
    virtual PMIDIEvent* Clone() override;
};

struct MIDIPARSEREXPORT UnsupportedEvent : PMIDIEvent
{
    virtual void Execute(IMIDIEventReceiver* receiver) override;
    virtual PMIDIEvent* Clone() override;
};

extern "C"
{
    MIDIPARSEREXPORT uint32_t MIDIEvent_GetDeltaTime(PMIDIEvent* event);
    MIDIPARSEREXPORT uint32_t MIDIChannelEvent_GetChannel(PMIDIChannelEvent* event);
    MIDIPARSEREXPORT void MIDIEvent_SetDeltaTime(PMIDIEvent* event, uint32_t deltaTime);
    MIDIPARSEREXPORT void MIDIChannelEvent_SetChannel(PMIDIChannelEvent* event, uint32_t channel);

    MIDIPARSEREXPORT uint32_t NoteOn_GetKey(NoteOn* event);
    MIDIPARSEREXPORT uint32_t NoteOn_GetVelocity(NoteOn* event);

    MIDIPARSEREXPORT uint32_t NoteOff_GetKey(NoteOff* event);

    MIDIPARSEREXPORT uint32_t NoteOnOff_GetKey(NoteOnOff* event);
    MIDIPARSEREXPORT uint32_t NoteOnOff_GetVelocity(NoteOnOff* event);
    MIDIPARSEREXPORT uint32_t NoteOnOff_GetDuration(NoteOnOff* event);
    MIDIPARSEREXPORT void NoteOnOff_SetKey(NoteOnOff* event, uint32_t key);
    MIDIPARSEREXPORT void NoteOnOff_SetVelocity(NoteOnOff* event, uint32_t velocity);
    MIDIPARSEREXPORT void NoteOnOff_SetDuration(NoteOnOff* event, uint32_t duration);

    MIDIPARSEREXPORT NoteOn* NoteOn_Create();
    MIDIPARSEREXPORT NoteOff* NoteOff_Create();
    MIDIPARSEREXPORT NoteOnOff* NoteOnOff_Create();
    MIDIPARSEREXPORT void PMIDIEvent_Destroy(PMIDIEvent* destroy);
}

