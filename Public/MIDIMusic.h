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
                    uint32_t removedEventDeltatime = (*it)->deltaTime;
                    it = midiEvents.erase(it);
                    if (it != midiEvents.end())
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



class MIDIMusicFiller : public IMIDIEventReceiver
{
public:
    using Super = IMIDIEventReceiver;

    class MIDIMusic* music = nullptr;
    uint32_t currentTrackIndex = 0;

    template<typename T>
    void CopyEvent(const T& event)
    {
        music->tracks[currentTrackIndex].midiEvents.emplace_back(new T(event));
    }

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
    virtual void OnNoteOn(const NoteOn& noteOn) override
    {
        Super::OnNoteOn(noteOn);
        // NoteOn* noteOn = new NoteOn();
        // noteOn->start = timePerTrack[currentTrackIndex];
        // noteOn->channel = channel;
        // noteOn->key = key;
        // noteOn->velocity = velocity;

        CopyEvent(noteOn);
    }
    virtual void OnNoteOff(const NoteOff& noteOff) override
    {
        Super::OnNoteOff(noteOff);
        CopyEvent(noteOff);
    }
    virtual void OnProgramChange(const ProgramChange& programChange) override
    {
        Super::OnProgramChange(programChange);
        CopyEvent(programChange);
    }
    virtual void OnControlChange(const ControlChange& controlChange) override
    {
        Super::OnControlChange(controlChange);
        CopyEvent(controlChange);
    }
    virtual void OnPitchBend(const PitchBend& pitchBend) override
    {
        Super::OnPitchBend(pitchBend);
        CopyEvent(pitchBend);
    }


    // virtual void OnControlChange(int channel, EControlChange ctrl, int value) override {}
    // virtual void OnPitchBend(int channel, int value) override {}
    // // End - ChannelEvents

    // // Begin - MetaEvents
    // // sf : 0 = key of C, -1 = 1 flat, 1 = 1 sharp
    // // mi : major or minor ?
    // virtual void OnKeySignature(int8_t sf, uint8_t mi) override
    // {
    //     music->sf = sf;
    //     music->mi = mi;
    //     music->isTonalitySet = true;
    // }
    // virtual void OnText(const char* text, uint32_t length) override {}
    // virtual void OnCopyright(const char* copyright, uint32_t length) override {}
    // virtual void OnTrackName(const char* trackName, uint32_t length) override {}
    // virtual void OnInstrumentName(const char* instrumentName, uint32_t length) override {}
    // virtual void OnLyric(const char* lyric, uint32_t length) override {}
    // virtual void OnMarker(const char* markerName, uint32_t length) override {}
    // virtual void OnCuePoint(const char* cuePointName, uint32_t length) override {}
    // // End - MetaEvents
};

// Convert NoteOn events and NoteOff events into NoteOnOff events
class MIDIMusic_NoteOnOffConverter
{
public:
    using Super = IMIDIEventReceiver;

    std::vector<std::shared_ptr<NoteOnOff>> openNotes;

    void Convert(MIDIMusic& music)
    {
        for (MIDIMusic::TrackData& trackData : music.tracks)
        {
            trackData.ForEachEvent(*this);
            openNotes.clear();
        }

        openNotes.clear();
    }

    bool operator()(std::shared_ptr<PMIDIEvent>& event)
    {
        for (std::shared_ptr<NoteOnOff>& noteOnOff : openNotes)
        {
            noteOnOff->duration += event->deltaTime;
        }

        if (typeid(*event) == typeid(NoteOn))
        {
            NoteOn& noteOn = *(NoteOn*)event.get();

            std::shared_ptr<NoteOnOff> noteOnOff = std::make_shared<NoteOnOff>();;
            
            noteOnOff->deltaTime = noteOn.deltaTime;
            noteOnOff->channel = noteOn.channel;
            noteOnOff->key = noteOn.key;
            noteOnOff->velocity = noteOn.velocity;

            openNotes.push_back(noteOnOff);

            event = std::move(noteOnOff);

            return false;
        }
        else if (typeid(*event) == typeid(NoteOff))
        {
            NoteOff& noteOff = *(NoteOff*)event.get();

            auto isSame = [&noteOff](const std::shared_ptr<NoteOnOff>& noteOnOff) 
            {
                return noteOnOff->channel == noteOff.channel && noteOnOff->key == noteOff.key;
            };
            auto it = std::find_if(openNotes.begin(), openNotes.end(), isSame);
            NoteOnOff& noteOnOff = **it;
            openNotes.erase(it);

            return true;
        }
        return false;
    }
};



