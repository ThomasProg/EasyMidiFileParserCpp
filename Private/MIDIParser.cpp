#include "MIDIParser.h"

#include <fstream>
#include <assert.h>
#include <algorithm>
#include <utility>
#include <concepts>
#include <bit>

template<typename T>
requires std::integral<T>
bool IsBigEndianRuntime()
{
    inline const union Temp{
        T i;
        char c[1];
    } temp = {0x01020304};

    return temp.c[0] == 1;
}

template<typename T>
requires std::integral<T>
inline void BigToNativeEndian(T& num)
{
    if constexpr (std::endian::native == std::endian::big)
    {
        // Already in native endian, do nothing
    }
    else if constexpr (std::endian::native == std::endian::little)
    {
        num = std::byteswap(num);
    }
    else
    {
        if (!IsBigEndianRuntime<T>())
        {
            num = std::byteswap(num);
        }
    }
}

template<typename T>
requires std::integral<T>
inline void NativeToBigEndian(T& num)
{
    BigToNativeEndian(num);
}

MIDIParser::FileHeader* MIDIParser::LoadFileHeaderFromBytes(const char*& bytes, size_t size)
{
    FileHeader* header = (FileHeader*) bytes;
    bytes += sizeof(FileHeader);
    BigToNativeEndian(header->size);

    assert(!memcmp(header->name, "MThd", 4));
    assert(header->size == 6);

    return header;
}

MIDIParser::FileHeaderData MIDIParser::LoadFileHeaderDataFromBytes(const char*& bytes, size_t size)
{
    struct FileHeaderDataBuffer
    {
        // <format >
        uint16_t format;

        // <ntrks>
        // defines the amount of track chunks
        int16_t nbTracks; 
        
        int8_t division;
    }; // TODO

    FileHeaderData fileHeaderData;

    fileHeaderData.format = * (uint16_t*) bytes;
    bytes += sizeof(uint16_t);
    BigToNativeEndian(fileHeaderData.format);

    fileHeaderData.nbTracks = * (uint16_t*) bytes;
    bytes += sizeof(uint16_t);
    BigToNativeEndian(fileHeaderData.nbTracks);

    int8_t* division = (int8_t*) bytes;
    bytes += sizeof(uint16_t);

    fileHeaderData.isTicksPerFrame = division[0] < 0; 
    if(fileHeaderData.isTicksPerFrame)
    {
        fileHeaderData.delta.perFrame.SMPTEFormat = -(signed char)division[0];
        fileHeaderData.delta.perFrame.ticksPerFrame = (unsigned) division[1];
    }
    else
    {
        fileHeaderData.delta.ticksPerQuarterNote = ((unsigned)division[0] << 8) | ((unsigned)division[1] & 0xff);

        // BigToNativeEndian(division);
        // fileHeaderData.delta.ticksPerQuarterNote = division;
    }


    // fileHeaderData.divi = * (uint16_t*) bytes;
    // bytes += sizeof(uint16_t);

    // BigToNativeEndian(data->division);

    // Files with format 0 MUST have only 1 track
    assert(fileHeaderData.format != 0 || fileHeaderData.nbTracks == 1);

    return fileHeaderData;
}

// void WriteVarLen(register uint32_t value)
// {
//    register uint32_t buffer;
//    buffer = value & 0x7F;

//    while ( (value >>= 7) )
//    {
//      buffer <<= 8;
//      buffer |= ((value & 0x7F) | 0x80);
//    }

//    while (TRUE)
//    {
//       putc(buffer,outfile);
//       if (buffer & 0x80)
//           buffer >>= 8;
//       else
//           break;
//    }
// }

uint32_t ReadVarLen(const char*& bytes)
{
    register uint32_t value;
    register unsigned char c;

    if ( (value = bytes[0]) & 0x80 )
    {
        value &= 0x7F;
        do
        {
            bytes++;
            value = (value << 7) + ((c = bytes[0]) & 0x7F);
        } while (c & 0x80);
    }

    bytes++;
    return value;
}

