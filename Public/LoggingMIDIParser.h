#pragma once

#include "AdvancedMIDIParser.h"
#include <fstream>

class LoggingMIDIParser : public AdvancedMIDIParser
{
public:
    std::ofstream stream;
    using Super = AdvancedMIDIParser;
    
    LoggingMIDIParser(const char* filename)
    {
        stream.open(filename);
    }

    ~LoggingMIDIParser()
    {
        stream.close();
    }

    virtual void OnSysEventLoaded(uint32_t deltaTime, SysexEvent& sysEvent) override 
    {
        Super::OnSysEventLoaded(deltaTime, sysEvent);

        stream << "track-sysex" << '\n';
        stream << "  time: " << deltaTime << std::endl;
    } 
    virtual void OnMetaEventLoaded(uint32_t deltaTime, MetaEvent& metaEvent) override 
    {
        Super::OnMetaEventLoaded(deltaTime, metaEvent);

        stream << "track-meta\n";
        stream << "   time: " << deltaTime << '\n';
        stream << "   type: " << (int) metaEvent.type << "[" << MIDIParser::MidiMetaToStr(metaEvent.type) << "]\n";
        stream << "   length: " << (int) metaEvent.length << std::endl; 
    }

    virtual void OnChannelEventLoaded(uint32_t deltaTime, ChannelEvent& channelEvent, bool isOpti) override
    {
        AdvancedMIDIParser::OnChannelEventLoaded(deltaTime, channelEvent, isOpti);

        stream << "track-midi" << '\n';
        stream << "  time: " << deltaTime << '\n';
        stream << "  status: " << (int) channelEvent.message << "[" << MIDIParser::ENoteEventToStr(channelEvent.message) << "]" << '\n';
        stream << "  channel: " << (int) channelEvent.channel << '\n';
        stream << "  param1: " << (int) channelEvent.param1 << '\n';
        stream << "  param2: " << (int) channelEvent.param2 << std::endl;
    }

//     virtual void OnNoteOn(int channel, int key, int velocity) 
//     {
//         timeScheduler.RunAt(timePerTrack[currentTrackIndex] * (tempo / ticksPerQuarterNote) / 1000, [channel, key, velocity]()
//         {
// //             // if (channelEvent.param1 > 55 && channelEvent.param1 < 62)
// //             // if (channelEvent.param1 > 50 && channelEvent.param1 !=69 && channelEvent.channel < 7)
// // fluid_synth_program_select(synth, 0, undertalesfID, 0, 64);
//             fluid_synth_noteon(synth, channel, key, velocity);
//         });
//     }
//     virtual void OnNoteOff(int channel, int key) 
//     {
//         timeScheduler.RunAt(timePerTrack[currentTrackIndex] * (tempo / ticksPerQuarterNote) / 1000, [channel, key]()
//         {
//             fluid_synth_noteoff(synth, channel, key);
//         });
//     }
//     virtual void OnProgramChange(int channel, int program) 
//     {
//         timeScheduler.RunAt(timePerTrack[currentTrackIndex] * (tempo / ticksPerQuarterNote) / 1000, [channel, program]()
//         {
//             fluid_synth_program_change(synth, channel, program);
//         });
//     }
//     virtual void OnControlChange(int channel, EControlChange ctrl, int value) 
//     {
//         timeScheduler.RunAt(timePerTrack[currentTrackIndex] * (tempo / ticksPerQuarterNote) / 1000, [channel, ctrl, value]()
//         {
//             fluid_synth_cc(synth, channel, (int)ctrl, value);
//         });
//     }
//     virtual void OnPitchBend(int channel, int value) 
//     {
//         timeScheduler.RunAt(timePerTrack[currentTrackIndex], [channel, value]()
//         {
//             fluid_synth_pitch_bend(synth, channel, value);
//         });
//     }


//     virtual void OnKeySignature(uint8_t sf, uint8_t mi) override
//     {
//         // std::cout << "sf : " << (int)sf << std::endl;
//         // if (mi == 0)
//         //     std::cout << "mi : major" << std::endl;
//         // else if (mi == 1)
//         //     std::cout << "mi : minor" << std::endl;
//         // else
//         //     std::cout << "mi : " << (int)mi << std::endl;
//     }

//     virtual void OnText(const char* text, uint32_t length) override
//     {
//         // std::cout << "OnText : " << std::string(text, length) << std::endl;
//     }
//     virtual void OnCopyright(const char* copyright, uint32_t length) override
//     {
//         // std::cout << "OnCopyright : " << std::string(copyright, length) << std::endl;
//     }
//     virtual void OnTrackName(const char* trackName, uint32_t length) override
//     {
//         // std::cout << "OnTrackName : " << std::string(trackName, length) << std::endl;
//     }
//     virtual void OnInstrumentName(const char* instrumentName, uint32_t length) override
//     {
//         // std::cout << "OnInstrumentName : " << std::string(instrumentName, length) << std::endl;
//     }
//     virtual void OnLyric(const char* lyric, uint32_t length) override
//     {
//         // std::cout << "OnLyric : " << std::string(lyric, length) << std::endl;
//     }
//     virtual void OnMarker(const char* markerName, uint32_t length) override
//     {
//         // std::cout << "OnMarker : " << std::string(markerName, length) << std::endl;
//     }
//     virtual void OnCuePoint(const char* cuePointName, uint32_t length) override
//     {
//         // std::cout << "OnCuePoint : " << std::string(cuePointName, length) << std::endl;
//     }
};