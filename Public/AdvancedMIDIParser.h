#ifndef _ADVANCED_MIDI_PARSER_H_
#define _ADVANCED_MIDI_PARSER_H_

#include "MIDIParserBase.h"
#include "IMIDIEventReceiver.h"

// Parser that keeps track of deltatime and other data.
// 1 dynamic memory allocation (to allocate dt for tracks).
// Uses virtual functions as callbacks.
// Possess more specialized callback functions
class MIDIPARSEREXPORT  AdvancedMIDIParser : public IMIDIEventReceiver
{
public:
    MIDIParserBase parser;

    int16_t nbTracks = 0;
    int16_t ticksPerQuarterNote = 4; // is 4 the default ?

    // only 3 bytes
    // msPerQuarterNote;
    uint32_t tempo = 500000; // 120 bpm by default 
    uint32_t currentTrackIndex = 0;

    uint32_t* timePerTrack = nullptr; // in ms

    AdvancedMIDIParser()
    {
        parser.observer = this;
    }
    ~AdvancedMIDIParser();
    void AddTimeToTrack(int16_t trackIndex, uint32_t deltaTime);
    virtual void OnEvent(const PMIDIEvent& event) override;
    void AddDebugLog(const void* data, const std::string&& message)
    {
        parser.AddDebugLog(data, std::move(message));
    }

    // Begin - IMIDIEventReceiver
    virtual void OnFileHeaderDataLoaded(FileHeaderData& fileHeaderData) override;
    virtual void OnTrackLoaded() override;
    // End - IMIDIEventReceiver
};


#endif // _ADVANCED_MIDI_PARSER_H_