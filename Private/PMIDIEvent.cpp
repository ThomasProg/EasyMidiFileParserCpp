#include "PMIDIEvent.h"
#include "IMIDIEventReceiver.h"

void PMIDISysEvent::Execute(IMIDIEventReceiver* receiver)
{
    receiver->OnSysEvent(*this);
}

PMIDIEvent* PMIDISysEvent::Clone() 
{
    return new PMIDISysEvent(*this);
}

void NoteOn::Execute(IMIDIEventReceiver* receiver)
{
    receiver->OnNoteOn(*this);
}

PMIDIEvent* NoteOn::Clone()
{
    return new NoteOn(*this);
}

void NoteOff::Execute(IMIDIEventReceiver* receiver)
{
    receiver->OnNoteOff(*this);
}

PMIDIEvent* NoteOff::Clone()
{
    return new NoteOff(*this);
}

void NoteOnOff::Execute(IMIDIEventReceiver* receiver)
{
    receiver->OnNoteOnOff(*this);
}

PMIDIEvent* NoteOnOff::Clone()
{
    return new NoteOnOff(*this);
}

void ControlChange::Execute(IMIDIEventReceiver* receiver) 
{
    receiver->OnControlChange(*this);
}

PMIDIEvent* ControlChange::Clone()
{
    return new ControlChange(*this);
}

void PitchBend::Execute(IMIDIEventReceiver* receiver) 
{
    receiver->OnPitchBend(*this);
}

PMIDIEvent* PitchBend::Clone()
{
    return new PitchBend(*this);
}

void NoteAfterTouch::Execute(IMIDIEventReceiver* receiver)
{
    receiver->OnNoteAfterTouch(*this);
}

PMIDIEvent* NoteAfterTouch::Clone()
{
    return new NoteAfterTouch(*this);
}

void ChannelAfterTouch::Execute(IMIDIEventReceiver* receiver)
{
    receiver->OnChannelAfterTouch(*this);
}

PMIDIEvent* ChannelAfterTouch::Clone()
{
    return new ChannelAfterTouch(*this);
}

void ProgramChange::Execute(IMIDIEventReceiver* receiver) 
{
    receiver->OnProgramChange(*this);
}

PMIDIEvent* ProgramChange::Clone()
{
    return new ProgramChange(*this);
}

void KeySignature::Execute(IMIDIEventReceiver* receiver) 
{
    receiver->OnKeySignature(*this);
}

PMIDIEvent* KeySignature::Clone()
{
    return new KeySignature(*this);
}

void TimeSignature::Execute(IMIDIEventReceiver* receiver)
{
    receiver->OnTimeSignature(*this);
}

PMIDIEvent* TimeSignature::Clone()
{
    return new TimeSignature(*this);
}

void MIDIPort::Execute(IMIDIEventReceiver* receiver)
{
    receiver->OnMIDIPort(*this);
}

PMIDIEvent* MIDIPort::Clone()
{
    return new MIDIPort(*this);
}

void EndOfTrack::Execute(IMIDIEventReceiver* receiver)
{
    receiver->OnEndOfTrack(*this);
}

PMIDIEvent* EndOfTrack::Clone()
{
    return new EndOfTrack(*this);
}

void Tempo::Execute(IMIDIEventReceiver* receiver) 
{
    receiver->OnTempo(*this);
}

PMIDIEvent* Tempo::Clone()
{
    return new Tempo(*this);
}

void Text::Execute(IMIDIEventReceiver* receiver) 
{
    receiver->OnText(*this);
}

PMIDIEvent* Text::Clone()
{
    return new Text(*this);
}

void Copyright::Execute(IMIDIEventReceiver* receiver) 
{
    receiver->OnCopyright(*this);
}

PMIDIEvent* Copyright::Clone()
{
    return new Copyright(*this);
}

void TrackName::Execute(IMIDIEventReceiver* receiver) 
{
    receiver->OnTrackName(*this);
}

PMIDIEvent* TrackName::Clone()
{
    return new TrackName(*this);
}

void InstrumentName::Execute(IMIDIEventReceiver* receiver) 
{
    receiver->OnInstrumentName(*this);
}

PMIDIEvent* InstrumentName::Clone()
{
    return new InstrumentName(*this);
}

void Lyrics::Execute(IMIDIEventReceiver* receiver) 
{
    receiver->OnLyrics(*this);
}

PMIDIEvent* Lyrics::Clone()
{
    return new Lyrics(*this);
}

void Marker::Execute(IMIDIEventReceiver* receiver) 
{
    receiver->OnMarker(*this);
}

PMIDIEvent* Marker::Clone()
{
    return new Marker(*this);
}

void CuePoint::Execute(IMIDIEventReceiver* receiver) 
{
    receiver->OnCuePoint(*this);
}

PMIDIEvent* CuePoint::Clone()
{
    return new CuePoint(*this);
}

void UnsupportedEvent::Execute(IMIDIEventReceiver* receiver)
{
    receiver->OnUnsupportedEvent(*this);
}

PMIDIEvent* UnsupportedEvent::Clone()
{
    return new UnsupportedEvent(*this);
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
