#ifndef _IMIDI_EVENT_RECEIVER_H_
#define _IMIDI_EVENT_RECEIVER_H_

#include "PMIDIEvent.h"

class MIDIPARSEREXPORT IMIDIEventReceiver
{
public:
    // Begin - MIDIParser
    virtual void OnLoadedFromFile(const char* filename) {}
    virtual void OnLoadedFromBytes() {}

    virtual void OnFileHeaderLoaded(const FileHeader& fileHeader) {}
    virtual void OnFileHeaderDataLoaded(FileHeaderData& fileHeaderData) {}

    virtual void OnTrackHeaderLoaded(TrackHeader& fileHeader) {}
    virtual void OnTrackLoaded() {}

    virtual void OnSysEventLoaded(uint32_t deltaTime, SysexEvent& sysEvent);
    virtual void OnMetaEventLoaded(uint32_t deltaTime, MetaEvent& metaEvent);
    virtual void OnChannelEventLoaded(uint32_t deltaTime, ChannelEvent& channelEvent, bool isOpti);
    // End - MIDIParser

    virtual void OnEvent(const PMIDIEvent& event) {}
    virtual void OnSysEvent(const PMIDISysEvent& sysEvent) {}
    virtual void OnMetaEvent(const PMIDIMetaEvent& metaEvent) {}
    virtual void OnChannelEvent(const PMIDIChannelEvent& channelEvent) {}

    // Begin - ChannelEvents
    virtual void OnNoteOn(const NoteOn& noteOn) { OnChannelEvent(noteOn); }
    virtual void OnNoteOff(const NoteOff& noteOff) { OnChannelEvent(noteOff); }
    virtual void OnNoteOnOff(const NoteOnOff& noteOnOff) { OnChannelEvent(noteOnOff); }

    virtual void OnProgramChange(const ProgramChange& programChange) { OnChannelEvent(programChange); }
    virtual void OnControlChange(const ControlChange& controlChange) { OnChannelEvent(controlChange); }
    virtual void OnPitchBend(const PitchBend& pitchBend) { OnChannelEvent(pitchBend); }
    // End - ChannelEvents

    // Begin - MetaEvents
    // sf : 0 = key of C, -1 = 1 flat, 1 = 1 sharp
    // mi : major or minor ?
    virtual void OnKeySignature(const KeySignature& keySignature) { OnMetaEvent(keySignature); }
    virtual void OnTempo(const Tempo& tempo) { OnMetaEvent(tempo); }
    virtual void OnText(const Text& text) { OnMetaEvent(text); }
    virtual void OnCopyright(const Copyright& copyright) { OnMetaEvent(copyright); }
    virtual void OnTrackName(const TrackName& trackName) { OnMetaEvent(trackName); }
    virtual void OnInstrumentName(const InstrumentName& instrumentName) { OnMetaEvent(instrumentName); }
    virtual void OnLyrics(const Lyrics& lyrics) { OnMetaEvent(lyrics); }
    virtual void OnMarker(const Marker& marker) { OnMetaEvent(marker); }
    virtual void OnCuePoint(const CuePoint& cuePoint) { OnMetaEvent(cuePoint); }
    // End - MetaEvents
};

#endif