#include "PMIDIEvent.h"
#include "IMIDIEventReceiver.h"

void PMIDISysEvent::Execute(IMIDIEventReceiver* receiver)
{
    receiver->OnSysEvent(*this);
}

void NoteOn::Execute(IMIDIEventReceiver* receiver)
{
    receiver->OnNoteOn(*this);
}

void NoteOff::Execute(IMIDIEventReceiver* receiver)
{
    receiver->OnNoteOff(*this);
}

void NoteOnOff::Execute(IMIDIEventReceiver* receiver)
{
    receiver->OnNoteOnOff(*this);
}

void ControlChange::Execute(IMIDIEventReceiver* receiver) 
{
    receiver->OnControlChange(*this);
}

void PitchBend::Execute(IMIDIEventReceiver* receiver) 
{
    receiver->OnPitchBend(*this);
}

void NoteAfterTouch::Execute(IMIDIEventReceiver* receiver)
{
    receiver->OnNoteAfterTouch(*this);
}

void ChannelAfterTouch::Execute(IMIDIEventReceiver* receiver)
{
    receiver->OnChannelAfterTouch(*this);
}

void ProgramChange::Execute(IMIDIEventReceiver* receiver) 
{
    receiver->OnProgramChange(*this);
}

void KeySignature::Execute(IMIDIEventReceiver* receiver) 
{
    receiver->OnKeySignature(*this);
}

void TimeSignature::Execute(IMIDIEventReceiver* receiver)
{
    receiver->OnTimeSignature(*this);
}

void MIDIPort::Execute(IMIDIEventReceiver* receiver)
{
    receiver->OnMIDIPort(*this);
}

void EndOfTrack::Execute(IMIDIEventReceiver* receiver)
{
    receiver->OnEndOfTrack(*this);
}

void Tempo::Execute(IMIDIEventReceiver* receiver) 
{
    receiver->OnTempo(*this);
}

void Text::Execute(IMIDIEventReceiver* receiver) 
{
    receiver->OnText(*this);
}

void Copyright::Execute(IMIDIEventReceiver* receiver) 
{
    receiver->OnCopyright(*this);
}

void TrackName::Execute(IMIDIEventReceiver* receiver) 
{
    receiver->OnTrackName(*this);
}

void InstrumentName::Execute(IMIDIEventReceiver* receiver) 
{
    receiver->OnInstrumentName(*this);
}

void Lyrics::Execute(IMIDIEventReceiver* receiver) 
{
    receiver->OnLyrics(*this);
}

void Marker::Execute(IMIDIEventReceiver* receiver) 
{
    receiver->OnMarker(*this);
}

void CuePoint::Execute(IMIDIEventReceiver* receiver) 
{
    receiver->OnCuePoint(*this);
}

void UnsupportedEvent::Execute(IMIDIEventReceiver* receiver)
{
    receiver->OnUnsupportedEvent(*this);
}


uint32_t MIDIEvent_GetDeltaTime(PMIDIEvent* event)
{
    return event->deltaTime;
}
uint32_t MIDIChannelEvent_GetChannel(PMIDIChannelEvent* event)
{
    return event->channel;
}

uint32_t NoteOn_GetKey(NoteOn* event)
{
    return event->key;
}
uint32_t NoteOn_GetVelocity(NoteOn* event)
{
    return event->velocity;
}

uint32_t NoteOff_GetKey(NoteOff* event)
{
    return event->key;
}

uint32_t NoteOnOff_GetKey(NoteOnOff* event)
{
    return event->key;
}
uint32_t NoteOnOff_GetVelocity(NoteOnOff* event)
{
    return event->velocity;
}
uint32_t NoteOnOff_GetDuration(NoteOnOff* event)
{
    return event->duration;
}
