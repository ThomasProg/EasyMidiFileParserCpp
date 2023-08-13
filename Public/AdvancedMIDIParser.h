#ifndef _ADVANCED_MIDI_PARSER_H_
#define _ADVANCED_MIDI_PARSER_H_

#include "MIDIParser.h"
#include <vector>

class MIDIPARSEREXPORT AdvancedMIDIParser : public MIDIParser
{
public:
    int16_t nbTracks = 0;
    int16_t ticksPerQuarterNote;

    // only 3 bytes
    // msPerQuarterNote;
    uint32_t tempo = 500000; // 120 bpm by default 
    uint32_t currentTrackIndex = 0;

    uint32_t* timePerTrack = nullptr; // in ms


    ~AdvancedMIDIParser();

    // Begin - MIDIParser
    virtual void OnTrackLoaded() override;
    virtual void OnFileHeaderDataLoaded(FileHeaderData& fileHeaderData) override;
    virtual void OnSysEventLoaded(uint32_t deltaTime, SysexEvent& sysEvent) override;
    virtual void OnMetaEventLoaded(uint32_t deltaTime, MetaEvent& metaEvent) override;
    virtual void OnChannelEventLoaded(uint32_t deltaTime, ChannelEvent& channelEvent, bool isOpti) override;
    // End - MIDIParser

    // Begin - ChannelEvents
    virtual void OnNoteOn(int channel, int key, int velocity) {}
    virtual void OnNoteOff(int channel, int key) {}
    virtual void OnProgramChange(int channel, int program) {}
    virtual void OnControlChange(int channel, int ctrl, int value) {}
    virtual void OnPitchBend(int channel, int value) {}
};


#endif // _ADVANCED_MIDI_PARSER_H_