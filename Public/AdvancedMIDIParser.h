#ifndef _ADVANCED_MIDI_PARSER_H_
#define _ADVANCED_MIDI_PARSER_H_

#include "MIDIParser.h"
#include <vector>

class MIDIPARSEREXPORT AdvancedMIDIParser : public MIDIParser
{
public:
    int16_t nbTracks = 0;
    int16_t ticksPerQuarterNote = 4; // is 4 the default ?

    // only 3 bytes
    // msPerQuarterNote;
    uint32_t tempo = 500000; // 120 bpm by default 
    uint32_t currentTrackIndex = 0;

    uint32_t* timePerTrack = nullptr; // in ms


    ~AdvancedMIDIParser();

    // Begin - MIDIParser
    virtual void OnFileHeaderDataLoaded(FileHeaderData& fileHeaderData) override;
    virtual void OnTrackLoaded() override;
    virtual void OnTrackHeaderLoaded(TrackHeader& fileHeader) override;
    virtual void OnSysEventLoaded(uint32_t deltaTime, SysexEvent& sysEvent) override;
    virtual void OnMetaEventLoaded(uint32_t deltaTime, MetaEvent& metaEvent) override;
    virtual void OnChannelEventLoaded(uint32_t deltaTime, ChannelEvent& channelEvent, bool isOpti) override;
    // End - MIDIParser

    // Begin - ChannelEvents
    virtual void OnNoteOn(int channel, int key, int velocity) {}
    virtual void OnNoteOff(int channel, int key) {}
    virtual void OnProgramChange(int channel, int program) {}
    virtual void OnControlChange(int channel, EControlChange ctrl, int value) {}
    virtual void OnPitchBend(int channel, int value) {}
    // End - ChannelEvents

    // Begin - MetaEvents
    // sf : 0 = key of C, -1 = 1 flat, 1 = 1 sharp
    // mi : major or minor ?
    virtual void OnKeySignature(uint8_t sf, uint8_t mi) {}
    virtual void OnText(const char* text, uint32_t length) {}
    virtual void OnCopyright(const char* copyright, uint32_t length) {}
    virtual void OnTrackName(const char* trackName, uint32_t length) {}
    virtual void OnInstrumentName(const char* instrumentName, uint32_t length) {}
    virtual void OnLyric(const char* lyric, uint32_t length) {}
    virtual void OnMarker(const char* markerName, uint32_t length) {}
    virtual void OnCuePoint(const char* cuePointName, uint32_t length) {}
    // End - MetaEvents
};


#endif // _ADVANCED_MIDI_PARSER_H_