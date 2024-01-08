#pragma once

#include <vector>
#include <memory>
// #include "json/single_include/nlohmann/json.hpp"
#include "PMIDIEvent.h"

class MIDIPARSEREXPORT MIDIMusic
{
public:
    struct MIDIPARSEREXPORT TrackData
    {
        std::vector<std::shared_ptr<PMIDIEvent>> midiEvents;

        template<typename CALLBACK>
        void ForEachEvent(CALLBACK&& callback)
        {
            std::vector<std::shared_ptr<PMIDIEvent>>::iterator it = midiEvents.begin();
            while (it != midiEvents.end())
            {
                bool shouldBeRemoved = callback(*it);

                if (shouldBeRemoved)
                {
                    uint32_t removedEventDeltatime = (*it)->deltaTime;
                    it = midiEvents.erase(it);
                    if (it == midiEvents.end())
                        break;
                    (*it)->deltaTime += removedEventDeltatime;
                }
                else 
                {
                    it++;
                }
            }
        }
    };

    FileHeaderData fileHeaderData;
    std::vector<TrackData> tracks;

    int16_t ticksPerQuarterNote = 4; // is 4 the default ?

    int8_t sf = 0;
    uint8_t mi = 0;
    bool isTonalitySet = false;

    template<typename CALLBACK>
    void ForEachTrack(CALLBACK&& callback)
    {
        std::vector<TrackData>::iterator it = tracks.begin();
        while (it != tracks.end())
        {
            bool shouldBeRemoved = callback(*it);

            if (shouldBeRemoved)
            {
                it = tracks.erase(it);
            }
            else 
            {
                it++;
            }
        }
    }
};

#include "IMIDIEventReceiver.h"

class MIDIPARSEREXPORT MIDIMusicFiller : public IMIDIEventReceiver
{
public:
    using Super = IMIDIEventReceiver;

    class MIDIMusic* music = nullptr;
    uint32_t currentTrackIndex = 0;

    // Begin - MIDIParser
    virtual void OnFileHeaderDataLoaded(FileHeaderData& fileHeaderData) override
    {
        Super::OnFileHeaderDataLoaded(fileHeaderData);
        music->fileHeaderData = fileHeaderData;
        music->ticksPerQuarterNote = fileHeaderData.delta.ticksPerQuarterNote;
        music->tracks.resize(fileHeaderData.nbTracks);
        currentTrackIndex = 0;
    }

    void OnTrackLoaded()
    {
        Super::OnTrackLoaded();
        currentTrackIndex++;
    }
    // End - MIDIParser

    // Begin - ChannelEvents
    virtual void OnNoteOn(const NoteOn& noteOn) override;
    virtual void OnNoteOff(const NoteOff& noteOff) override;
    virtual void OnProgramChange(const ProgramChange& programChange) override;
    virtual void OnControlChange(const ControlChange& controlChange) override;
    virtual void OnPitchBend(const PitchBend& pitchBend) override;
    virtual void OnNoteAfterTouch(const NoteAfterTouch& noteAfterTouch) override;
    virtual void OnChannelAfterTouch(const ChannelAfterTouch& channelAfterTouch) override;
    // // End - ChannelEvents

    // Begin - MetaEvents
    // sf : 0 = key of C, -1 = 1 flat, 1 = 1 sharp
    // mi : major or minor ?
    virtual void OnKeySignature(const KeySignature& keySignature) override;
    virtual void OnTimeSignature(const TimeSignature& timeSignature) override;
    virtual void OnMIDIPort(const MIDIPort& midiPort) override;
    virtual void OnEndOfTrack(const EndOfTrack& endOfTrack) override;
    virtual void OnTempo(const Tempo& tempo) override;
    virtual void OnText(const Text& text) override;
    virtual void OnCopyright(const Copyright& copyright) override;
    virtual void OnTrackName(const TrackName& trackName) override;
    virtual void OnInstrumentName(const InstrumentName& instrumentName) override;
    virtual void OnLyrics(const Lyrics& lyrics) override;
    virtual void OnMarker(const Marker& marker) override;
    virtual void OnCuePoint(const CuePoint& cuePoint) override;
    // End - MetaEvents

    // Begin - SysEvents
    virtual void OnSysEvent(const PMIDISysEvent& sysEvent) override;
    // End - SysEvents

    virtual void OnUnsupportedEvent(const UnsupportedEvent& unsupportedEvent) override;


    virtual void OnEvent(const PMIDIEvent& event) override;
};

