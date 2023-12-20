#pragma once

#include <vector>
#include <memory>
#include <set>
// #include "json/single_include/nlohmann/json.hpp"
#include "PMIDIEvent.h"

#include "AdvancedMIDIParser.h"

class MIDIMusic
{
public:
    struct TrackData
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
                    it = midiEvents.erase(it);
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



class MidiMusicParser : public AdvancedMIDIParser
{
public:
    using Super = AdvancedMIDIParser;

    MIDIMusic* music = nullptr;
    bool parseAsPartition = true;

    // Begin - MIDIParser
    virtual void OnFileHeaderDataLoaded(FileHeaderData& fileHeaderData) override
    {
        Super::OnFileHeaderDataLoaded(fileHeaderData);
        music->fileHeaderData = fileHeaderData;
        music->tracks.resize(fileHeaderData.nbTracks);
    }
    // virtual void OnTrackLoaded() override;
    // virtual void OnTrackHeaderLoaded(TrackHeader& fileHeader) override
    // {
    //     Super::OnTrackHeaderLoaded();
    // }
    // virtual void OnSysEventLoaded(uint32_t deltaTime, SysexEvent& sysEvent) override;
    // virtual void OnMetaEventLoaded(uint32_t deltaTime, MetaEvent& metaEvent) override;
    // virtual void OnChannelEventLoaded(uint32_t deltaTime, ChannelEvent& channelEvent, bool isOpti) override;
    // End - MIDIParser

    // Begin - ChannelEvents
    virtual void OnNoteOn(int channel, int key, int velocity) 
    {
        NoteOn* noteOn = new NoteOn();
        noteOn->start = timePerTrack[currentTrackIndex];
        noteOn->channel = channel;
        noteOn->key = key;
        noteOn->velocity = velocity;
        music->tracks[currentTrackIndex].midiEvents.emplace_back(noteOn);
    }
    virtual void OnNoteOff(int channel, int key) 
    {
        NoteOff* noteOff = new NoteOff();
        noteOff->start = timePerTrack[currentTrackIndex];
        noteOff->channel = channel;
        noteOff->key = key;
        music->tracks[currentTrackIndex].midiEvents.emplace_back(noteOff);
    }
    virtual void OnProgramChange(int channel, int program)
    {
        ProgramChange* programChange = new ProgramChange();
        programChange->start = timePerTrack[currentTrackIndex];
        programChange->channel = channel;
        programChange->newProgram = program;
        music->tracks[currentTrackIndex].midiEvents.emplace_back(programChange);
    }
    virtual void OnControlChange(int channel, EControlChange ctrl, int value) {}
    virtual void OnPitchBend(int channel, int value) {}
    // End - ChannelEvents

    // Begin - MetaEvents
    // sf : 0 = key of C, -1 = 1 flat, 1 = 1 sharp
    // mi : major or minor ?
    virtual void OnKeySignature(int8_t sf, uint8_t mi) override
    {
        music->sf = sf;
        music->mi = mi;
        music->isTonalitySet = true;
    }
    virtual void OnText(const char* text, uint32_t length) {}
    virtual void OnCopyright(const char* copyright, uint32_t length) {}
    virtual void OnTrackName(const char* trackName, uint32_t length) {}
    virtual void OnInstrumentName(const char* instrumentName, uint32_t length) {}
    virtual void OnLyric(const char* lyric, uint32_t length) {}
    virtual void OnMarker(const char* markerName, uint32_t length) {}
    virtual void OnCuePoint(const char* cuePointName, uint32_t length) {}
    // End - MetaEvents
};






