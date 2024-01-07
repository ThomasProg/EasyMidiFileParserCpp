#pragma once

#include "IMIDIEventReceiver.h"
#include <iostream>

class EventsPrinter : public IMIDIEventReceiver
{
public:
    virtual void OnEvent(const PMIDIEvent& event) override
    {
        std::cout << "OnEvent" << std::endl;
    }
    virtual void OnSysEvent(const PMIDISysEvent& sysEvent) override
    {
        std::cout << "OnSysEvent" << std::endl;
    }
    virtual void OnMetaEvent(const PMIDIMetaEvent& metaEvent) override
    {
        std::cout << "OnMetaEvent" << std::endl;
    }
    virtual void OnChannelEvent(const PMIDIChannelEvent& channelEvent) override
    {
        std::cout << "OnChannelEvent" << std::endl;
    }

    // Begin - ChannelEvents
    virtual void OnNoteOn(const NoteOn& noteOn) override
    {
        std::cout << "OnNoteOn" << std::endl;
    }
    virtual void OnNoteOff(const NoteOff& noteOff) override
    {
        std::cout << "OnNoteOff" << std::endl;
    }
    virtual void OnNoteOnOff(const NoteOnOff& noteOnOff) override
    {
        std::cout << "OnNoteOnOff" << std::endl;
    }

    virtual void OnProgramChange(const ProgramChange& programChange) override
    {
        std::cout << "OnProgramChange" << std::endl;
    }
    virtual void OnControlChange(const ControlChange& controlChange) override
    {
        std::cout << "OnControlChange" << std::endl;
    }
    virtual void OnPitchBend(const PitchBend& pitchBend) override
    {
        std::cout << "OnPitchBend" << std::endl;
    }

    virtual void OnNoteAfterTouch(const NoteAfterTouch& noteAfterTouch) override
    {
        std::cout << "OnNoteAfterTouch" << std::endl;
    }
    virtual void OnChannelAfterTouch(const ChannelAfterTouch& channelAfterTouch) override
    {
        std::cout << "OnChannelAfterTouch" << std::endl;
    }
    // End - ChannelEvents

    // Begin - MetaEvents
    // sf : 0 = key of C, -1 = 1 flat, 1 = 1 sharp
    // mi : major or minor ?
    //virtual void OnKeySignature(const KeySignature& keySignature) { OnMetaEvent(keySignature); }
    //virtual void OnTimeSignature(const TimeSignature& timeSignature) { OnMetaEvent(timeSignature); }
    //virtual void OnMIDIPort(const MIDIPort& midiPort) { OnMetaEvent(midiPort); }
    //virtual void OnEndOfTrack(const EndOfTrack& endOfTrack) { OnMetaEvent(endOfTrack); }
    //virtual void OnTempo(const Tempo& tempo) { OnMetaEvent(tempo); }
    //virtual void OnText(const Text& text) { OnMetaEvent(text); }
    //virtual void OnCopyright(const Copyright& copyright) { OnMetaEvent(copyright); }
    //virtual void OnTrackName(const TrackName& trackName) { OnMetaEvent(trackName); }
    //virtual void OnInstrumentName(const InstrumentName& instrumentName) { OnMetaEvent(instrumentName); }
    //virtual void OnLyrics(const Lyrics& lyrics) { OnMetaEvent(lyrics); }
    //virtual void OnMarker(const Marker& marker) { OnMetaEvent(marker); }
    //virtual void OnCuePoint(const CuePoint& cuePoint) { OnMetaEvent(cuePoint); }
    // End - MetaEvents

    virtual void OnUnsupportedEvent(const UnsupportedEvent& unsupportedEvent) { }

};