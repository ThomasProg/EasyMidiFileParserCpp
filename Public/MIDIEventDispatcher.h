#pragma once

#include "IMIDIEventReceiver.h"

// C Wrapper for other language bindings

struct MIDIPARSEREXPORT MIDIEventCallbacks
{
    void (*OnEvent)(void* userData, const PMIDIEvent* event) = nullptr;

    void (*OnSysEvent)(void* userData, const PMIDISysEvent* sysEvent) = nullptr;
    void (*OnMetaEvent)(void* userData, const PMIDIMetaEvent* metaEvent) = nullptr;
    void (*OnChannelEvent)(void* userData, const PMIDIChannelEvent* channelEvent) = nullptr;

    void (*OnNoteOn)(void* userData, const NoteOn* noteOn) = nullptr;
    void (*OnNoteOff)(void* userData, const NoteOff* noteOff) = nullptr;
    void (*OnNoteOnOff)(void* userData, const NoteOnOff* noteOnOff) = nullptr;
    void (*OnProgramChange)(void* userData, const ProgramChange* programChange) = nullptr;
    void (*OnControlChange)(void* userData, const ControlChange* controlChange) = nullptr;
    void (*OnPitchBend)(void* userData, const PitchBend* pitchBend) = nullptr;
    void (*OnNoteAfterTouch)(void* userData, const NoteAfterTouch* noteAfterTouch) = nullptr;
    void (*OnChannelAfterTouch)(void* userData, const ChannelAfterTouch* channelAfterTouch) = nullptr;

    void (*OnTimeSignature)(void* userData, const TimeSignature* timeSignature) = nullptr;
};

class MIDIEventDispatcher : public IMIDIEventReceiver
{
public:
    void* userData;
    MIDIEventCallbacks callbacks;

    // // Begin - MIDIParser
    // virtual void OnLoadedFromFile(const char* filename) {}
    // virtual void OnLoadedFromBytes() {}

    // virtual void OnFileHeaderLoaded(const FileHeader& fileHeader) {}
    // virtual void OnFileHeaderDataLoaded(FileHeaderData& fileHeaderData) {}

    // virtual void OnTrackHeaderLoaded(TrackHeader& fileHeader) {}
    // virtual void OnTrackLoaded() {}

    // virtual void OnSysEventLoaded(uint32_t deltaTime, SysexEvent& sysEvent);
    // virtual void OnMetaEventLoaded(uint32_t deltaTime, MetaEvent& metaEvent);
    // virtual void OnChannelEventLoaded(uint32_t deltaTime, ChannelEvent& channelEvent, bool isOpti);
    // // End - MIDIParser

    virtual void OnEvent(const PMIDIEvent& event) override
    { 
        IMIDIEventReceiver::OnEvent(event); 
        if (callbacks.OnEvent)
            callbacks.OnEvent(userData, &event); 
    }
    virtual void OnSysEvent(const PMIDISysEvent& sysEvent) override
    { 
        IMIDIEventReceiver::OnSysEvent(sysEvent); 
        if (callbacks.OnSysEvent)
            callbacks.OnSysEvent(userData, &sysEvent);
    }
    virtual void OnMetaEvent(const PMIDIMetaEvent& metaEvent) 
    { 
        IMIDIEventReceiver::OnMetaEvent(metaEvent); 
        if (callbacks.OnMetaEvent)
            callbacks.OnMetaEvent(userData, &metaEvent); 
    }
    virtual void OnChannelEvent(const PMIDIChannelEvent& channelEvent) 
    { 
        IMIDIEventReceiver::OnChannelEvent(channelEvent); 
        if (callbacks.OnChannelEvent)
            callbacks.OnChannelEvent(userData, &channelEvent); 
    }