const char* MIDIParser::MidiMetaToStr(const EMidiMeta& midiMeta)
{
    switch (midiMeta) 
    {
        case EMidiMeta::SEQ_NUM: return "Sequence Number";
        case EMidiMeta::TEXT: return "Text";
        case EMidiMeta::COPYRIGHT: return "Copyright";
        case EMidiMeta::TRACK_NAME: return "Track Name";
        case EMidiMeta::INSTRUMENT_NAME: return "Instrument Name";
        case EMidiMeta::LYRICS: return "Lyrics";
        case EMidiMeta::MAKER: return "Maker";
        case EMidiMeta::CUE_POINT: return "Cue Point";
        case EMidiMeta::CHANNEL_PREFIX: return "Channel Prefix";
        case EMidiMeta::END_OF_TRACK: return "End of Track";
        case EMidiMeta::SET_TEMPO: return "Set Tempo";
        case EMidiMeta::SMPTE_OFFSET: return "SMPTE Offset";
        case EMidiMeta::TIME_SIGNATURE: return "Time Signature";
        case EMidiMeta::KEY_SIGNATURE: return "Key Signature";
        case EMidiMeta::SEQ_SPECIFIC: return "Sequencer Specific";

        default: return "(unknown)";
    }
}

const char* MIDIParser::SysEventToStr(const ESysEvent& sysEvent)
{
    switch (sysEvent) 
    {
        case ESysEvent::SYSTEM_EXCLUSIVE: return "System Exclusive";

        case ESysEvent::MIDI_TIMING_CODE: return "MIDI Timing Code";
        case ESysEvent::SONG_POSITION_POINTER: return "Song Position Pointer";
        case ESysEvent::SONG_SELECT: return "Song Select";
        case ESysEvent::TUNE_REQUEST: return "Tune Request";
        case ESysEvent::ESCAPE_EVENT: return "Escape Event (or continuation)";

        case ESysEvent::TIMING_CLOCK: return "Timing Clock";
        case ESysEvent::START_SEQUENCE: return "Start Sequence";
        case ESysEvent::CONTINUE_SEQUENCE: return "Continue Sequence";
        case ESysEvent::STOP_SEQUENCE: return "Stop Sequence";
        case ESysEvent::ACTIVE_SENSING: return "Active Sensing";
        case ESysEvent::SYSTEM_RESET: return "System reset";

        default: return "(unknown)";
    }
}

const char* MIDIParser::ENoteEventToStr(const ENoteEvent& noteEvent)
{
    switch (noteEvent) {
        case ENoteEvent::NOTE_OFF: return "Note Off";
        case ENoteEvent::NOTE_ON: return "Note On";
        case ENoteEvent::NOTE_AFTER_TOUCH: return "Note Aftertouch";
        case ENoteEvent::CONTROL_CHANGE: return "Control Change";
        case ENoteEvent::PGM_CHANGE: return "Program Change";
        case ENoteEvent::CHANNEL_AFTER_TOUCH: return "Channel Aftertouch";
        case ENoteEvent::PITCH_BEND: return "Pitch Bend";

        default: return "(unknown)";
    }
}

MIDIParser::MetaEvent MIDIParser::LoadMetaEventFromBytes(const char*& bytes, size_t size)
{
    MetaEvent metaEvent;
    metaEvent.type = * (EMidiMeta*) bytes;
    bytes += sizeof(EMidiMeta);

    metaEvent.length = ReadVarLen(bytes);

    assert(metaEvent.length >= 0);

    metaEvent.bytes = (uint8_t*) bytes;
    bytes += metaEvent.length;

    return metaEvent;
}

int GetMIDIEventDataLength(MIDIParser::ENoteEvent channelEvent)
{
    switch (channelEvent) {
        case MIDIParser::ENoteEvent::PGM_CHANGE: 
        case MIDIParser::ENoteEvent::CHANNEL_AFTER_TOUCH: 
            return 1;

        default: 
            return 2;
    }
}

MIDIParser::SysexEvent MIDIParser::LoadSysexEventFromBytes(const MessageStatus& status, const char*& bytes, size_t size)
{
    MIDIParser::SysexEvent sysEvent;
    sysEvent.type = (ESysEvent) status.value;

    sysEvent.messageLength = ReadVarLen(bytes);
    sysEvent.message = bytes;

    
    bytes += sysEvent.messageLength;
    return sysEvent;
}

