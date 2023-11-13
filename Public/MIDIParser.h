#ifndef _MIDI_PARSER_H_
#define _MIDI_PARSER_H_

#include <cstdint>
#include <unordered_map>
#include <string>
#include "MIDIHelpers.h"

#include <stdexcept>

#ifdef MAKE_DLL // CPP
#define MIDIPARSEREXPORT __declspec(dllexport)
#else
#define MIDIPARSEREXPORT __declspec(dllimport)
#endif

class BufferReader;

class MIDIPARSEREXPORT MIDIParser
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

public:
    std::unordered_map<const void*, std::string> byteToDataStr;
    BufferReader* debugBufferPtr = nullptr;

public:
    const char* GetDataStrFromByte(void* byte) const;

    virtual void OnLoadedFromFile(const char* filename) {}
    virtual void OnLoadedFromBytes() {}

    virtual void OnFileHeaderLoaded(const FileHeader& fileHeader) {}
    virtual void OnFileHeaderDataLoaded(FileHeaderData& fileHeaderData) {}

    virtual void OnTrackHeaderLoaded(TrackHeader& fileHeader) {}
    virtual void OnTrackLoaded() {}

    virtual void OnSysEventLoaded(uint32_t deltaTime, SysexEvent& sysEvent) {}
    virtual void OnMetaEventLoaded(uint32_t deltaTime, MetaEvent& metaEvent) {}
    virtual void OnChannelEventLoaded(uint32_t deltaTime, ChannelEvent& channelEvent, bool isOpti) {}


    static void Reduce(size_t& size, size_t reduced);

    /*static*/ const FileHeader LoadFileHeaderFromBuffer(BufferReader& buffer);
    static FileHeaderData LoadFileHeaderDataFromBuffer(BufferReader& buffer);

    void LoadTrackFromBuffer(BufferReader& buffer, MessageStatus& runtimeStatus);
    /* static */ TrackHeader LoadTrackHeaderFromBuffer(BufferReader& buffer);

    /* static */ MetaEvent LoadMetaEventFromBuffer(BufferReader& buffer);
    /* static */ ChannelEvent LoadChannelEventDataFromBuffer(MessageStatus& midiEvent, BufferReader& buffer);
    static SysexEvent LoadSysexEventFromBuffer(const MessageStatus& status, BufferReader& buffer);

    void LoadFromBuffer(BufferReader& buffer);
    void LoadFromBytes(const char* bytes, size_t size);
    void LoadFromFile(const char* filename);

    static const char* ENoteEventToStr(const ENoteEvent& noteEvent);
    static const char* MidiMetaToStr(const EMidiMeta& midiMeta);
    static const char* SysEventToStr(const ESysEvent& sysEvent);
};

#endif // _MIDI_PARSER_H_