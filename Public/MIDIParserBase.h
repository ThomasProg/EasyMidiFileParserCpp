#ifndef _MIDI_PARSER_BASE_H_
#define _MIDI_PARSER_BASE_H_

#include <cstdint>
#include <unordered_map>
#include <string>
#include "MIDIHelpers.h"
#include "Macros.h"

#include <stdexcept>

class BufferReader;

// If you want to parse midi from a file or a buffer, use this class.
// 0 dynamic memory allocations.
// Uses virtual functions as callbacks.
class MIDIPARSEREXPORT MIDIParserBase
{
public:
    // <MTrk event>
    struct MIDIPARSEREXPORT TrackEvent
    {
        // <delta-time>
        // Variable-length, int32_t is the maximum size this variable can have
        uint32_t deltaTime;
    };

    struct MIDIPARSEREXPORT Event
    {
        uint64_t midiEvent;
        uint64_t sysexEvent;
        uint64_t metaEvent;
    };

    // class MIDIPARSEREXPORT Observer
    // {
    // public:
    //     virtual void OnLoadedFromFile(const char* filename) {}
    //     virtual void OnLoadedFromBytes() {}

    //     virtual void OnFileHeaderLoaded(const FileHeader& fileHeader) {}
    //     virtual void OnFileHeaderDataLoaded(FileHeaderData& fileHeaderData) {}

    //     virtual void OnTrackHeaderLoaded(TrackHeader& fileHeader) {}
    //     virtual void OnTrackLoaded() {}

    //     virtual void OnSysEventLoaded(uint32_t deltaTime, SysexEvent& sysEvent) {}
    //     virtual void OnMetaEventLoaded(uint32_t deltaTime, MetaEvent& metaEvent) {}
    //     virtual void OnChannelEventLoaded(uint32_t deltaTime, ChannelEvent& channelEvent, bool isOpti) {}
    // };

    class IMIDIEventReceiver* observer;

public:
    std::unordered_map<const void*, std::string> byteToDataStr;
    BufferReader* debugBufferPtr = nullptr;

    void AddDebugLog(const void* data, const std::string&& message)
    {
        byteToDataStr.emplace(data, message);
    }

public:
    const char* GetDataStrFromByte(void* byte) const;
    static void Reduce(size_t& size, size_t reduced);

    /*static*/ const FileHeader LoadFileHeaderFromBuffer(BufferReader& buffer);
    static FileHeaderData LoadFileHeaderDataFromBuffer(BufferReader& buffer);

    void LoadTrackFromBuffer(BufferReader& buffer, MessageStatus& runtimeStatus);
    /* static */ TrackHeader LoadTrackHeaderFromBuffer(BufferReader& buffer);

    /* static */ MetaEvent LoadMetaEventFromBuffer(BufferReader& buffer);
    /* static */ ChannelEvent LoadChannelEventDataFromBuffer(MessageStatus& midiEvent, BufferReader& buffer);
    /* static */ SysexEvent LoadSysexEventFromBuffer(const MessageStatus& status, BufferReader& buffer);

    void LoadFromBuffer(BufferReader& buffer);
    void LoadFromBytes(const char* bytes, size_t size);
    void LoadFromFile(const char* filename);

    static const char* ENoteEventToStr(const ENoteEvent& noteEvent);
    static const char* MidiMetaToStr(const EMidiMeta& midiMeta);
    static const char* SysEventToStr(const ESysEvent& sysEvent);
};

extern "C"
{
    inline MIDIPARSEREXPORT MIDIParserBase* CreateMIDIParser()
    {
        return new MIDIParserBase();
    }

    inline MIDIPARSEREXPORT void ParseFromParser(MIDIParserBase* parser, const char* path)
    {
        parser->LoadFromFile(path);
    }

    inline MIDIPARSEREXPORT void DeleteMIDIParser(MIDIParserBase* parser)
    {
        delete parser;
    }
}

#endif // _MIDI_PARSER_H_