MIDIParser::ChannelEvent MIDIParser::LoadChannelEventDataFromBytes(MessageStatus& midiEvent, const char*& bytes, size_t size)
{
    MIDIParser::ChannelEvent channelEvent;

    channelEvent.message = midiEvent.GetMessage();

    int8_t channelID = midiEvent.GetChannel();
    BigToNativeEndian(channelID);
    channelEvent.channel = channelID; 

    int dataLength = GetMIDIEventDataLength(channelEvent.message);

    channelEvent.param1 = (dataLength > 0 ? bytes[0] : 0);
    channelEvent.param2 = (dataLength > 1 ? bytes[1] : 0);

    bytes += dataLength;

    return channelEvent;
}

MIDIParser::TrackHeader MIDIParser::LoadTrackHeaderFromBytes(const char*& bytes, size_t size)
{
    TrackHeader trackChunk = * (TrackHeader*) bytes;
    bytes += sizeof(TrackHeader);

    BigToNativeEndian(trackChunk.length);

    return trackChunk;
}

void MIDIParser::LoadTrackFromBytes(MessageStatus& runtimeStatus, const char*& bytes, size_t size)
{
    TrackHeader trackChunk = LoadTrackHeaderFromBytes(bytes, size);
    OnTrackHeaderLoaded(trackChunk);

    if (trackChunk.length == 0)
        return;

    const char* startBytes = bytes;

    // http://midi.teragonaudio.com/tech/midifile/evts.htm

    for (size_t j = 0; j < trackChunk.length; j++)
    {
        uint32_t deltaTime = ReadVarLen(bytes); 

        MessageStatus* status = (MessageStatus*) bytes;
        bytes += sizeof(MessageStatus); 

        if (!status->IsValidStatusEvent()) [[likely]] // If not Status Byte
        {
            // Use previous status byte
            assert(runtimeStatus.IsValid());
            MIDIParser::ChannelEvent channelEvent = LoadChannelEventDataFromBytes(runtimeStatus, bytes, size);
            OnChannelEventLoaded(deltaTime, channelEvent, true);
        }
        else if (status->IsSysExEvent()) [[unlikely]]
        {
            runtimeStatus.Reset();
            SysexEvent sysEvent = LoadSysexEventFromBytes(*status, bytes, size);
            OnSysEventLoaded(deltaTime, sysEvent);
        }
        else if (status->IsMetaEvent()) [[unlikely]] // Non-MIDI events / RESET byte
        {
            runtimeStatus.Reset();

            // parse meta event
            MetaEvent metaEvent = LoadMetaEventFromBytes(bytes, size);
            OnMetaEventLoaded(deltaTime, metaEvent);

            if (metaEvent.type == EMidiMeta::END_OF_TRACK)
                break;
        }
        else [[likely]] // If normal status byte
        {
            // Update status byte
            runtimeStatus = *status;
            MIDIParser::ChannelEvent channelEvent = LoadChannelEventDataFromBytes(runtimeStatus, bytes, size);
            OnChannelEventLoaded(deltaTime, channelEvent, false);
        }
    }

    const char* temp = startBytes + trackChunk.length;
    assert(bytes - temp == 0);

    // bytes = startBytes + trackChunk->length;

    OnTrackLoaded();
}

void MIDIParser::LoadFromBytes(const char* bytes, size_t size)
{
    FileHeader* header = LoadFileHeaderFromBytes(bytes, size);
    OnFileHeaderLoaded(header);
    FileHeaderData data = LoadFileHeaderDataFromBytes(bytes, size);
    OnFileHeaderDataLoaded(data);
    
    MessageStatus runtimeStatus;
    for (size_t i = 0; i < data.nbTracks; i++)
    {
        LoadTrackFromBytes(runtimeStatus, bytes, size);
    }

    OnLoadedFromBytes();
}

void MIDIParser::LoadFromFile(const char* filename)
{
    std::ifstream file (filename, std::ios::in|std::ios::binary|std::ios::ate);
    if (file.is_open())
    {
        size_t size = file.tellg();
        char* memblock = new char [size];
        file.seekg (0, std::ios::beg);
        file.read (memblock, size);
        file.close();

        LoadFromBytes(memblock, size);
        OnLoadedFromFile(filename);

        delete[] memblock;
    }
    else
    {
        throw std::runtime_error("Couldn't open file : " + std::string(filename));
    }
}