    // Begin - ChannelEvents
    virtual void OnNoteOn(const NoteOn& noteOn) 
    { 
        IMIDIEventReceiver::OnNoteOn(noteOn);
        if (callbacks.OnNoteOn)
            callbacks.OnNoteOn(userData, &noteOn); 
    }
    virtual void OnNoteOff(const NoteOff& noteOff) 
    {
        IMIDIEventReceiver::OnNoteOff(noteOff);
        if (callbacks.OnNoteOff)
            callbacks.OnNoteOff(userData, &noteOff); 
    }
    virtual void OnNoteOnOff(const NoteOnOff& noteOnOff) 
    {
        IMIDIEventReceiver::OnNoteOnOff(noteOnOff);
        if (callbacks.OnNoteOnOff)
            callbacks.OnNoteOnOff(userData, &noteOnOff); 
    }

    virtual void OnProgramChange(const ProgramChange& programChange) 
    {
        IMIDIEventReceiver::OnProgramChange(programChange);
        if (callbacks.OnProgramChange)
            callbacks.OnProgramChange(userData, &programChange); 
    }
    virtual void OnControlChange(const ControlChange& controlChange) 
    {
        IMIDIEventReceiver::OnControlChange(controlChange);
        if (callbacks.OnControlChange)
            callbacks.OnControlChange(userData, &controlChange); 
    }
    virtual void OnPitchBend(const PitchBend& pitchBend) 
    {
        IMIDIEventReceiver::OnPitchBend(pitchBend);
        if (callbacks.OnPitchBend)
            callbacks.OnPitchBend(userData, &pitchBend); 
    }

    virtual void OnNoteAfterTouch(const NoteAfterTouch& noteAfterTouch) 
    {
        IMIDIEventReceiver::OnNoteAfterTouch(noteAfterTouch);
        if (callbacks.OnNoteAfterTouch)
            callbacks.OnNoteAfterTouch(userData, &noteAfterTouch); 
    }
    virtual void OnChannelAfterTouch(const ChannelAfterTouch& channelAfterTouch) 
    {
        IMIDIEventReceiver::OnChannelAfterTouch(channelAfterTouch);
        if (callbacks.OnChannelAfterTouch)
            callbacks.OnChannelAfterTouch(userData, &channelAfterTouch); 
    }
    // End - ChannelEvents

    // // Begin - MetaEvents
    // // sf : 0 = key of C, -1 = 1 flat, 1 = 1 sharp
    // // mi : major or minor ?
    // virtual void OnKeySignature(const KeySignature& keySignature) { OnMetaEvent(keySignature); }
    virtual void OnTimeSignature(const TimeSignature& timeSignature) 
    { 
        IMIDIEventReceiver::OnTimeSignature(timeSignature);
        if (callbacks.OnTimeSignature)
            callbacks.OnTimeSignature(userData, &timeSignature); 
        OnMetaEvent(timeSignature); 
    }
    // virtual void OnMIDIPort(const MIDIPort& midiPort) { OnMetaEvent(midiPort); }
    // virtual void OnEndOfTrack(const EndOfTrack& endOfTrack) { OnMetaEvent(endOfTrack); }
    // virtual void OnTempo(const Tempo& tempo) { OnMetaEvent(tempo); }
    // virtual void OnText(const Text& text) { OnMetaEvent(text); }
    // virtual void OnCopyright(const Copyright& copyright) { OnMetaEvent(copyright); }
    // virtual void OnTrackName(const TrackName& trackName) { OnMetaEvent(trackName); }
    // virtual void OnInstrumentName(const InstrumentName& instrumentName) { OnMetaEvent(instrumentName); }
    // virtual void OnLyrics(const Lyrics& lyrics) { OnMetaEvent(lyrics); }
    // virtual void OnMarker(const Marker& marker) { OnMetaEvent(marker); }
    // virtual void OnCuePoint(const CuePoint& cuePoint) { OnMetaEvent(cuePoint); }
    // // End - MetaEvents

    // virtual void OnUnsupportedEvent(const UnsupportedEvent& unsupportedEvent) 
    // { 

    // }
};

extern "C"
{
    MIDIPARSEREXPORT void MIDIMusic_Dispatch(void* userData, MIDIEventCallbacks callbacks, class MIDIMusic* music);
}